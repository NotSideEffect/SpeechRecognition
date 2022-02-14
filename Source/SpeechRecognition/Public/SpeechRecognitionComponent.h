#pragma once

#include "Components/ActorComponent.h"
#include "Generators/AudioGenerator.h"
#include "SpeechRecognitionSession.h"
#include "SpeechRecognitionSubmixListener.h"

#include "SpeechRecognitionComponent.generated.h"

UCLASS(BlueprintType, Blueprintable)
class SPEECHRECOGNITION_API USpeechRecognitionComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	USpeechRecognitionComponent();
	~USpeechRecognitionComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	bool SetAudioSource(class UAudioGenerator* InAudioSource);

	UFUNCTION(BlueprintCallable)
	void Start()
	{
		for (TPair<Audio::FDeviceId, TUniquePtr<FSpeechRecognitionSubmixListener>>& Listener : this->SubmixListeners)
		{
			Listener.Value->StartListening();
		}
		this->SpeechRecognition.StartListening();
	}

	UFUNCTION(BlueprintCallable)
	void Stop()
	{
		for (TPair<Audio::FDeviceId, TUniquePtr<FSpeechRecognitionSubmixListener>>& Listener : this->SubmixListeners)
		{
			Listener.Value->StopListening();
		}
		this->SpeechRecognition.StopListening();
	}

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif	  // WITH_EDITOR

	virtual void PostInitProperties() override;
	virtual void BeginDestroy() override;
	virtual void PostLoad() override;

	/** Called when the Submix property changes. */
	virtual void OnUpdateSubmix(USoundSubmix* InSubmix);

protected:
	virtual void OnAudioData(const float* InAudio, int32 NumSamples);

	void AddSubmixListener(const Audio::FDeviceId InDeviceId);
	void RemoveSubmixListener(const Audio::FDeviceId InDeviceId);

	void RegisterToAllAudioDevices();
	void UnregisterFromAllAudioDevices();

	void OnNewDeviceCreated(Audio::FDeviceId InDeviceId);
	void OnDeviceDestroyed(Audio::FDeviceId InDeviceId);

private:
	FSpeechRecognition SpeechRecognition;
	class UAudioGenerator* AudioSource;
	FAudioGeneratorHandle SourceDelegateHandle;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundSubmix* Submix;

	// Map of audio devices to submix listeners. Needed for editor where multiple audio devices
	// may exist.
	TMap<Audio::FDeviceId, TUniquePtr<FSpeechRecognitionSubmixListener>> SubmixListeners;

	bool bIsSubmixListenerRegistered;
	
	FDelegateHandle DeviceCreatedHandle;
	FDelegateHandle DeviceDestroyedHandle;

	class USpeechRecognitionSubsystem* SpeechRecognitionSubsystem;

	FDelegateHandle OnSpeechRecognitionResultHandle;
};