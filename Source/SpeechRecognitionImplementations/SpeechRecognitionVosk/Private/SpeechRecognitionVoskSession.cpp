#include "SpeechRecognitionVoskSession.h"

#include "DSP/SampleRateConverter.h"
#include "SpeechRecognitionCoreModule.h"
#include "SpeechRecognitionSession.h"
#include "SpeechRecognitionVoskModel.h"
#include "VoskJson.h"
#include "vosk_api.h"

static FString Implementation(TEXT("Vosk"));

FSpeechRecognitionVoskSession::FSpeechRecognitionVoskSession() : bInitialized(false), bActive(false)
{
}

FSpeechRecognitionVoskSession::~FSpeechRecognitionVoskSession()
{
}

bool FSpeechRecognitionVoskSession::Initialize(FString InLocale, const TSet<FString>& InVocabulary)
{
	if (this->bInitialized)
	{
		UE_LOG(LogTemp, Error, TEXT("FSpeechRecognitionVoskSession::Initialize Already Initialized"));
		return true;
	}

	this->Locale = InLocale;
	this->ModelManager = &FSpeechRecognitionModelManager::Get();
	FSpeechRecognitionVoskModelPtr Model = this->ModelManager->GetModel<FSpeechRecognitionVoskModel>(Implementation, this->Locale);
	if (Model.IsValid())
	{
		this->Recognizer.Initialize(Model, 16000.0f, InVocabulary);
		this->bInitialized = true;
		this->OnSpeechRecognitionInitializedDelegate.Broadcast();
		UE_LOG(LogTemp, Verbose, TEXT("FSpeechRecognitionVoskSession::Initialize Success"));
		return true;
	}

	return false;
}

bool FSpeechRecognitionVoskSession::ProcessAudio(const FAudioBuffer& Buffer)
{
	if (this->bActive)
	{
		TArray<float> FloatBuffer;
		TArray<float> DownsampleBuffer;
		TArray<float> SingleChannelBuffer;

		const float* FrameData = Buffer.Frames.GetData();
		uint32_t FramesCount = Buffer.Frames.Num();

		if (Buffer.SampleRate != 16000)
		{
			// UE_LOG(LogTemp, Error, TEXT("FSpeechRecognitionVoskSession::ProcessAudio Downsample"));

			Audio::ISampleRateConverter* SRC = Audio::ISampleRateConverter::CreateSampleRateConverter();
			SRC->Init(Buffer.SampleRate / 16000.0f, Buffer.Channels);
			SRC->ProcessFullbuffer(FrameData, FramesCount, DownsampleBuffer);
			delete SRC;

			FrameData = DownsampleBuffer.GetData();
			FramesCount = DownsampleBuffer.Num();
		}

		// UE_LOG(LogTemp, Error, TEXT("FSpeechRecognitionVoskSession::ProcessAudio Sample:%f"), FrameData[FramesCount / 2]);

		if (Buffer.Channels > 1)
		{
			if (Buffer.bInterleaved)
			{
				// UE_LOG(LogTemp, Error, TEXT("FSpeechRecognitionVoskSession::ProcessAudio Deinterleave"));

				for (uint32_t FrameIndex = 0; FrameIndex < FramesCount; FrameIndex += Buffer.Channels)
				{
					SingleChannelBuffer.Add(FrameData[FrameIndex]);
				}

				FrameData = SingleChannelBuffer.GetData();
				FramesCount = SingleChannelBuffer.Num();
			}
			else
			{
				// UE_LOG(LogTemp, Error, TEXT("FSpeechRecognitionVoskSession::ProcessAudio Mono"));

				for (uint32_t FrameIndex = 0; FrameIndex < FramesCount / Buffer.Channels; ++FrameIndex)
				{
					SingleChannelBuffer.Add(FrameData[FrameIndex]);
				}

				FrameData = SingleChannelBuffer.GetData();
				FramesCount = SingleChannelBuffer.Num();
			}
		}

		FString Result;
		if (this->Recognizer.AcceptWaveform(FrameData, FramesCount))
		{
			Result = this->Recognizer.GetFinalResult();
		}
		else
		{
			Result = this->Recognizer.GetPartialResult();
		}

		// Only report result on game thread
		if (IsInGameThread())
		{
			TArray<FSpeechResult> Results;
			if (VoskJsonHelper::ParseJson(Result, Results))
			{
				this->OnSpeechRecognitionResultDelegate.Broadcast(Results);
			}
		}
		else
		{
			AsyncTask(ENamedThreads::GameThread,
				[this, Result]()
				{
					TArray<FSpeechResult> Results;
					if (VoskJsonHelper::ParseJson(Result, Results))
					{
						this->OnSpeechRecognitionResultDelegate.Broadcast(Results);
					}
				});
		}

		return true;
	}

	return false;
}

bool FSpeechRecognitionVoskSession::StartListening()
{
	if (this->bActive || !this->bInitialized)
	{
		return false;
	}

	this->bActive = true;
	return true;
}

void FSpeechRecognitionVoskSession::StopListening()
{
	this->bActive = false;
	TArray<FSpeechResult> Results;
	if (VoskJsonHelper::ParseJson(this->Recognizer.GetFinalResult(), Results))
	{
		this->OnSpeechRecognitionResultDelegate.Broadcast(Results);
	}
}

bool FSpeechRecognitionVoskSession::AddWord(FString InWord)
{
	return false;
}

bool FSpeechRecognitionVoskSession::RemoveWord(FString InWord)
{
	return false;
}

bool FSpeechRecognitionVoskSession::ContainsWord(FString InWord) const
{
	return false;
}

bool FSpeechRecognitionVoskSession::SupportsLocale(FString InLocale) const
{
	return this->ModelManager->IsRegistered(Implementation, InLocale);
}

bool FSpeechRecognitionVoskSession::SetLocale(FString InLocale, const TSet<FString>& InVocabulary)
{
	FSpeechRecognitionVoskModelPtr Model = this->ModelManager->GetModel<FSpeechRecognitionVoskModel>(Implementation, InLocale);
	if (Model.IsValid())
	{
		this->Locale = InLocale;
		this->Recognizer.Initialize(Model, 16000.0f, InVocabulary);
		return true;
	}
	return false;
}

FString FSpeechRecognitionVoskSession::GetLocale() const
{
	return this->Locale;
}

FOnSpeechRecognitionInitializedDelegate& FSpeechRecognitionVoskSession::OnSpeechRecognitionInitialized()
{
	return this->OnSpeechRecognitionInitializedDelegate;
}

FOnSpeechRecognitionResultDelegate& FSpeechRecognitionVoskSession::OnSpeechRecognitionResult()
{
	return this->OnSpeechRecognitionResultDelegate;
}

FOnSpeechRecognitionLocaleChangedDelegate& FSpeechRecognitionVoskSession::OnSpeechRecognitionLocaleChanged()
{
	return this->OnSpeechRecognitionLocaleChangedDelegate;
}