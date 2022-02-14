#include "Features/IModularFeatures.h"
#include "Modules/ModuleManager.h"
#include "SpeechRecognitionCoreModule.h"
#include "SpeechRecognitionVoskSubsystem.h"

class FSpeechRecognitionVoskModule : public IModuleInterface
{
public:
	static FName GetImplementationName()
	{
		return FName(TEXT("Vosk"));
	}

	virtual void StartupModule() override
	{
		this->Subsystem = MakeShared<FSpeechRecognitionVoskSubsystem, ESPMode::ThreadSafe>();
		FSpeechRecognitionCoreModule& SpeechRecognitionCore = FModuleManager::GetModuleChecked<FSpeechRecognitionCoreModule>("SpeechRecognitionCore");
		SpeechRecognitionCore.RegisterSubsystem(GetImplementationName(), this->Subsystem);
	}

	virtual void ShutdownModule() override
	{
		FSpeechRecognitionCoreModule& SpeechRecognitionCore = FModuleManager::GetModuleChecked<FSpeechRecognitionCoreModule>("SpeechRecognitionCore");
		SpeechRecognitionCore.UnregisterSubsystem(GetImplementationName());
	}

private:
	FSpeechRecognitionVoskSubsystemPtr Subsystem;
};

IMPLEMENT_MODULE(FSpeechRecognitionVoskModule, SpeechRecognitionVosk);