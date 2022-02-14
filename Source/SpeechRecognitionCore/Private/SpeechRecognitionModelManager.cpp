#include "SpeechRecognitionModelManager.h"

#include "SpeechRecognitionCoreLog.h"
#include "SpeechRecognitionCoreModule.h"

void FSpeechRecognitionModelManager::RegisterLocale(FString Implementation, FString Locale, FString ModelPath)
{
	TTuple<FString, FString> ImplLocale(Implementation, Locale);
	if (!this->ImplLocaleModelPathMap.Contains(ImplLocale))
	{
		this->ImplLocaleModelPathMap.Add(ImplLocale, ModelPath);
	}
	else
	{
		this->ImplLocaleModelPathMap[ImplLocale] = ModelPath;
	}
}

void FSpeechRecognitionModelManager::UnregisterLocale(FString Implementation, FString Locale)
{
	TTuple<FString, FString> ImplLocale(Implementation, Locale);
	this->ImplLocaleModelPathMap.Remove(ImplLocale);
}

bool FSpeechRecognitionModelManager::LoadModel(FString Implementation, FString Locale, FString ModelPath)
{
	if (!this->PathModelMap.Contains(ModelPath))
	{
		ISpeechRecognitionSubsystemPtr Subsystem = ISpeechRecognitionSubsystem::Get(FName(*Implementation));
		check(Subsystem);

		FSpeechRecognitionModelPtr Model = Subsystem->CreateModel(Locale, ModelPath);
		if (Model->Load())
		{
			this->PathModelMap.Add(ModelPath, Model);
			this->RegisterLocale(Implementation, Locale, ModelPath);
			return true;
		}
		else
		{
			UE_LOG(LogSpeechRecognitionCore, Error, TEXT("Failed to load '%s'. Implementation: '%s'"), *ModelPath, *Implementation);
			return false;
		}
	}

	return true;
}

bool FSpeechRecognitionModelManager::UnloadModel(FString Implementation, FString Locale)
{
	return false;
}

bool FSpeechRecognitionModelManager::UnloadModel(FString ModelPath)
{
	return false;
}

bool FSpeechRecognitionModelManager::UnloadAllModels(FString Implementation)
{
	return false;
}

void FSpeechRecognitionModelManager::UnloadAllModels()
{
}

FSpeechRecognitionModelManager& FSpeechRecognitionModelManager::Get()
{
	FSpeechRecognitionCoreModule& SpeechRecognitionCore = FModuleManager::GetModuleChecked<FSpeechRecognitionCoreModule>("SpeechRecognitionCore");
	return SpeechRecognitionCore.GetModelManager();
}
