#ifndef AUDIO_PLAYER
#define AUDIO_PLAYER

#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
}

namespace lighter {
    class AudioPlayer {
    public:
        AudioPlayer();
        ~AudioPlayer();

        bool init(AVFormatContext* formatCtx);
        void play();
        void pause();
        void stop();
        void setVolume(int volume);

    private:
        // Audio Decoding Context
        AVCodecContext* codecCtx = nullptr;
        SwrContext* swrCtx = nullptr;
        int audioStreamIndex = -1;

        // Playback State
        bool isPlaying = false;

        // Threads
        std::thread decodeThread;

        // Synchronization
        std::mutex audioMutex;
        std::condition_variable audioCv;

        // Helper Methods
        void decodeAudio();
    };
}

#endif // !AUDIO_PLAYER
