#include <oboe/Oboe.h>
#include "AudioEngine.h"
#include "Processor.h"
#include "RingBuffer.h"
#include "MLEngine.h"
#include "TFLiteMLEngine.h"
#include <memory>
#include <vector>

namespace supervox {

class AndroidAudioEngine : public AudioEngine, public oboe::AudioStreamDataCallback {
public:
    AndroidAudioEngine() {
        gainProcessor_ = std::make_unique<GainProcessor>(1.0f);
        mlEngine_ = std::make_unique<TFLiteMLEngine>();
        
        int frameSize = mlEngine_->getExpectedFrameSize();
        mlInputBuffer_.resize(frameSize);
        mlOutputBuffer_.resize(frameSize);
        
        // Ring buffers for 4096 samples (approx 85ms @ 48kHz)
        inputRingBuffer_ = std::make_unique<RingBuffer>(4096);
        outputRingBuffer_ = std::make_unique<RingBuffer>(4096);
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
        
        // 1. Write incoming mic data to input ring buffer
        inputRingBuffer_->write(floatData, numFrames);
        
        // 2. If we have enough for an ML frame, process it
        int frameSize = mlEngine_->getExpectedFrameSize();
        while (inputRingBuffer_->availableToRead() >= frameSize) {
            inputRingBuffer_->read(mlInputBuffer_.data(), frameSize);
            
            // ML Inference
            mlEngine_->processFrame(mlInputBuffer_.data(), mlOutputBuffer_.data());
            
            // DSP Post-processing
            gainProcessor_->process(mlOutputBuffer_.data(), frameSize);
            
            // Write to output ring buffer
            outputRingBuffer_->write(mlOutputBuffer_.data(), frameSize);
        }
        
        // 3. Read processed data into the playback buffer
        int samplesRead = outputRingBuffer_->read(floatData, numFrames);
        
        // Zero out any samples we couldn't fulfill to avoid glitching
        if (samplesRead < numFrames) {
            std::memset(floatData + samplesRead, 0, (numFrames - samplesRead) * sizeof(float));
        }

        return oboe::DataCallbackResult::Continue;
    }

private:
    std::shared_ptr<oboe::AudioStream> stream_;
    std::unique_ptr<GainProcessor> gainProcessor_;
    std::unique_ptr<MLEngine> mlEngine_;
    
    std::unique_ptr<RingBuffer> inputRingBuffer_;
    std::unique_ptr<RingBuffer> outputRingBuffer_;
    
    std::vector<float> mlInputBuffer_;
    std::vector<float> mlOutputBuffer_;
};

std::unique_ptr<AudioEngine> AudioEngine::create() {
    return std::make_unique<AndroidAudioEngine>();
}

} // namespace supervox
