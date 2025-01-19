#ifndef CORE_PLAYER
#define CORE_PLAYER

#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "AudioPlayer.hpp"
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
}

namespace lighter {
	class Player {
	public:
		Player();
		~Player();

		bool loadMedia(const std::string& filePath);
		//void play();
		//void pause();
		//void stop();
		//void seek(double seconds);
		//void setVolume(int volume);

	private:
		//void decode();
		//void renderVideo();
		//void playAudio();

		// FFmpeg Contexts
		AVFormatContext* formatCtx = nullptr;
		AVCodecContext* audioCodecCtx = nullptr;
		AVCodecContext* videoCodecCtx = nullptr;

		// Audio/Video Streams
		int audioStreamIndex = -1;
		int videoStreamIndex = -1;

		// Threads for Playback
		std::thread decodeThread;
		std::thread audioThread;
		std::thread videoThread;

		// Playback State
		bool isPlaying = false;
		bool isPaused = false;

		// Synchronization
		std::mutex stateMutex;
		std::condition_variable cv;
	};
}

#endif // !CORE_PLAYBACK