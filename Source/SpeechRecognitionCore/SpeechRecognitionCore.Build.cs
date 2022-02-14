
using System;
using System.IO;

namespace UnrealBuildTool.Rules
{
	public class SpeechRecognitionCore : ModuleRules
	{
		public SpeechRecognitionCore(ReadOnlyTargetRules Target) : base(Target)
		{
			PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Voice", "AudioMixer", "SignalProcessing", "AudioCapture" });
			PrivateDependencyModuleNames.AddRange(new string[] {  });
		}
	}
}
