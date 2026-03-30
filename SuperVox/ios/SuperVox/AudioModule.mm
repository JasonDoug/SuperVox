#import "AudioModule.h"
#import <React/RCTLog.h>
#include "AudioEngine.h"
#include <memory>

@implementation AudioModule {
  std::unique_ptr<supervox::AudioEngine> _audioEngine;
}

RCT_EXPORT_MODULE();

RCT_EXPORT_METHOD(getNativeHello:(RCTPromiseResolveBlock)resolve rejecter:(RCTPromiseRejectBlock)reject)
{
  resolve(@"Hello from SuperVox Native Audio Engine (iOS)!");
}

RCT_EXPORT_METHOD(startEngine:(RCTPromiseResolveBlock)resolve rejecter:(RCTPromiseRejectBlock)reject)
{
  if (!_audioEngine) {
    _audioEngine = supervox::AudioEngine::create();
  }
  
  if (_audioEngine->start()) {
    resolve(@YES);
  } else {
    reject(@"ERR_ENGINE_START", @"Failed to start iOS Audio Engine", nil);
  }
}

RCT_EXPORT_METHOD(stopEngine)
{
  if (_audioEngine) {
    _audioEngine->stop();
    _audioEngine.reset();
  }
}

RCT_EXPORT_METHOD(setFocusStrength:(float)strength)
{
  if (_audioEngine) {
    _audioEngine->setFocusStrength(strength);
  }
}

+ (BOOL)requiresMainQueueSetup
{
  return NO;
}

@end
