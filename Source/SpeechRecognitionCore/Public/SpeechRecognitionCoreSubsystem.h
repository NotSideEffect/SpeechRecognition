#pragma once

#include "SpeechRecognitionModel.h"
#include "SpeechRecognitionSessionInterface.h"

typedef TSharedPtr<class ISpeechRecognitionSubsystem, ESPMode::ThreadSafe> ISpeechRecognitionSubsystemPtr;

class ISpeechRecognitionSubsystem : public TSharedFromThis<ISpeechRecognitionSubsystem, ESPMode::ThreadSafe>
{
public:
	ISpeechRecognitionSubsystem()
	{
	}

	virtual ~ISpeechRecognitionSubsystem()
	{
	}

	static ISpeechRecognitionSubsystemPtr Get(const FName& SubsystemName = NAME_None);

	virtual FName GetImplementationName() const = 0;

	virtual TUniquePtr<ISpeechRecognitionSession> CreateSession() = 0;
	virtual FSpeechRecognitionModelPtr CreateModel(const FString& InLocale, const FString& InModelPath) = 0;
};
