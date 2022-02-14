#include "SpeechRecognitionCoreSubsystem.h"

#include "SpeechRecognitionCoreLog.h"
#include "SpeechRecognitionCoreModule.h"

ISpeechRecognitionSubsystemPtr ISpeechRecognitionSubsystem::Get(const FName& SubsystemName)
{
	static const FName SpeechRecognitionCoreModuleName = TEXT("SpeechRecognitionCore");
	FSpeechRecognitionCoreModule& Module = FModuleManager::GetModuleChecked<FSpeechRecognitionCoreModule>(SpeechRecognitionCoreModuleName);
	return Module.GetSubsystem(SubsystemName);
}