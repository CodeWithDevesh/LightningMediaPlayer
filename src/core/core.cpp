#include "core.hpp"
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
}

bool lighter::Core::init() {
	try {
		INFO("Initializing FFmpeg...");
		avformat_network_init();
		INFO("FFmpeg initialized successfully.");

		INFO("FFmpeg version: {}", av_version_info());

		INFO("Core module initialized successfully.");
		return true;
	}
	catch (const std::exception& ex) {
		ERROR("Core initialization failed: {}", ex.what());
		return false;
	}
}

void lighter::Core::cleanup() {
	INFO("Cleaning up FFmpeg resources...");
	avformat_network_deinit();
	INFO("Core module cleanup complete.");
}