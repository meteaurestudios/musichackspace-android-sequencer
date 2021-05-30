//
// Created by Baptiste Le Goff on 11/05/2021.
//

#ifndef MHS_ANDROID_SESSION_2_AUDIOENGINE_H
#define MHS_ANDROID_SESSION_2_AUDIOENGINE_H

#include <Oboe/Oboe.h>
#include "SineWaveGenerator.h"

class AudioEngine
        : public oboe::AudioStreamDataCallback
        , public oboe::AudioStreamErrorCallback {

public:

    void start();
    void stop();

    void setIsActive(bool active);
    void setSineFrequency(float freq);

    oboe::DataCallbackResult onAudioReady(oboe::AudioStream *oboeStream,
                                          void *audioData,
                                          int32_t numFrames) override;

    void onErrorAfterClose(oboe::AudioStream *oboeStream, oboe::Result error) override;

private:

    std::shared_ptr<oboe::AudioStream> mStream;
    bool mIsActive = false;

    SineWaveGenerator mSineWaveGen;
};


#endif //MHS_ANDROID_SESSION_2_AUDIOENGINE_H
