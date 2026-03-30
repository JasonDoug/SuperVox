package com.supervox.audio

import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.bridge.ReactContextBaseJavaModule
import com.facebook.react.bridge.ReactMethod
import com.facebook.react.bridge.Promise

class AudioModule(reactContext: ReactApplicationContext) : ReactContextBaseJavaModule(reactContext) {

    override fun getName(): String {
        return "AudioModule"
    }

    @ReactMethod
    fun getNativeHello(promise: Promise) {
        try {
            promise.resolve(getHelloFromNative())
        } catch (e: Exception) {
            promise.reject("ERR_NATIVE", e.message)
        }
    }

    @ReactMethod
    fun startEngine(promise: Promise) {
        try {
            val success = startEngine()
            promise.resolve(success)
        } catch (e: Exception) {
            promise.reject("ERR_ENGINE_START", e.message)
        }
    }

    @ReactMethod
    fun stopEngine() {
        stopEngine()
    }

    @ReactMethod
    fun setFocusStrength(strength: Float) {
        setFocusStrength(strength)
    }

    private external fun getHelloFromNative(): String
    private external fun startEngine(): Boolean
    private external fun stopEngine()
    private external fun setFocusStrength(strength: Float)
