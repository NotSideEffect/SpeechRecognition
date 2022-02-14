#pragma once

#include "vosk_api.h"
#include "SpeechRecognitionVoskModel.h"

class FVoskRecognizer
{
public:
	FVoskRecognizer();
	FVoskRecognizer(FSpeechRecognitionVoskModelPtr InModel, float InSampleRate);
	~FVoskRecognizer();

public:
	void SetMaxAlternatives(int MaxAlternatives)
	{
		vosk_recognizer_set_max_alternatives(this->RawRecognizer, MaxAlternatives);
	}

	// void SetSpkModel(SpkModel *spk_model);

	void EnableWordMetadata(bool bEnable)
	{
		vosk_recognizer_set_words(this->RawRecognizer, bEnable);
	}

	FORCEINLINE bool AcceptWaveform(const char* Data, int Length)
	{
		return (bool)vosk_recognizer_accept_waveform(this->RawRecognizer, Data, Length);
	}

	FORCEINLINE bool AcceptWaveform(const short* Data, int Length)
	{
		return (bool)vosk_recognizer_accept_waveform_s(this->RawRecognizer, Data, Length);
	}

	FORCEINLINE bool AcceptWaveform(const float* Data, int Length)
	{
		return (bool)vosk_recognizer_accept_waveform_f(this->RawRecognizer, Data, Length);
	}

	FORCEINLINE FString GetResult()
	{
		return FString(vosk_recognizer_result(this->RawRecognizer));
	}

	FORCEINLINE FString GetFinalResult()
	{
		return FString(vosk_recognizer_final_result(this->RawRecognizer));
	}

	FORCEINLINE FString GetPartialResult()
	{
		return FString(vosk_recognizer_partial_result(this->RawRecognizer));
	}

	void Reset()
	{
		vosk_recognizer_reset(this->RawRecognizer);
	}

	void SetModel(FSpeechRecognitionVoskModelPtr InModel);
	void SetSampleRate(float InSampleRate);

	void Initialize();
	void Initialize(FSpeechRecognitionVoskModelPtr InModel, float InSampleRate, const TSet<FString>& InVocabulary);

private:
	FSpeechRecognitionVoskModelPtr Model;
	struct VoskRecognizer* RawRecognizer;
	float SampleRate;
};
