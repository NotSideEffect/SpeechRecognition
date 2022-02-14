
#include "SpeechRecognitionSubsystem.h"

#include "AudioCapture.h"
#include "Misc/ScopedSlowTask.h"
#include "SpeechRecognitionCoreSubsystem.h"
#include "SpeechRecognitionModelManager.h"
#include "SpeechRecognitionSession.h"

#define LOCTEXT_NAMESPACE "SpeechRecognition"

void USpeechRecognitionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	this->ModelManager = &FSpeechRecognitionModelManager::Get();

	// Register all cultures defined in the config
	for (const FLocaleEntry& LocaleEntry : this->Locales)
	{
		this->ModelManager->RegisterLocale(LocaleEntry.Implementation, LocaleEntry.Locale, this->GetModelPath(LocaleEntry));
	}

	this->LoadOnStartup();
}

void USpeechRecognitionSubsystem::Deinitialize()
{
	this->UnloadAllModels();
}

void USpeechRecognitionSubsystem::LoadModel(FString Implementation, FString Locale)
{
	GWarn->BeginSlowTask(LOCTEXT("LoadingSpeechModel", "Loading Speech Model..."), true);

	for (const FLocaleEntry& LocaleEntry : this->Locales)
	{
		UE_LOG(LogTemp, Warning, TEXT("Impl: %s, Locale: %s, Model: %s"), *LocaleEntry.Implementation, *LocaleEntry.Locale, *LocaleEntry.Model);
		if (LocaleEntry.Implementation == Implementation && LocaleEntry.Locale == Locale)
		{
			GWarn->StatusForceUpdate(0, 1, FText::Format(LOCTEXT("LoadingSpeechModelLocale", "Loading Speech Model '{0}' for '{1}'..."), FText::FromString(LocaleEntry.Locale), FText::FromString(LocaleEntry.Implementation)));
			this->ModelManager->LoadModel(LocaleEntry.Implementation, LocaleEntry.Locale, this->GetModelPath(LocaleEntry));
			break;
		}
	}

	GWarn->EndSlowTask();
}

void USpeechRecognitionSubsystem::LoadAllModels(FString Implementation)
{
	GWarn->BeginSlowTask(LOCTEXT("LoadingSpeechModels", "Loading Speech Models..."), true);

	for (const FLocaleEntry& LocaleEntry : this->Locales)
	{
		UE_LOG(LogTemp, Warning, TEXT("Impl: %s, Locale: %s, Model: %s"), *LocaleEntry.Implementation, *LocaleEntry.Locale, *LocaleEntry.Model);
		if (LocaleEntry.Implementation == Implementation)
		{
			GWarn->StatusForceUpdate(0, 0, FText::Format(LOCTEXT("LoadingSpeechModelLocale", "Loading Speech Model '{0}' for '{1}'..."), FText::FromString(LocaleEntry.Locale), FText::FromString(LocaleEntry.Implementation)));
			this->ModelManager->LoadModel(LocaleEntry.Implementation, LocaleEntry.Locale, this->GetModelPath(LocaleEntry));
		}
	}

	GWarn->EndSlowTask();
}

void USpeechRecognitionSubsystem::LoadAllModels()
{
	GWarn->BeginSlowTask(LOCTEXT("LoadingSpeechModels", "Loading Speech Models..."), true);

	int Progress = 0;
	int TotalProgress = this->Locales.Num();

	for (const FLocaleEntry& LocaleEntry : this->Locales)
	{
		UE_LOG(LogTemp, Warning, TEXT("Impl: %s, Locale: %s, Model: %s"), *LocaleEntry.Implementation, *LocaleEntry.Locale, *LocaleEntry.Model);
		GWarn->StatusForceUpdate(Progress++, TotalProgress, FText::Format(LOCTEXT("LoadingSpeechModelLocale", "Loading Speech Model '{0}' for '{1}'..."), FText::FromString(LocaleEntry.Locale), FText::FromString(LocaleEntry.Implementation)));
		this->ModelManager->LoadModel(LocaleEntry.Implementation, LocaleEntry.Locale, this->GetModelPath(LocaleEntry));
	}

	GWarn->EndSlowTask();
}

void USpeechRecognitionSubsystem::UnloadModel(FString Implementation, FString Locale)
{
	this->ModelManager->UnloadModel(Implementation, Locale);
}

void USpeechRecognitionSubsystem::UnloadAllModels(FString Implementation)
{
	this->ModelManager->UnloadAllModels(Implementation);
}

void USpeechRecognitionSubsystem::UnloadAllModels()
{
	this->ModelManager->UnloadAllModels();
}

void USpeechRecognitionSubsystem::LoadOnStartup()
{
	int Progress = 0;
	int TotalProgress = this->LoadOnStartupLocales.Num();

	GWarn->BeginSlowTask(LOCTEXT("LoadingSpeechModels", "Loading Speech Models..."), true);
	GWarn->StatusForceUpdate(Progress, TotalProgress, LOCTEXT("LoadingSpeechModels", "Loading Speech Models..."));

	UE_LOG(LogTemp, Warning, TEXT("LoadOnStartup: %d"), LoadOnStartupLocales.Num());
	for (const FLocaleEntry& LocaleEntry : this->Locales)
	{
		UE_LOG(LogTemp, Warning, TEXT("Impl: %s, Locale: %s, Model: %s"), *LocaleEntry.Implementation, *LocaleEntry.Locale, *LocaleEntry.Model);
		if (LocaleEntry.Implementation == CurrentImplementation && this->LoadOnStartupLocales.Contains(LocaleEntry.Locale))
		{
			GWarn->StatusForceUpdate(Progress++, TotalProgress, FText::Format(LOCTEXT("LoadingSpeechModelLocale", "Loading Speech Model '{0}' for '{1}'..."), FText::FromString(LocaleEntry.Locale), FText::FromString(LocaleEntry.Implementation)));
			this->ModelManager->LoadModel(LocaleEntry.Implementation, LocaleEntry.Locale, this->GetModelPath(LocaleEntry));
		}
	}

	GWarn->EndSlowTask();
}

class UAudioCapture* USpeechRecognitionSubsystem::CreateMicrophoneCapture()
{
	if (this->AudioCapture)
	{
		return this->AudioCapture;
	}

	this->AudioCapture = NewObject<UAudioCapture>(this);
	this->AudioCapture->OpenDefaultAudioStream();

	return this->AudioCapture;
}

#undef LOCTEXT_NAMESPACE