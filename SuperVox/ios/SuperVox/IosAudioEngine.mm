#import <AVFoundation/AVFoundation.h>
#include "AudioEngine.h"
#include "Processor.h"
#include <memory>

namespace supervox {

class IosAudioEngine : public AudioEngine {
public:
    IosAudioEngine() {
        gainProcessor_ = std::make_unique<GainProcessor>(1.0f);
        engine_ = [[AVAudioEngine alloc] init];
    }

    ~IosAudioEngine() {
        stop();
    }

    bool start() override {
        AVAudioInputNode *inputNode = [engine_ inputNode];
        AVAudioFormat *inputFormat = [inputNode inputFormatForBus:0];

        [inputNode installTapOnBus:0 bufferSize:1024 format:inputFormat block:^(AVAudioPCMBuffer * _Nonnull buffer, AVAudioTime * _Nonnull when) {
            float * const *data = [buffer floatChannelData];
            int numSamples = buffer.frameLength;
            // Process the buffer with our shared C++ logic
            for (int i = 0; i < buffer.format.channelCount; ++i) {
                gainProcessor_->process(data[i], numSamples);
            }
        }];

        NSError *error = nil;
        [engine_ startAndReturnError:&error];
        if (error) {
            NSLog(@"Audio Engine Error: %@", error.localizedDescription);
            return false;
        }

        return true;
    }

    void stop() override {
        [[engine_ inputNode] removeTapOnBus:0];
        if ([engine_ isRunning]) {
            [engine_ stop];
        }
    }

    void setFocusStrength(float strength) override {
        gainProcessor_->setGain(strength);
    }

    void setLowEQ(float db) override {}
    void setMidEQ(float db) override {}
    void setHighEQ(float db) override {}

private:
    AVAudioEngine *engine_;
    std::unique_ptr<GainProcessor> gainProcessor_;
};

std::unique_ptr<AudioEngine> AudioEngine::create() {
    return std::make_unique<IosAudioEngine>();
}

} // namespace supervox
