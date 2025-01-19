#include "player.hpp"
#include "utils.hpp"


lighter::Player::Player() {
	INFO("Initializing Player...");
	INFO("Player initialized successfully.");
}

lighter::Player::~Player() {
	INFO("Clearing Player resources...");
	if (formatCtx)
		avformat_close_input(&formatCtx); // Frees formatCtx and its resources
	if (videoCodecCtx)
		avcodec_free_context(&videoCodecCtx); // Frees videoCodecCtx
	if (audioCodecCtx)
		avcodec_free_context(&audioCodecCtx); // Frees audioCodecCtx
}

bool lighter::Player::loadMedia(const std::string& filePath) {
	// Open the input file
	formatCtx = avformat_alloc_context();
	if (avformat_open_input(&formatCtx, filePath.c_str(), nullptr, nullptr) != 0) {
		ERROR("Error: Could not open file: {}", filePath);
		return false;
	}

	// Retrieve stream information
	if (avformat_find_stream_info(formatCtx, nullptr) < 0) {
		ERROR("Error: Could not retrieve stream info.");
		return false;
	}

	// Log media details: duration, format, and codec info
	INFO("Media Details:");
	INFO("  Format: {}", formatCtx->iformat->name);
	INFO("  Duration: {} seconds", formatCtx->duration / AV_TIME_BASE);

	// Find audio and video streams
	for (unsigned int i = 0; i < formatCtx->nb_streams; i++) {
		AVStream* stream = formatCtx->streams[i];
		AVCodecParameters* codecParams = stream->codecpar;

		// Check for video stream
		if (codecParams->codec_type == AVMEDIA_TYPE_VIDEO && videoStreamIndex == -1) {
			videoStreamIndex = i;
			const AVCodec* codec = avcodec_find_decoder(codecParams->codec_id);
			if (!codec) {
				ERROR("Error: Unsupported video codec.");
				return false;
			}
			videoCodecCtx = avcodec_alloc_context3(codec);
			avcodec_parameters_to_context(videoCodecCtx, codecParams);
			if (avcodec_open2(videoCodecCtx, codec, nullptr) < 0) {
				ERROR("Error: Could not open video codec.");
				return false;
			}
			INFO("  Video Stream: Codec: {}", codec->name);
			INFO("  Resolution: {}x{}", videoCodecCtx->width, videoCodecCtx->height);
		}

		// Check for audio stream
		if (codecParams->codec_type == AVMEDIA_TYPE_AUDIO && audioStreamIndex == -1) {
			audioStreamIndex = i;
			const AVCodec* codec = avcodec_find_decoder(codecParams->codec_id);
			if (!codec) {
				ERROR("Error: Unsupported audio codec.");
				return false;
			}
			audioCodecCtx = avcodec_alloc_context3(codec);
			avcodec_parameters_to_context(audioCodecCtx, codecParams);
			if (avcodec_open2(audioCodecCtx, codec, nullptr) < 0) {
				ERROR("Error: Could not open audio codec.");
				return false;
			}
			INFO("  Audio Stream: Codec: {}", codec->name);
			INFO("  Channels: {}; Sample Rate: {}", audioCodecCtx->channels, audioCodecCtx->sample_rate);
		}
	}

	// Ensure at least one valid stream is found
	if (videoStreamIndex == -1 && audioStreamIndex == -1) {
		ERROR("Error: No valid video or audio streams found.");
		return false;
	}

	INFO("Media loaded successfully: {}", filePath);
	return true;
}

