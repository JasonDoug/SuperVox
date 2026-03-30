#include <oboe/Oboe.h>
#include "AudioEngine.h"
#include "Processor.h"
#include <memory>
#include <vector>

namespace supervox {

class AndroidAudioEngine : public AudioEngine, public oboe::AudioStreamDataCallback {
public:
    AndroidAudioEngine() {
        gainProcessor_ = std::make_unique<GainProcessor>(1.0f);
    }

    bool start() override {
        oboe::AudioStreamBuilder builder;
        builder.setDirection(oboe::Direction::Input)
                ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
                ->setSharingMode(oboe::SharingMode::Exclusive)
                ->setFormat(oboe::AudioFormat::Float)
                ->setChannelCount(oboe::ChannelCount::Mono)
                ->setDataCallback(this);

        oboe::Result result = builder.openStream(stream_);
        if (result != oboe::Result::OK) return false;

        result = stream_->requestStart();
        return result == oboe::Result::OK;
    }

    void stop() override {
        if (stream_) {
            stream_->requestStop();
            stream_->close();
            stream_.reset();
        }
    }

    void setFocusStrength(float strength) override {
        gainProcessor_->setGain(strength);
    }

    void setLowEQ(float db) override {}
    void setMidEQ(float db) override {}
    void setHighEQ(float db) override {}

    oboe::DataCallbackResult onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) override {
        float *floatData = static_cast<float *>(audioData);
        gainProcessor_->process(floatData, numFrames);
        return oboe::DataCallbackResult::Continue;
    }

private:
    std::shared_ptr<oboe::AudioStream> stream_;
    std::unique_ptr<GainProcessor> gainProcessor_;
};

std::unique_ptr<AudioEngine> AudioEngine::create() {
    return std::make_unique<AndroidAudioEngine>();
}

} // namespace supervox
