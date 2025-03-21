#include "core.hpp"
#include <filesystem>

namespace lighter
{

	bool Core::init()
	{
		try
		{
			INFO("Initializing libVLC...");

			// VLC command-line arguments (modify as needed)
			const char *vlc_args[] = {
					"--verbose=2",
					"--no-video-title-show",
					"--file-caching=1000",
					"--network-caching=1000",
					"--live-caching=1000",
					"--avcodec-hw=any" // Enable hardware acceleration
			};

			// Create VLC instance
			vlcInstance = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
			if (!vlcInstance)
			{
				ERROR("Failed to initialize libVLC.");
				return false;
			}

			INFO("libVLC initialized successfully.");
			return true;
		}
		catch (const std::exception &ex)
		{
			ERROR("Core initialization failed: {}", ex.what());
			return false;
		}
	}

	void Core::cleanup()
	{
		INFO("Cleaning up libVLC resources...");

		if (vlcInstance)
		{
			libvlc_release(vlcInstance);
			vlcInstance = nullptr;
		}

		INFO("Core module cleanup complete.");
	}

	bool Core::loadMedia(const std::string &filePath)
	{
		if (!std::filesystem::exists(filePath))
		{
			ERROR("Media file does not exist: {}", filePath);
			return false;
		}

		libvlc_media_t *media = libvlc_media_new_path(filePath.c_str());
		if (!media)
		{
			ERROR("Failed to create media instance for file: {}", filePath);
			return false;
		}

		// Create a media player
		player = libvlc_media_player_new_from_media(vlcInstance, media);
		if (!player)
		{
			ERROR("Failed to create media player for file: {}", filePath);
			return false;
		}

		libvlc_media_release(media);

		INFO("Media loaded successfully: {}", filePath);
		return true;
	}

	void Core::play()
	{
		if (player)
		{
			std::thread playbackThread([this]()
																 { libvlc_media_player_play(player); });
			playbackThread.detach(); // Detach the thread to allow it to run independently
		}
	}

	void Core::pause()
	{
		if (player)
		{
			libvlc_media_player_pause(player);
		}
	}

	void Core::stop()
	{
		if (player)
		{
			libvlc_media_player_stop_async(player);
		}
	}

	bool Core::isPlaying()
	{
		if (player)
		{
			return libvlc_media_player_is_playing(player);
		}
		return false;
	}

	void Core::seek(double seconds)
	{
		if (player)
		{
			libvlc_media_player_set_time(player, static_cast<int64_t>(seconds * 1000), false);
		}
	}

} // namespace lighter
