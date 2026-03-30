#import <AVFoundation/AVFoundation.h>
#include "AudioEngine.h"
#include "Processor.h"
#include "RingBuffer.h"
#include "MLEngine.h"
#import "CoreMLEngine.h"
#include <memory>
#include <vector>

namespace supervox {

class IosAudioEngine : public AudioEngine {
public:
    IosAudioEngine() {
        gainProcessor_ = std::make_unique<GainProcessor>(1.0f);
        mlEngine_ = std::make_unique<CoreMLEngine>();
        
        int frameSize = mlEngine_->getExpectedFrameSize();
        mlInputBuffer_.resize(frameSize);
        mlOutputBuffer_.resize(frameSize);
        
        // Ring buffers for 4096 samples (approx 85ms @ 48kHz)
        inputRingBuffer_ = std::make_unique<RingBuffer>(4096);
        outputRingBuffer_ = std::make_unique<RingBuffer>(4096);
        
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
            
            // 1. Write incoming mic data to input ring buffer (assuming mono for now)
            inputRingBuffer_->write(data[0], numSamples);
            
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
            
            // Note: AVAudioEngine Tap is capture-only. For real-time playback, 
            // you would typically route outputRingBuffer to the mainMixer output node.
            // For MVP demonstration, we are capturing and processing here.
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
    std::unique_ptr<MLEngine> mlEngine_;
    
    std::unique_ptr<RingBuffer> inputRingBuffer_;
    std::unique_ptr<RingBuffer> outputRingBuffer_;
    
    std::vector<float> mlInputBuffer_;
    std::vector<float> mlOutputBuffer_;
};

std::unique_ptr<AudioEngine> AudioEngine::create() {
    return std::make_unique<IosAudioEngine>();
}

} // namespace supervox
