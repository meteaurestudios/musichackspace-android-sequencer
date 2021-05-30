#include <jni.h>
#include <string>
#include <android/log.h>

#include <oboe/Oboe.h>

#include "AudioEngine.h"

AudioEngine engine;

#define TAG "MyNativeTag"
#define LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE, TAG,__VA_ARGS__)

extern "C"
JNIEXPORT void JNICALL
Java_com_example_mhsandroidsession4_MainActivity_startAudio(JNIEnv *env, jobject thiz)
{
    LOGV("Start audio");

    engine.start();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_mhsandroidsession4_MainActivity_stopAudio(JNIEnv *env, jobject thiz)
{
    LOGV("Stop audio");

    engine.stop();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_mhsandroidsession4_MainActivity_setDefaultStreamValues(JNIEnv *env, jobject thiz,
                                                                        jint sampleRate,
                                                                        jint framesPerBurst) {
    oboe::DefaultStreamValues::SampleRate = (int32_t) sampleRate;
    oboe::DefaultStreamValues::FramesPerBurst = (int32_t) framesPerBurst;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_mhsandroidsession4_MainActivity_setEngineIsPlaying(JNIEnv *env, jobject thiz,
                                                                    jboolean isPlaying) {
    engine.setIsPlaying(isPlaying);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_mhsandroidsession4_MainActivity_setEngineTempo(JNIEnv *env, jobject thiz,
                                                                jint tempo) {
    engine.setTempo(tempo);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_mhsandroidsession4_MainActivity_setEngineStepIsActive(JNIEnv *env, jobject thiz,
                                                                       jint step,
                                                                       jboolean is_active) {
    engine.setStepIsActive(step, is_active);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_mhsandroidsession4_MainActivity_setEngineStepPitch(JNIEnv *env, jobject thiz,
                                                                    jint step, jint pitch) {
    engine.setStepPitch(step, pitch);
}