
#include "SpeechRecognitionSession.h"

#include "SpeechRecognitionCoreSubsystem.h"

FSpeechRecognition::FSpeechRecognition()
{
}

FSpeechRecognition::~FSpeechRecognition()
{
}

bool FSpeechRecognition::Initialize(FString InImplementation, FString InLocale, const TSet<FString>& InVocabulary)
{
	this->Impl = this->CreateImpl(InImplementation);
	if (this->Impl.IsValid())
	{
		return this->Impl->Initialize(InLocale, InVocabulary);
	}

	return false;
}

uint32_t FSpeechRecognition::GetExpectedSampleRate() const
{
	if (this->Impl.IsValid())
	{
		return this->Impl->GetExpectedSampleRate();
	}

	return 0;
}

bool FSpeechRecognition::ProcessAudio(const FAudioBuffer& InBuffer)
{
	if (this->Impl.IsValid())
	{
		return this->Impl->ProcessAudio(InBuffer);
	}

	return false;
}

bool FSpeechRecognition::IsInitialized() const
{
	if (this->Impl.IsValid())
	{
		return this->Impl->IsInitialized();
	}

	return false;
}

bool FSpeechRecognition::IsActive() const
{
	if (Impl.IsValid())
	{
		return Impl->IsActive();
	}

	return false;
}

bool FSpeechRecognition::SetLocale(FString InLocale, const TSet<FString>& InVocabulary)
{
	if (this->Impl.IsValid())
	{
		return this->Impl->SetLocale(InLocale);
	}

	return false;
}

bool FSpeechRecognition::StartListening()
{
	if (this->Impl.IsValid())
	{
		return this->Impl->StartListening();
	}

	return false;
}

void FSpeechRecognition::StopListening()
{
	if (this->Impl.IsValid())
	{
		return this->Impl->StopListening();
	}
}

bool FSpeechRecognition::SupportsLocale(FString InLocale) const
{
	if (this->Impl.IsValid())
	{
		return this->Impl->SupportsLocale(InLocale);
	}

	return false;
}

bool FSpeechRecognition::AddWord(FString InWord)
{
	if (this->Impl.IsValid())
	{
		return this->Impl->AddWord(InWord);
	}

	return false;
}

bool FSpeechRecognition::RemoveWord(FString InWord)
{
	if (this->Impl.IsValid())
	{
		return this->Impl->RemoveWord(InWord);
	}

	return false;
}


bool FSpeechRecognition::ContainsWord(FString InWord) const
{
	if (this->Impl.IsValid())
	{
		return this->Impl->ContainsWord(InWord);
	}

	return false;
}

FString FSpeechRecognition::GetImplementationName() const
{
	if (this->Impl.IsValid())
	{
		return this->Impl->GetImplementationName();
	}

	return FString(TEXT("None"));
}

FString FSpeechRecognition::GetLocale() const
{
	if (Impl.IsValid())
	{
		return this->Impl->GetLocale();
	}

	return FString(TEXT("None"));
}

bool FSpeechRecognition::SetImplementation(FString InImplementation)
{
	const FString& Locale = this->GetLocale();
	Impl = this->CreateImpl(InImplementation);
	return this->SetLocale(Locale);
}

FOnSpeechRecognitionInitializedDelegate& FSpeechRecognition::OnSpeechRecognitionInitialized()
{
	return this->Impl->OnSpeechRecognitionInitialized();
}

FOnSpeechRecognitionResultDelegate& FSpeechRecognition::OnSpeechRecognitionResult()
{
	return this->Impl->OnSpeechRecognitionResult();
}

FOnSpeechRecognitionLocaleChangedDelegate& FSpeechRecognition::OnSpeechRecognitionLocaleChanged()
{
	return this->Impl->OnSpeechRecognitionLocaleChanged();
}

FOnSpeechRecognitionImplementationChangedDelegate& FSpeechRecognition::OnSpeechRecognitionImplementationChanged()
{
	return this->OnSpeechRecognitionImplementationChangedDelegate;
}

TUniquePtr<ISpeechRecognitionSession> FSpeechRecognition::CreateImpl(FString InImplementation)
{
	ISpeechRecognitionSubsystemPtr Subsystem = ISpeechRecognitionSubsystem::Get(FName(*InImplementation));
	if (Subsystem.IsValid())
	{
		return Subsystem->CreateSession();
	}

	return nullptr;
}