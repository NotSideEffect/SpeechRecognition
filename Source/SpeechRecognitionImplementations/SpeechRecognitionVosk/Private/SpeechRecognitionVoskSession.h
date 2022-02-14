#pragma once

#include "SpeechRecognitionModelManager.h"
#include "SpeechRecognitionSessionInterface.h"
#include "VoskRecognizer.h"

class FSpeechRecognitionVoskSession : public ISpeechRecognitionSession
{
public:
	FSpeechRecognitionVoskSession();
	virtual ~FSpeechRecognitionVoskSession();

	virtual bool Initialize(FString InLocale, const TSet<FString>& InVocabulary = TSet<FString>()) override;
	
	FORCEINLINE virtual bool IsActive() const override
	{
		return this->bActive;
	}

	FORCEINLINE virtual bool IsInitialized() const override
	{
		return this->bInitialized;
	}

	FORCEINLINE virtual uint32_t GetExpectedSampleRate() const override
	{
		return 16000;
	}

	virtual bool ProcessAudio(const FAudioBuffer& Buffer) override;
	virtual bool StartListening() override;
	virtual void StopListening() override;

	virtual bool AddWord(FString InWord) override;
	virtual bool RemoveWord(FString InWord) override;
	virtual bool ContainsWord(FString InWord) const override;

	virtual bool SupportsLocale(FString InLocale) const override;
	virtual bool SetLocale(FString InLocale, const TSet<FString>& InVocabulary = TSet<FString>()) override;
	virtual FString GetLocale() const override;

	virtual FString GetImplementationName() const override
	{
		static FString ImplName = FString(TEXT("Vosk"));
		return ImplName;
	}

	virtual FOnSpeechRecognitionInitializedDelegate& OnSpeechRecognitionInitialized() override;
	virtual FOnSpeechRecognitionResultDelegate& OnSpeechRecognitionResult() override;
	virtual FOnSpeechRecognitionLocaleChangedDelegate& OnSpeechRecognitionLocaleChanged() override;

private:
	bool bInitialized;
	bool bActive;

	FString Locale;

	FSpeechRecognitionModelManager* ModelManager;
	class FVoskRecognizer Recognizer;

	FOnSpeechRecognitionInitializedDelegate OnSpeechRecognitionInitializedDelegate;
	FOnSpeechRecognitionResultDelegate OnSpeechRecognitionResultDelegate;
	FOnSpeechRecognitionLocaleChangedDelegate OnSpeechRecognitionLocaleChangedDelegate;
	FOnSpeechRecognitionImplementationChangedDelegate OnSpeechRecognitionImplementationChangedDelegate;
};