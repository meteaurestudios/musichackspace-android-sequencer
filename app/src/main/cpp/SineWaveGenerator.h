//
// Created by Baptiste Le Goff on 10/05/2021.
//

#ifndef MY_APPLICATION_SINEWAVEGENERATOR_H
#define MY_APPLICATION_SINEWAVEGENERATOR_H

#include <cmath>

class SineWaveGenerator
{
public:

    void setSampleRate(float sampleRate)
    {
        mSampleRate = sampleRate;
        UpdateIncrement(mFrequency);
    }

    void setFrequency(float freq)
    {
        mFrequency = freq;
        UpdateIncrement(mFrequency);
    }

    void process(float* buffer, uint32_t channels, uint32_t frames)
    {
        // We only allow 1 or 2 channels to be processed
        channels = std::max(1, std::min(2, (int)channels));

        // Write same sine wave value to each channel
        for(uint32_t f=0; f < frames; ++f) {

            float value = sin(mPhase) * 0.3f;

            for(uint32_t c=0; c < channels; ++c) {
                buffer[f*channels + c] = value;
            }

            mPhase += mPhaseIncrement;

            while(mPhase >= kTwoPi) {
                mPhase -= kTwoPi;
            }
        }
    }

private:

    inline void UpdateIncrement(float frequency)
    {
        mPhaseIncrement = frequency * kTwoPi / mSampleRate;
    }

private:

    float mFrequency = 440.f;
    float mPhase = 0.f;
    float mSampleRate = 44100.f;
    float mPhaseIncrement = 0.f;
    static float constexpr kTwoPi = M_PI * 2;
};

#endif //MY_APPLICATION_SINEWAVEGENERATOR_H
