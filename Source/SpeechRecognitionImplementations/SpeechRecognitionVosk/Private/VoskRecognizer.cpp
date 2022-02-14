	#include "VoskRecognizer.h"

    FVoskRecognizer::FVoskRecognizer() : RawRecognizer(nullptr)
    {
    }
    
    FVoskRecognizer::FVoskRecognizer(FSpeechRecognitionVoskModelPtr InModel, float InSampleRate) : SampleRate(InSampleRate)
    {
        this->SetModel(InModel);
    }

	FVoskRecognizer::~FVoskRecognizer()
	{
		if (this->RawRecognizer)
		{
			vosk_recognizer_free(this->RawRecognizer);
			this->RawRecognizer = nullptr;
		}
	}

    void FVoskRecognizer::SetModel(FSpeechRecognitionVoskModelPtr InModel)
	{
        check(InModel)
        check(InModel->GetVoskModel());

        this->Model = InModel;
	}

    void FVoskRecognizer::SetSampleRate(float InSampleRate)
    {
        this->SampleRate = InSampleRate;
    }

    void FVoskRecognizer::Initialize()
    {
        if (this->RawRecognizer)
		{
			vosk_recognizer_free(this->RawRecognizer);
		}

		this->RawRecognizer = vosk_recognizer_new(this->Model->GetVoskModel(), this->SampleRate);
        UE_LOG(LogTemp, Warning, TEXT("FVoskRecognizer::Initialize"));

        this->EnableWordMetadata(true);
        //this->SetMaxAlternatives(3);
    }

    void FVoskRecognizer::Initialize(FSpeechRecognitionVoskModelPtr InModel, float InSampleRate, const TSet<FString>& InVocabulary)
    {
        this->SetModel(InModel);
        this->SetSampleRate(InSampleRate);

        this->Initialize();
    }