#include "SpeechRecognitionSubmixListener.h"

#include "AudioMixerBlueprintLibrary.h"
#include "DSP/SampleRateConverter.h"

FSpeechRecognitionSubmixListener::FSpeechRecognitionSubmixListener(FSubmixDataEventFunction InOnSubmixData, Audio::FDeviceId InDeviceId, USoundSubmix* InSoundSubmix)
	: OnSubmixData(InOnSubmixData), NumChannelsInSubmix(0), SubmixSampleRate(0), AudioDeviceId(InDeviceId), Submix(InSoundSubmix), bIsRegistered(false)
{
}

FSpeechRecognitionSubmixListener::FSpeechRecognitionSubmixListener() : NumChannelsInSubmix(0), SubmixSampleRate(0), AudioDeviceId(INDEX_NONE), Submix(nullptr), bIsRegistered(false), bListening(false)
{
}

FSpeechRecognitionSubmixListener::FSpeechRecognitionSubmixListener(FSpeechRecognitionSubmixListener&& Other) : FSpeechRecognitionSubmixListener()
{
	UE_LOG(LogTemp, Warning, TEXT("FSpeechRecognitionSubmixListener MOVE"))

	UnregisterFromSubmix();
	Other.UnregisterFromSubmix();

	NumChannelsInSubmix = Other.NumChannelsInSubmix.load();
	Other.NumChannelsInSubmix = 0;

	SubmixSampleRate = Other.SubmixSampleRate.load();
	Other.SubmixSampleRate = 0;

	OnSubmixData = Other.OnSubmixData;

	Submix = Other.Submix;
	Other.Submix = nullptr;

	AudioDeviceId = Other.AudioDeviceId;
	Other.AudioDeviceId = INDEX_NONE;

	bListening = Other.bListening;
	Other.bListening = false;

	RegisterToSubmix();
}

FSpeechRecognitionSubmixListener::~FSpeechRecognitionSubmixListener()
{
	UnregisterFromSubmix();
}

void FSpeechRecognitionSubmixListener::RegisterToSubmix()
{
	UE_LOG(LogTemp, Warning, TEXT("FSpeechRecognitionSubmixListener REGISTER"))

	FAudioDevice* DeviceHandle = FAudioDeviceManager::Get()->GetAudioDeviceRaw(AudioDeviceId);
	ensure(DeviceHandle);

	if (DeviceHandle)
	{
		DeviceHandle->RegisterSubmixBufferListener(this, Submix);
		bIsRegistered = true;

		// RegisterSubmixBufferListener lazily enqueues the registration on the audio thread,
		// so we have to wait for the audio thread to destroy.
		FAudioCommandFence Fence;
		Fence.BeginFence();
		Fence.Wait();
	}
}

void FSpeechRecognitionSubmixListener::UnregisterFromSubmix()
{
	if (bIsRegistered)
	{
		FAudioDevice* DeviceHandle = FAudioDeviceManager::Get()->GetAudioDeviceRaw(AudioDeviceId);
		ensure(DeviceHandle);
		DeviceHandle->UnregisterSubmixBufferListener(this, Submix);

		bIsRegistered = false;

		// UnregisterSubmixBufferListener lazily enqueues the unregistration on the audio thread,
		// so we have to wait for the audio thread to destroy.
		FAudioCommandFence Fence;
		Fence.BeginFence();
		Fence.Wait();
	}
}

float FSpeechRecognitionSubmixListener::GetSampleRate() const
{
	return (float) SubmixSampleRate;
}

int32 FSpeechRecognitionSubmixListener::GetNumChannels() const
{
	return NumChannelsInSubmix;
}

void FSpeechRecognitionSubmixListener::OnNewSubmixBuffer(const USoundSubmix* OwningSubmix, float* AudioData, int32 NumSamples, int32 NumChannels, const int32 SampleRate, double AudioClock)
{
	if (!this->bListening)
	{
		return;
	}
}