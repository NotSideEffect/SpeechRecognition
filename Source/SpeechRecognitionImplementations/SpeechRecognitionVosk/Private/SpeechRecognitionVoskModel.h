#pragma once

#include "SpeechRecognitionModel.h"
#include "vosk_api.h"

class FSpeechRecognitionVoskModel : public TSharedFromThis<FSpeechRecognitionVoskModel, ESPMode::ThreadSafe>, public FSpeechRecognitionModel
{
public:
	FSpeechRecognitionVoskModel(const FString& InLocale, const FString& InModelPath) : FSpeechRecognitionModel(InLocale, InModelPath), RawModel(nullptr)
	{
	}

	FORCEINLINE virtual bool Load() override
	{
		this->RawModel = vosk_model_new(TCHAR_TO_UTF8(*this->GetModelPath()));
        return this->RawModel != nullptr;
	}

	FORCEINLINE virtual bool Unload() override
	{
		vosk_model_free(this->RawModel);
		return true;
	}

	FORCEINLINE virtual bool IsLoaded() const override
	{
		return this->RawModel != nullptr;
	}

	FORCEINLINE virtual uint32_t GetExpectedSampleRate() const override
	{
		return 16000;
	}

	FORCEINLINE virtual FName GetImplementation() const override
	{
		return FName(TEXT("Vosk"));
	}

	FORCEINLINE VoskModel* GetVoskModel() const
	{
		return this->RawModel;
	}

private:
	VoskModel* RawModel;
};

typedef TSharedPtr<FSpeechRecognitionVoskModel, ESPMode::ThreadSafe> FSpeechRecognitionVoskModelPtr;