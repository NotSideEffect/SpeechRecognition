#pragma once

#include "SpeechRecognitionModel.h"

class SPEECHRECOGNITIONCORE_API FSpeechRecognitionModel : public TSharedFromThis<FSpeechRecognitionModel, ESPMode::ThreadSafe>
{
protected:
	FSpeechRecognitionModel(const FString& InLocale, const FString& InModelPath) : Locale(InLocale), ModelPath(InModelPath)
	{
	}

public:
	virtual ~FSpeechRecognitionModel()
	{
	}

	virtual bool Load() = 0;
	virtual bool Unload() = 0;
	virtual bool IsLoaded() const = 0;

	virtual uint32_t GetExpectedSampleRate() const = 0;

	virtual FName GetImplementation() const = 0;

	FORCEINLINE FString GetLocale() const
	{
		return this->Locale;
	}

	FORCEINLINE FString GetModelPath() const
	{
		return this->ModelPath;
	}

private:
	FString Locale;
	FString ModelPath;
};

typedef TSharedPtr<FSpeechRecognitionModel, ESPMode::ThreadSafe> FSpeechRecognitionModelPtr;
