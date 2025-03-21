#ifndef CORE_CORE
#define CORE_CORE

#include "utils.hpp"
#include "player.hpp"
#include <vlc/vlc.h>

namespace lighter
{
	class Core
	{
	public:
		libvlc_instance_t *vlcInstance = nullptr;
		// Constructor: Initializes the Core
		Core()
		{
			if (!init())
			{
				throw std::runtime_error("Failed to initialize Core");
			}
		}
		// Destructor: Cleans up resources
		~Core()
		{
			cleanup();
		}
		// Delete copy constructor and assignment operator
		Core(const Core &) = delete;
		Core &operator=(const Core &) = delete;

	private:
		bool init();
		void cleanup();
		libvlc_media_player_t *player = nullptr;
	};
}

#endif // !CORE_CORE
