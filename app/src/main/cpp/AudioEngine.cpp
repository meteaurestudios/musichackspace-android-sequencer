//
// Created by Baptiste Le Goff on 11/05/2021.
//

#include "AudioEngine.h"

#include <android/log.h>

#define TAG "MyNativeTag"
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, TAG,__VA_ARGS__)
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, TAG,__VA_ARGS__)

// -------------------------------------------------------------------------------------------------
void AudioEngine::start()
{
    oboe::AudioStreamBuilder builder;

    builder.setFormat(oboe::AudioFormat::Float);
    builder.setChannelCount(1);

    builder.setSharingMode(oboe::SharingMode::Exclusive);
    builder.setPerformanceMode(oboe::PerformanceMode::LowLatency);
    builder.setDataCallback(this);
    builder.setErrorCallback(this);

    oboe::Result result = builder.openStream(mStream);

    if(result != oboe::Result::OK) {
        LOGE("Error opening the audio stream");
        return;
    }

    // Initialization
    setSampleRate(mStream->getSampleRate());
    mEnvelopeBuffer.resize(mStream->getBufferCapacityInFrames());
    mSineBuffer.resize(mStream->getBufferCapacityInFrames());

    result = mStream->requestStart();

    if(result != oboe::Result::OK) {
        LOGE("Error starting the audio stream");
    }
}

// -------------------------------------------------------------------------------------------------
void AudioEngine::stop()
{
    if(mStream) {
        mStream->stop();
        mStream->close();
        mStream.reset();
    }
}

// -------------------------------------------------------------------------------------------------
void AudioEngine::setSampleRate(float sampleRate)
{
    mSampleRate = sampleRate;
    mSineWaveGen.setSampleRate(sampleRate);
    UpdateSequencePhaseIncrement();
}

// -------------------------------------------------------------------------------------------------
void AudioEngine::UpdateSequencePhaseIncrement()
{
    const double samples_per_step = (60.0/mTempo) * mSampleRate / 4.f; // 1/16 bar
    mSequencePhaseIncrement = 1.0/samples_per_step;
}

// -------------------------------------------------------------------------------------------------
oboe::DataCallbackResult
AudioEngine::onAudioReady(oboe::AudioStream *oboeStream, void *audioData, int32_t numFrames)
{
    // Fill buffer with silence
    std::memset(audioData, 0, numFrames*mStream->getChannelCount()*sizeof(float));

    auto buffer = static_cast<float *>(audioData);
    int channels = mStream->getChannelCount();

    // Sine wave -----

    mSineWaveGen.process(mSineBuffer.data(), channels, numFrames);

    // Sequencer -----

    for(uint32_t i=0; i < numFrames; ++i) {

        // Step change
        if(mSequencePhase >= 1.0) {

            // Increase step counter
            mCurrentStep = (mCurrentStep+1) % (kSequencerStepsCount);
            mSequencePhase = fmod(mSequencePhase, 1.0);
        }

        const float envelope_level = (float) std::max(1.0 - mSequencePhase, 0.0);
        buffer[i] = envelope_level * mSineBuffer[i];

        mSequencePhase += mSequencePhaseIncrement;
    }

    return oboe::DataCallbackResult::Continue;
}

// -------------------------------------------------------------------------------------------------
void AudioEngine::onErrorAfterClose(oboe::AudioStream *oboeStream, oboe::Result error)
{
    if(error == oboe::Result::ErrorDisconnected) {
        LOGI("Restarting AudioStream");
        start();
    }

    LOGE("Error was %s", oboe::convertToText(error));
}

// -------------------------------------------------------------------------------------------------
void AudioEngine::setIsActive(bool active)
{
    mIsActive = active;
}

// -------------------------------------------------------------------------------------------------
void AudioEngine::setSineFrequency(float freq)
{
    mSineWaveGen.setFrequency(freq);
}
