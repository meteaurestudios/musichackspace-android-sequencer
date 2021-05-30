//
// Created by Baptiste Le Goff on 11/05/2021.
//

#ifndef MHS_ANDROID_SESSION_2_AUDIOENGINE_H
#define MHS_ANDROID_SESSION_2_AUDIOENGINE_H

#include <Oboe/Oboe.h>
#include <vector>
#include <array>
#include "SineWaveGenerator.h"

class AudioEngine
        : public oboe::AudioStreamDataCallback
        , public oboe::AudioStreamErrorCallback {

public:

    AudioEngine();

    void start();
    void stop();

    void setIsPlaying(bool isPlaying);
    void setTempo(int tempo);
    void setStepIsActive(int step, bool isActive);
    void setStepPitch(int step, int pitch);

    oboe::DataCallbackResult onAudioReady(oboe::AudioStream *oboeStream,
                                          void *audioData,
                                          int32_t numFrames) override;

    void onErrorAfterClose(oboe::AudioStream *oboeStream, oboe::Result error) override;

private:

    /**
     * To be called when the sample rate has been assigned to the stream
     * @param sampleRate
     */
    void setSampleRate(float sampleRate);

    /**
     * To be called when sample rate or tempo change
     */
    void updateSequencePhaseIncrement();

    float pitchToFrequency(int pitch);

private:

    float mSampleRate = 0.f;
    std::shared_ptr<oboe::AudioStream> mStream;

    bool mIsPlaying = false;

    SineWaveGenerator mSineWaveGen;
    std::vector<float> mSineBuffer;

    // Sequencing related
    static constexpr int kSequencerStepsCount = 8;
    double mSequencePhase = 0.f;
    double mSequencePhaseIncrement = 0.f;
    float mTempo = 120;
    int mCurrentStep = 0;
    std::array<bool, kSequencerStepsCount> mIsStepActive;
    std::array<int, kSequencerStepsCount> mStepPitch;
};


#endif //MHS_ANDROID_SESSION_2_AUDIOENGINE_H
