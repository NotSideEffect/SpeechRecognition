#pragma once

#include "SpeechRecognitionModel.h"

class SPEECHRECOGNITIONCORE_API FSpeechRecognitionModelManager
{
public:
	bool LoadModel(FString Implementation, FString Locale, FString ModelPath);

	bool UnloadModel(FString Implementation, FString Locale);
	bool UnloadModel(FString ModelPath);
	bool UnloadAllModels(FString Implementation);
	void UnloadAllModels();

	void RegisterLocale(FString Implementation, FString Locale, FString ModelPath);
	void UnregisterLocale(FString Implementation, FString Locale);

	FORCEINLINE bool IsRegistered(FString Implementation, FString Locale) const
	{
		return ImplLocaleModelPathMap.Contains(TTuple<FString, FString>(Implementation, Locale));
	}

	FORCEINLINE bool IsLoaded(FString Implementation, FString Locale) const
	{
		TTuple<FString, FString> ImplLocale(Implementation, Locale);
		return ImplLocaleModelPathMap.Contains(ImplLocale) && this->PathModelMap.Contains(ImplLocaleModelPathMap[ImplLocale]);
	}

	template <class T>
	TSharedPtr<T, ESPMode::ThreadSafe> GetModel(FString Implementation, FString Locale)
	{
		TTuple<FString, FString> ImplLocale(Implementation, Locale);
		if (ImplLocaleModelPathMap.Contains(ImplLocale))
		{
			const FString& ModelPath = ImplLocaleModelPathMap[ImplLocale];
			if (PathModelMap.Contains(ModelPath))
			{
				return StaticCastSharedPtr<T>(PathModelMap[ModelPath]);
			}
		}

		return TSharedPtr<T, ESPMode::ThreadSafe>();
	}

	static FSpeechRecognitionModelManager& Get();

private:
	TMap<FString, FSpeechRecognitionModelPtr> PathModelMap;
	TMap<TTuple<FString, FString>, FString> ImplLocaleModelPathMap;
};
