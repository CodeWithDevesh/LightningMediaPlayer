#pragma once

#ifndef CORE_PLAYER
#define CORE_PLAYER
#include "playlist.hpp"
#include "vlc/vlc.h"
#include "utils.hpp"

namespace lighter
{
	class Player
	{
	public:
		Player(libvlc_instance_t *vlcInstance, Playlist *playlist);
		~Player();

		void play();
		void pause();
		void stop();
		void seek(double seconds);
		void setVolume(int volume);

	private:
		// Audio/Video Streams
		int audioStreamIndex = -1;
		int videoStreamIndex = -1;
		libvlc_media_player_t	*player = nullptr;

		// Playback State
		bool isPlaying = false;
	};
}

#endif // !CORE_PLAYBACK