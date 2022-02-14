#pragma once

#include "AudioDevice.h"
#include "AudioDeviceManager.h"
#include "Sound/SoundSubmix.h"

typedef TFunction<void(const USoundSubmix* OwningSubmix, float* AudioData, int32 NumSamples, int32 NumChannels, const int32 SampleRate, double AudioClock)> FSubmixDataEventFunction;

class SPEECHRECOGNITIONCORE_API FSpeechRecognitionSubmixListener : public ISubmixBufferListener
{
public:
	FSpeechRecognitionSubmixListener(FSubmixDataEventFunction InOnSubmixData, Audio::FDeviceId InDeviceId, USoundSubmix* InSoundSubmix);
	FSpeechRecognitionSubmixListener(FSpeechRecognitionSubmixListener&& Other);
	virtual ~FSpeechRecognitionSubmixListener();

	// Begin ISubmixBufferListener overrides
	virtual void OnNewSubmixBuffer(const USoundSubmix* OwningSubmix, float* AudioData, int32 NumSamples, int32 NumChannels, const int32 SampleRate, double AudioClock) override;
	// End ISubmixBufferListener overrides

	void RegisterToSubmix();

	/** Returns the current sample rate of the current submix. */
	float GetSampleRate() const;

	/** Returns the number of channels of the current submix. */
	int32 GetNumChannels() const;

	bool IsListening() const
	{
		return this->bListening;
	}

	void StartListening()
	{
		this->bListening = true;
	}

	void StopListening()
	{
		this->bListening = false;
	}

private:
	FSpeechRecognitionSubmixListener();

	void UnregisterFromSubmix();

	FSubmixDataEventFunction OnSubmixData;

	std::atomic<int32> NumChannelsInSubmix;
	std::atomic<int32> SubmixSampleRate;

	Audio::FDeviceId AudioDeviceId;
	USoundSubmix* Submix;
	bool bIsRegistered;

	bool bListening = false;

	TArray<float> OutputBuffer;
};