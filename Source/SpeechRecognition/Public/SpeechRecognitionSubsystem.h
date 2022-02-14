
#pragma once

#include "SpeechRecognitionCoreModule.h"
#include "Subsystems/EngineSubsystem.h"

#include "SpeechRecognitionSubsystem.generated.h"

USTRUCT()
struct FLocaleEntry
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Implementation;

	UPROPERTY()
	FString Locale;

	UPROPERTY()
	FString Model;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpeechEventDelegate, const TArray<FSpeechResult>&, InResults, class APlayerController*, InSpeaker);

/**
 *
 */
UCLASS(BlueprintType, Config = Engine)
class SPEECHRECOGNITION_API USpeechRecognitionSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	void LoadModel(FString Implementation, FString Locale);
	void LoadAllModels(FString Implementation);
	void LoadAllModels();

	void UnloadModel(FString Implementation, FString Locale);
	void UnloadAllModels(FString Implementation);
	void UnloadAllModels();

	FORCEINLINE FString GetModelPath(FLocaleEntry Entry) const
	{
#ifdef WITH_EDITOR
		return FPaths::Combine(FPaths::ProjectDir(), this->ModelRootDirectory, Entry.Implementation, Entry.Model);	 // Use project directory as the root directory when using the editor
#else
		return FPaths::Combine(this->ModelRootDirectory, Entry.Implementation, Entry.Model);
#endif
	}

	void BroadcastSpeechEvent(const TArray<FSpeechResult>& InResults, class APlayerController* InSpeaker)
	{
		this->OnSpeechEvent.Broadcast(InResults, InSpeaker);
	}

	UFUNCTION(BlueprintCallable)
	class UAudioCapture* CreateMicrophoneCapture();

private:
	void LoadOnStartup();

public:
	UPROPERTY(BlueprintAssignable, Category = "Speech Recognition")
	FOnSpeechEventDelegate OnSpeechEvent;

	UPROPERTY(Config)
	FString CurrentImplementation;

	UPROPERTY(Config)
	FString ModelRootDirectory;

	UPROPERTY(Config)
	TArray<FLocaleEntry> Locales;

	UPROPERTY(Config)
	TArray<FString> LoadOnStartupLocales;

	UPROPERTY()
	class UAudioCapture* AudioCapture;

private:
	FSpeechRecognitionModelManager* ModelManager;
	FRunnableThread* OnlineAsyncTaskThread;
};
