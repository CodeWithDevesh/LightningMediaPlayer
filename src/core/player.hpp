#pragma once

#ifndef CORE_PLAYER
#define CORE_PLAYER

#include "utils.hpp"


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

		// Audio/Video Streams
		int audioStreamIndex = -1;
		int videoStreamIndex = -1;

		std::thread playbackThread;

		// Playback State
		bool isPlaying = false;
	};
}

#endif // !CORE_PLAYBACK