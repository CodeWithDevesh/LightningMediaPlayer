#include <core/core.hpp>
#include <string>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/hwcontext.h>
}

// Callback function for selecting the best pixel format
static enum AVPixelFormat get_hw_format(AVCodecContext* ctx, const enum AVPixelFormat* pix_fmts) {
	for (const enum AVPixelFormat* p = pix_fmts; *p != -1; p++) {
		if (*p == AV_PIX_FMT_D3D11 || *p == AV_PIX_FMT_CUDA) {
			INFO("Using hardware-accelerated pixel format: {}", av_get_pix_fmt_name(*p));
			return *p;
		}
	}
	INFO("Falling back to software decoding");
	return AV_PIX_FMT_YUV420P;
}

int main() {
	INFO("Running the tutorial");
	lighter::Core core;
	av_log_set_level(AV_LOG_DEBUG);

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		ERROR("Could not initialize SDL - {}", SDL_GetError());
		return -1;
	}

	AVFormatContext* pFormatCtx = NULL;
	if (avformat_open_input(&pFormatCtx, "./blah2.mkv", NULL, NULL) != 0) {
		ERROR("Failed to open file");
		return -1;
	}
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
		ERROR("Failed to find stream info");
		return -1;
	}
	av_dump_format(pFormatCtx, 0, "./blah.mkv", 0);

	int videoStream = -1;
	for (int i = 0; i < pFormatCtx->nb_streams; i++) {
		if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoStream = i;
			break;
		}
	}
	if (videoStream == -1) {
		ERROR("Didn't find a video stream");
		return -1;
	}
	INFO("Video stream found {}", videoStream);

	AVCodecParameters* pCodecPar = pFormatCtx->streams[videoStream]->codecpar;
	const AVCodec* pCodec = avcodec_find_decoder(pCodecPar->codec_id);
	if (pCodec == NULL) {
		ERROR("Unsupported Codec");
		return -1;
	}

	AVCodecContext* pCodecCtx = avcodec_alloc_context3(pCodec);
	if (avcodec_parameters_to_context(pCodecCtx, pCodecPar) < 0) {
		ERROR("Couldn't allocate codec context");
		return -1;
	}

	// Attempt to set up hardware acceleration
	AVHWDeviceType hw_types[] = { AV_HWDEVICE_TYPE_D3D11VA, AV_HWDEVICE_TYPE_CUDA, AV_HWDEVICE_TYPE_VAAPI };
	AVBufferRef* hw_device_ctx = nullptr;

	for (AVHWDeviceType type : hw_types) {
		if (av_hwdevice_ctx_create(&hw_device_ctx, type, NULL, NULL, 0) == 0) {
			INFO("Hardware device context created for type: {}", av_hwdevice_get_type_name(type));
			pCodecCtx->hw_device_ctx = av_buffer_ref(hw_device_ctx);
			//pCodecCtx->get_format = get_hw_format;  // Set hardware format selection
			break;
		}
		WARN("Failed to create hardware device context for {}", av_hwdevice_get_type_name(type));
	}
	if (!hw_device_ctx) {
		WARN("Proceeding without hardware decoding support");
	}

	if (avcodec_open2(pCodecCtx, pCodec, nullptr) < 0) {
		ERROR("Couldn't open codec");
		return -1;
	}

	AVFrame* pFrame = av_frame_alloc();
	if (pFrame == NULL) {
		ERROR("Couldn't allocate a frame");
		return -1;
	}

	AVPacket* packet = av_packet_alloc();
	if (!packet) {
		ERROR("Couldn't allocate packet");
		return -1;
	}

	// Set up SDL window and renderer
	SDL_Window* window = SDL_CreateWindow("Video Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, pCodecCtx->width, pCodecCtx->height, 0);
	if (!window) {
		ERROR("Could not create SDL window - {}", SDL_GetError());
		return -1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		ERROR("Could not create SDL renderer - {}", SDL_GetError());
		return -1;
	}

	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, pCodecCtx->width, pCodecCtx->height);
	if (!texture) {
		ERROR("Could not create SDL texture - {}", SDL_GetError());
		return -1;
	}

	static enum AVPixelFormat last_format = pCodecCtx->pix_fmt;
	SwsContext* sws_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
		pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BILINEAR, NULL, NULL, NULL);

	AVFrame* sw_frame = av_frame_alloc();
	AVFrame* pFrameYUV = av_frame_alloc();
	int numBytes = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 32);
	uint8_t* buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));

	int frameCount = 0;
	while (av_read_frame(pFormatCtx, packet) >= 0) {
		if (packet->stream_index == videoStream) {
			if (avcodec_send_packet(pCodecCtx, packet) == 0) {
				while (avcodec_receive_frame(pCodecCtx, pFrame) == 0) {
					INFO("Processing frame {}", ++frameCount);

					// Check if frame is in hardware memory, if yes, transfer to CPU memory for further processing
					if (pFrame->format == AV_PIX_FMT_D3D11 || pFrame->format == AV_PIX_FMT_CUDA) {
						if (av_hwframe_transfer_data(sw_frame, pFrame, 0) < 0) {
							ERROR("Failed to transfer hardware frame to system memory");
							av_frame_free(&sw_frame);
							continue;
						}

						// Check if the frame data is valid
						if (!sw_frame->data[0]) {
							ERROR("sw_frame data is NULL after hardware transfer");
							av_frame_free(&sw_frame);
							continue;
						}

						// Reinitialize sws_ctx if necessary
						if (last_format != (AVPixelFormat)sw_frame->format) {
							if (sws_ctx) {
								sws_freeContext(sws_ctx);
							}
							sws_ctx = sws_getContext(
								pCodecCtx->width,
								pCodecCtx->height,
								(AVPixelFormat)sw_frame->format,  // Correct format from transferred frame
								pCodecCtx->width,
								pCodecCtx->height,
								AV_PIX_FMT_YUV420P,
								SWS_FAST_BILINEAR,
								NULL, NULL, NULL
							);
							last_format = (AVPixelFormat)sw_frame->format;
						}

						// Convert the frame to YUV for SDL

						av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 32);
						//sws_scale(sws_ctx, (uint8_t const* const*)sw_frame->data, sw_frame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
						sws_scale(sws_ctx, (uint8_t const* const*)sw_frame->data, sw_frame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);

						// Update the texture with the new frame
						//SDL_UpdateYUVTexture(texture, NULL, sw_frame->data[0], sw_frame->linesize[0], sw_frame->data[1], sw_frame->linesize[1], sw_frame->data[2], sw_frame->linesize[2]);
						SDL_UpdateYUVTexture(texture, NULL, pFrameYUV->data[0], pFrameYUV->linesize[0], pFrameYUV->data[1], pFrameYUV->linesize[1], pFrameYUV->data[2], pFrameYUV->linesize[2]);
						SDL_RenderClear(renderer);
						SDL_RenderCopy(renderer, texture, NULL, NULL);
						SDL_RenderPresent(renderer);

						/*av_frame_unref(sw_frame);
						av_frame_unref(pFrameYUV);*/
					}
					else {
						// Convert the frame to YUV for SDL
						AVFrame* pFrameYUV = av_frame_alloc();
						int numBytes = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 32);
						uint8_t* buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
						av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 32);
						sws_scale(sws_ctx, (uint8_t const* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);

						// Update the texture with the new frame
						SDL_UpdateYUVTexture(texture, NULL, pFrameYUV->data[0], pFrameYUV->linesize[0], pFrameYUV->data[1], pFrameYUV->linesize[1], pFrameYUV->data[2], pFrameYUV->linesize[2]);
						SDL_RenderClear(renderer);
						SDL_RenderCopy(renderer, texture, NULL, NULL);
						SDL_RenderPresent(renderer);

						av_free(buffer);
						av_frame_free(&pFrameYUV);
					}

					//av_frame_unref(pFrame);
				}
			}
		}
		av_packet_unref(packet);
	}

	// Free resources
	av_packet_free(&packet);
	av_free(buffer);
	av_frame_free(&pFrameYUV);
	av_frame_free(&sw_frame);
	av_frame_free(&pFrame);
	avcodec_free_context(&pCodecCtx);
	avformat_close_input(&pFormatCtx);
	if (hw_device_ctx) {
		av_buffer_unref(&hw_device_ctx);
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

