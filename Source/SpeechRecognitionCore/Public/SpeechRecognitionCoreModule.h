
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "SpeechRecognitionCoreSubsystem.h"
#include "SpeechRecognitionModelManager.h"

class FSpeechRecognitionCoreModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	FSpeechRecognitionModelManager& GetModelManager()
	{
		return this->ModelManager;
	}

	ISpeechRecognitionSubsystemPtr GetSubsystem(const FName& SubsystemName)
	{
		if (this->SubsystemMap.Contains(SubsystemName))
		{
			return this->SubsystemMap[SubsystemName];
		}

		return nullptr;
	}

	bool RegisterSubsystem(const FName& SubsystemName, ISpeechRecognitionSubsystemPtr Subsystem)
	{
		if (!this->SubsystemMap.Contains(SubsystemName))
		{
			this->SubsystemMap.Add(SubsystemName, Subsystem);
			return true;
		}

		return false;
	}

	bool UnregisterSubsystem(const FName& SubsystemName)
	{
		if (this->SubsystemMap.Contains(SubsystemName))
		{
			this->SubsystemMap.Remove(SubsystemName);
			return true;
		}

		return false;
	}

private:
	FSpeechRecognitionModelManager ModelManager;
	TMap<FName, ISpeechRecognitionSubsystemPtr> SubsystemMap;
};
