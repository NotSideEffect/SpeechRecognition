using UnrealBuildTool;

public class SpeechRecognitionDeepSpeech : ModuleRules
{
	public SpeechRecognitionDeepSpeech(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.Add("Core");
		PrivateDependencyModuleNames.Add("SpeechRecognitionCore");
	}
}
