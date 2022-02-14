#pragma once

#include "Modules/ModuleInterface.h"
#include "SpeechRecognitionSessionInterface.h"

class SPEECHRECOGNITIONCORE_API FSpeechRecognition
{
public:
	FSpeechRecognition();
	~FSpeechRecognition();

public:
	bool Initialize(FString InImplementation, FString InLocale, const TSet<FString>& InVocabulary = TSet<FString>());
	bool IsActive() const;
	bool IsInitialized() const;
	
	uint32_t GetExpectedSampleRate() const;

	bool ProcessAudio(const FAudioBuffer& InBuffer);
	bool StartListening();
	void StopListening();

	bool AddWord(FString InWord);
	bool RemoveWord(FString InWord);
	bool ContainsWord(FString InWord) const;

	bool SupportsLocale(FString InLocale) const;
	bool SetLocale(FString InLocale, const TSet<FString>& InVocabulary = TSet<FString>());
	FString GetLocale() const;

	FString GetImplementationName() const;
	bool SetImplementation(FString InImplementation);

	FOnSpeechRecognitionInitializedDelegate& OnSpeechRecognitionInitialized();
	FOnSpeechRecognitionResultDelegate& OnSpeechRecognitionResult();
	FOnSpeechRecognitionLocaleChangedDelegate& OnSpeechRecognitionLocaleChanged();
	FOnSpeechRecognitionImplementationChangedDelegate& OnSpeechRecognitionImplementationChanged();

private:
	TUniquePtr<ISpeechRecognitionSession> CreateImpl(FString InImplementation);
	TUniquePtr<ISpeechRecognitionSession> Impl;

	FOnSpeechRecognitionImplementationChangedDelegate OnSpeechRecognitionImplementationChangedDelegate;
};