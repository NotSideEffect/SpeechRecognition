#pragma once

#include "SpeechRecognitionCoreSubsystem.h"
#include "SpeechRecognitionSession.h"
#include "SpeechRecognitionVoskModel.h"
#include "SpeechRecognitionVoskSession.h"
#include "SpeechRecognitionVoskSubsystem.h"

class FSpeechRecognitionVoskSubsystem : public TSharedFromThis<FSpeechRecognitionVoskSubsystem, ESPMode::ThreadSafe>, public ISpeechRecognitionSubsystem
{
public:
	FSpeechRecognitionVoskSubsystem()
	{
	}

	virtual ~FSpeechRecognitionVoskSubsystem()
	{
	}

	virtual FName GetImplementationName() const
	{
		return FName(TEXT("Vosk"));
	}

	virtual TUniquePtr<ISpeechRecognitionSession> CreateSession() override
	{
		return TUniquePtr<ISpeechRecognitionSession>(new FSpeechRecognitionVoskSession());
	}

	virtual FSpeechRecognitionModelPtr CreateModel(const FString& InLocale, const FString& InModelPath) override
	{
		return MakeShared<FSpeechRecognitionVoskModel, ESPMode::ThreadSafe>(InLocale, InModelPath);
	}
};

typedef TSharedPtr<FSpeechRecognitionVoskSubsystem, ESPMode::ThreadSafe> FSpeechRecognitionVoskSubsystemPtr;