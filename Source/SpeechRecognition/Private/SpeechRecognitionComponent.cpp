#include "SpeechRecognitionComponent.h"

#include "AudioMixerBlueprintLibrary.h"
#include "DSP/SampleRateConverter.h"
#include "SpeechRecognitionSubsystem.h"

USpeechRecognitionComponent::USpeechRecognitionComponent()
{
	if (!this->SpeechRecognition.IsInitialized())
	{
		if (this->SpeechRecognition.Initialize("Vosk", "en-us"))
		{
		}
	}

	DeviceCreatedHandle = FAudioDeviceManagerDelegates::OnAudioDeviceCreated.AddUObject(this, &USpeechRecognitionComponent::OnNewDeviceCreated);
	DeviceDestroyedHandle = FAudioDeviceManagerDelegates::OnAudioDeviceDestroyed.AddUObject(this, &USpeechRecognitionComponent::OnDeviceDestroyed);
}

USpeechRecognitionComponent::~USpeechRecognitionComponent()
{
	this->SetAudioSource(nullptr);
	FAudioDeviceManagerDelegates::OnAudioDeviceCreated.Remove(DeviceCreatedHandle);
	FAudioDeviceManagerDelegates::OnAudioDeviceDestroyed.Remove(DeviceDestroyedHandle);
}

void USpeechRecognitionComponent::BeginPlay()
{
	Super::BeginPlay();

	this->SpeechRecognitionSubsystem = GEngine->GetEngineSubsystem<USpeechRecognitionSubsystem>();

	this->OnSpeechRecognitionResultHandle = this->SpeechRecognition.OnSpeechRecognitionResult().AddLambda(
		[this](const TArray<FSpeechResult>& InResults)
		{
			this->SpeechRecognitionSubsystem->BroadcastSpeechEvent(InResults, nullptr);
			// for (const FSpeechResult& Result : InResults)
			// {
			// 	switch (Result.Type)
			// 	{
			// 		case EResultTypes::Partial:
			// 			UE_LOG(LogTemp, Warning, TEXT("PARTIAL: %s"), *Result.GetText());
			// 			break;
			// 		case EResultTypes::Alternative:
			// 			UE_LOG(LogTemp, Warning, TEXT("ALTERNATIVE: %s"), *Result.GetText());
			// 			for (const FSpeechResultWord& Word : Result.Words)
			// 			{
			// 				Word.DebugPrint();
			// 			}
			// 			break;
			// 		case EResultTypes::Final:
			// 			UE_LOG(LogTemp, Warning, TEXT("FINAL: %s"), *Result.GetText());
			// 			for (const FSpeechResultWord& Word : Result.Words)
			// 			{
			// 				Word.DebugPrint();
			// 			}
			// 			break;
			// 		default:
			// 			break;
			// 	}
			//}
		});

	UE_LOG(LogTemp, Error, TEXT("USpeechRecognitionComponent::BeginPlay()"));
	this->OnUpdateSubmix(Submix);
}

bool USpeechRecognitionComponent::SetAudioSource(class UAudioGenerator* InAudioSource)
{
	if (this->AudioSource)
	{
		this->AudioSource->RemoveGeneratorDelegate(this->SourceDelegateHandle);
	}

	this->AudioSource = InAudioSource;

	if (this->AudioSource)
	{
		this->SourceDelegateHandle = this->AudioSource->AddGeneratorDelegate([this](const float* InAudio, int32 NumSamples) { this->OnAudioData(InAudio, NumSamples); });
	}

	return true;
}

void USpeechRecognitionComponent::OnAudioData(const float* InAudio, int32 NumSamples)
{
	FAudioBuffer Buffer;
	Buffer.bInterleaved = true;
	Buffer.Channels = this->AudioSource->GetNumChannels();
	Buffer.SampleRate = this->AudioSource->GetSampleRate();

	TArray<float> FloatBuffer;
	for (int32 FrameIndex = 0; FrameIndex < NumSamples; ++FrameIndex)
	{
		int16 SampleValue = TNumericLimits<int16>::Max() * InAudio[FrameIndex];
		FloatBuffer.Add(SampleValue);
	}

	Buffer.Frames = MakeArrayView(FloatBuffer.GetData(), FloatBuffer.Num());

	this->SpeechRecognition.ProcessAudio(Buffer);
}

void USpeechRecognitionComponent::RegisterToAllAudioDevices()
{
	if (FAudioDeviceManager* DeviceManager = FAudioDeviceManager::Get())
	{
		// Register a new submix listener for every audio device that currently exists.
		DeviceManager->IterateOverAllDevices([&](Audio::FDeviceId DeviceId, FAudioDevice* InDevice) { this->AddSubmixListener(DeviceId); });
	}
}

void USpeechRecognitionComponent::AddSubmixListener(const Audio::FDeviceId InDeviceId)
{
	UE_LOG(LogTemp, Error, TEXT("USpeechRecognitionComponent::AddSubmixListener()"));
	check(!SubmixListeners.Contains(InDeviceId));
	SubmixListeners.Emplace(InDeviceId, new FSpeechRecognitionSubmixListener(
											[this](const USoundSubmix* OwningSubmix, float* AudioData, int32 NumSamples, int32 NumChannels, const int32 SampleRate, double AudioClock)
											{
												if (!this->SpeechRecognition.IsActive())
												{
													return;
												}

												FAudioBuffer Buffer;
												Buffer.bInterleaved = false;
												Buffer.Channels = NumChannels;
												Buffer.SampleRate = SampleRate;
												Buffer.Frames = MakeArrayView(AudioData, NumSamples);

												this->SpeechRecognition.ProcessAudio(Buffer);
											},
											InDeviceId, Submix));
	SubmixListeners[InDeviceId]->RegisterToSubmix();
}

void USpeechRecognitionComponent::RemoveSubmixListener(Audio::FDeviceId InDeviceId)
{
	if (SubmixListeners.Contains(InDeviceId))
	{
		SubmixListeners.Remove(InDeviceId);
	}
}

void USpeechRecognitionComponent::UnregisterFromAllAudioDevices()
{
	if (FAudioDeviceManager* DeviceManager = FAudioDeviceManager::Get())
	{
		// Register a new submix listener for every audio device that currently exists.
		DeviceManager->IterateOverAllDevices([&](Audio::FDeviceId DeviceId, FAudioDevice* InDevice) { this->RemoveSubmixListener(DeviceId); });
	}
}

void USpeechRecognitionComponent::OnUpdateSubmix(USoundSubmix* InSubmix)
{
	if (this->bIsSubmixListenerRegistered)
	{
		this->UnregisterFromAllAudioDevices();
		this->bIsSubmixListenerRegistered = false;
	}

	this->Submix = InSubmix;

	if (!InSubmix)
	{
		return;
	}

	this->RegisterToAllAudioDevices();
	this->bIsSubmixListenerRegistered = true;
}

void USpeechRecognitionComponent::OnNewDeviceCreated(Audio::FDeviceId InDeviceId)
{
	if (this->bIsSubmixListenerRegistered)
	{
		this->AddSubmixListener(InDeviceId);
	}
}

void USpeechRecognitionComponent::OnDeviceDestroyed(Audio::FDeviceId InDeviceId)
{
	if (this->bIsSubmixListenerRegistered)
	{
		this->RemoveSubmixListener(InDeviceId);
	}
}

#if WITH_EDITOR
void USpeechRecognitionComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	static FName SubmixFName = GET_MEMBER_NAME_CHECKED(USpeechRecognitionComponent, Submix);

	// Regenerate on save any compressed sound formats or if analysis needs to be re-done
	if (FProperty* PropertyThatChanged = PropertyChangedEvent.Property)
	{
		const FName& Name = PropertyThatChanged->GetFName();
		if (Name == SubmixFName)
		{
			this->OnUpdateSubmix(Submix);
		}
	}
}
#endif	  // WITH_EDITOR

void USpeechRecognitionComponent::PostInitProperties()
{
	Super::PostInitProperties();

	this->OnUpdateSubmix(Submix);
}

void USpeechRecognitionComponent::BeginDestroy()
{
	if (this->bIsSubmixListenerRegistered)
	{
		UnregisterFromAllAudioDevices();
	}

	Super::BeginDestroy();
}

void USpeechRecognitionComponent::PostLoad()
{
	Super::PostLoad();
}
