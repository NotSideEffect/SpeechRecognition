#pragma once

#include "SpeechResult.h"
#include "SpeechTypes.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSpeechRecognitionAudioDelegate, const FAudioBuffer&, AudioBuffer);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSpeechRecognitionSampleRateChangedDelegate, uint32_t, SampleRate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSpeechRecognitionNumberOfChannelsChangedDelegate, uint32_t, NumberOfChannels);

class ISpeechRecognitionAudioSource
{
    virtual uint32_t GetSampleRate() const = 0;
    virtual uint32_t GetNumberOfChannels() const = 0; 
    
    virtual FOnSpeechRecognitionAudioDelegate OnSpeechRecognitionAudio();
    virtual FOnSpeechRecognitionSampleRateChangedDelegate OnSpeechRecognitionSampleRateChanged();
    virtual FOnSpeechRecognitionNumberOfChannelsChangedDelegate OnSpeechRecognitionNumberOfChannelsChanged();
};

typedef TSharedPtr<ISpeechRecognitionAudioSource, ESPMode::ThreadSafe> ISpeechRecognitionAudioSourcePtr;