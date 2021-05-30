//
// Created by Baptiste Le Goff on 11/05/2021.
//

#ifndef MHS_ANDROID_SESSION_2_AUDIOENGINE_H
#define MHS_ANDROID_SESSION_2_AUDIOENGINE_H

#include <Oboe/Oboe.h>
#include <vector>
#include "SineWaveGenerator.h"

class AudioEngine
        : public oboe::AudioStreamDataCallback
        , public oboe::AudioStreamErrorCallback {

public:

    void start();
    void stop();

    void setIsPlaying(bool isPlaying);
    void setTempo(int tempo);

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
    void UpdateSequencePhaseIncrement();

private:

    std::shared_ptr<oboe::AudioStream> mStream;
    bool mIsPlaying = false;

    SineWaveGenerator mSineWaveGen;

    float mSampleRate = 0.f;

    // Sequencing related
    static constexpr int kSequencerStepsCount = 8;
    double mSequencePhase = 0.f;
    double mSequencePhaseIncrement = 0.f;
    float mTempo = 120;
    int mCurrentStep = 0;
    std::vector<float> mEnvelopeBuffer;
    std::vector<float> mSineBuffer;

};


#endif //MHS_ANDROID_SESSION_2_AUDIOENGINE_H
