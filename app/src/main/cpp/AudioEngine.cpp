//
// Created by Baptiste Le Goff on 11/05/2021.
//

#include "AudioEngine.h"

#include <android/log.h>

#define TAG "MyNativeTag"
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, TAG,__VA_ARGS__)
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, TAG,__VA_ARGS__)

void AudioEngine::start()
{
    oboe::AudioStreamBuilder builder;

    builder.setFormat(oboe::AudioFormat::Float);
    builder.setChannelCount(2);

    builder.setSharingMode(oboe::SharingMode::Exclusive);
    builder.setPerformanceMode(oboe::PerformanceMode::LowLatency);
    builder.setDataCallback(this);
    builder.setErrorCallback(this);

    oboe::Result result = builder.openStream(mStream);

    if(result != oboe::Result::OK) {
        LOGE("Error opening the audio stream");
        return;
    }

    mSineWaveGen.setSampleRate(mStream->getSampleRate());

    result = mStream->requestStart();

    if(result != oboe::Result::OK) {
        LOGE("Error starting the audio stream");
    }
}

void AudioEngine::stop()
{
    if(mStream) {
        mStream->stop();
        mStream->close();
        mStream.reset();
    }
}

oboe::DataCallbackResult
AudioEngine::onAudioReady(oboe::AudioStream *oboeStream, void *audioData, int32_t numFrames)
{
    // Fill buffer with silence
    std::memset(audioData, 0, numFrames*mStream->getChannelCount()*sizeof(float));

    if(mIsActive) {
        auto buffer = static_cast<float *>(audioData);
        int channels = mStream->getChannelCount();
        mSineWaveGen.process(buffer, channels, numFrames);
    }

    return oboe::DataCallbackResult::Continue;
}

void AudioEngine::onErrorAfterClose(oboe::AudioStream *oboeStream, oboe::Result error)
{
    if(error == oboe::Result::ErrorDisconnected) {
        LOGI("Restarting AudioStream");
        start();
    }

    LOGE("Error was %s", oboe::convertToText(error));
}

void AudioEngine::setIsActive(bool active) {
    mIsActive = active;
}

void AudioEngine::setSineFrequency(float freq) {
    mSineWaveGen.setFrequency(freq);
}
