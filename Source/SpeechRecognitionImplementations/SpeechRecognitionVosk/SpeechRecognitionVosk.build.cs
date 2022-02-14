
using UnrealBuildTool;
using System;
using System.IO;

public class SpeechRecognitionVosk : ModuleRules
{
	private string ThirdPartyPath
	{
       	get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../../ThirdParty/")); }
	}

	private string VoskLibFileName
	{
		get 
		{ 
			if (Target.Platform == UnrealTargetPlatform.Mac)
			{
				return "libvosk.dylib";
			}
			else if(Target.Platform == UnrealTargetPlatform.Win64)
			{
				return "libvosk.lib";
			}

			return "libvosk.dylib";
		}
	}

	private string VoskPath
	{
		get { return Path.GetFullPath(Path.Combine(ThirdPartyPath, "vosk/")); }
	}

	private string VoskIncludePath
	{
		get { return Path.GetFullPath(Path.Combine(VoskPath, "include/")); }
	}

	private string VoskLibPath
	{
		get { return Path.GetFullPath(Path.Combine(VoskPath, "lib/")); }
	}

	private string VoskLibraryPath
	{
		get { return Path.GetFullPath(Path.Combine(VoskLibPath, VoskLibFileName)); }
	}

	public SpeechRecognitionVosk(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "SpeechRecognitionCore", "Json", "SignalProcessing" });
		PublicIncludePaths.Add(VoskIncludePath);
		PublicAdditionalLibraries.Add(VoskLibraryPath);

		if (Target.Platform == UnrealTargetPlatform.Mac)
		{
		}
		else if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			RuntimeDependencies.Add(Path.Combine("$(BinaryOutputDir)/", "libvosk.dll"), Path.Combine(VoskPath, "bin/libvosk.dll"));
		}

		
	}
}
