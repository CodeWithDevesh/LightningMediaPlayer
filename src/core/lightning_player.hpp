#pragma once

#ifndef CORE_PLAYER
#define CORE_PLAYER

#include "vlc/vlc.h"
#include "lightning_playlist.hpp"
#include "lightning_eventmanager.hpp"
#include "lightning_utils.hpp"

namespace lighter
{

	// TODO: Add support for multiple audio/video streams
	// TODO: Add support for subtitles
	// TODO: Add support for multiple audio outputs
	// TODO: Add support for video filters
	// TODO: Add support for audio filters
	// TODO: Add support for video scaling
	// TODO: Add support for video cropping
	// TODO: Add support for video aspect ratio
	// TODO: Add support for video rotation
	// TODO: Add speed control
	// TODO: Add support for bookmarks
	// TODO: Add support for chapters

	class Player
	{
	public:
		/**
		 * @brief Constructs a Player instance.
		 *
		 * @param vlcInstance Pointer to the libVLC instance.
		 * @param playlist Pointer to the Playlist instance.
		 * @param eventManager Pointer to the EventManager instance.
		 */
		Player(libvlc_instance_t *vlcInstance, Playlist *playlist, EventManager *eventManager);

		/**
		 * @brief Destroys the Player instance and releases resources.
		 */
		~Player();

		/**
		 * @brief Starts media playback.
		 */
		void play();

		/**
		 * @brief Pauses media playback.
		 */
		void pause();

		/**
		 * @brief Stops media playback.
		 */
		void stop();

		/**
		 * @brief Seeks to a specific time in the media.
		 *
		 * @param seconds The time to seek to, in seconds.
		 */
		void seek(double seconds);

		/**
		 * @brief Sets the playback volume.
		 *
		 * @param volume The volume level (0-100).
		 */
		void setVolume(int volume);


		void setXWindow(int id);

		inline bool isPlaying() {return playing;}

	private:
		// Audio/Video Streams
		/**
		 * @brief Index of the current audio stream.
		 * Defaults to -1 if no audio stream is selected.
		 */
		int audioStreamIndex = -1;

		/**
		 * @brief Index of the current video stream.
		 * Defaults to -1 if no video stream is selected.
		 */
		int videoStreamIndex = -1;

		/**
		 * @brief Pointer to the libVLC media player instance.
		 */
		libvlc_media_player_t *player_inst = nullptr;

		/**
		 * @brief Pointer to the libVLC instance.
		 */
		libvlc_instance_t *vlc_inst = nullptr;

		/**
		 * @brief Pointer to the Playlist instance.
		 */
		Playlist *playlist;

		/**
		 * @brief Pointer to the EventManager instance.
		 */
		EventManager *eventManager;

		/**
		 * @brief Handles the event when the media changes.
		 *
		 * @param arg The event data containing information about the new media.
		 */
		void onMediaChanged(std::any arg);

		// Playback State
		/**
		 * @brief Indicates whether media is currently playing.
		 */
		bool playing = false;
	};

}

#endif // !CORE_PLAYBACK