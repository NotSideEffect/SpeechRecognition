#pragma once

#include "SpeechResult.h"
#include "SpeechTypes.h"

DECLARE_MULTICAST_DELEGATE(FOnSpeechRecognitionInitializedDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSpeechRecognitionResultDelegate, const TArray<FSpeechResult>& InResult);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSpeechRecognitionLocaleChangedDelegate, FString Locale);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSpeechRecognitionImplementationChangedDelegate, FString Implementation);

class ISpeechRecognitionSession
{
public:
	ISpeechRecognitionSession(){};
	virtual ~ISpeechRecognitionSession()
	{
	}

	virtual bool Initialize(FString InLocale, const TSet<FString>& InVocabulary = TSet<FString>()) = 0;
	virtual bool IsActive() const = 0;
	virtual bool IsInitialized() const = 0;

	virtual uint32_t GetExpectedSampleRate() const = 0;

	virtual bool ProcessAudio(const FAudioBuffer& InBuffer) = 0;
	virtual bool StartListening() = 0;
	virtual void StopListening() = 0;

	virtual bool AddWord(FString InWord) = 0;
	virtual bool RemoveWord(FString InWord) = 0;
	virtual bool ContainsWord(FString InWord) const = 0;

	virtual bool SupportsLocale(FString InLocale) const = 0;
	virtual bool SetLocale(FString InLocale, const TSet<FString>& InVocabulary = TSet<FString>()) = 0;
	virtual FString GetLocale() const = 0;

	virtual FString GetImplementationName() const = 0;

	virtual FOnSpeechRecognitionInitializedDelegate& OnSpeechRecognitionInitialized() = 0;
	virtual FOnSpeechRecognitionResultDelegate& OnSpeechRecognitionResult() = 0;
	virtual FOnSpeechRecognitionLocaleChangedDelegate& OnSpeechRecognitionLocaleChanged() = 0;
};
