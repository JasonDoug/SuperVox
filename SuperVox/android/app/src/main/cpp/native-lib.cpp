#include <jni.h>
#include <string>
#include <memory>
#include "AudioEngine.h"

std::unique_ptr<supervox::AudioEngine> gAudioEngine;

extern "C" JNIEXPORT jstring JNICALL
Java_com_supervox_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++ (MainActivity)";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_supervox_audio_AudioModule_getHelloFromNative(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from SuperVox Native Audio Engine!";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_supervox_audio_AudioModule_startEngine(
        JNIEnv* env,
        jobject /* this */) {
    if (!gAudioEngine) {
        gAudioEngine = supervox::AudioEngine::create();
    }
    return gAudioEngine->start();
}

extern "C" JNIEXPORT void JNICALL
Java_com_supervox_audio_AudioModule_stopEngine(
        JNIEnv* env,
        jobject /* this */) {
    if (gAudioEngine) {
        gAudioEngine->stop();
        gAudioEngine.reset();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_supervox_audio_AudioModule_setFocusStrength(
        JNIEnv* env,
        jobject /* this */,
        jfloat strength) {
    if (gAudioEngine) {
        gAudioEngine->setFocusStrength(strength);
    }
}
