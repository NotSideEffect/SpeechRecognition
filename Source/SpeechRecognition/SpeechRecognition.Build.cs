
using System;
using System.IO;

namespace UnrealBuildTool.Rules
{
	public class SpeechRecognition : ModuleRules
	{
		public SpeechRecognition(ReadOnlyTargetRules Target) : base(Target)
		{
			PublicDependencyModuleNames.AddRange(
				new string[] {
					"Core",
					"CoreUObject",
					"Engine",
					"SpeechRecognitionCore", 
					"AudioMixer",
					"SignalProcessing",
					"AudioCapture",
					"AIModule"
				}
			);

			PrivateDependencyModuleNames.Add("SpeechRecognitionVosk");
		}
	}
}


