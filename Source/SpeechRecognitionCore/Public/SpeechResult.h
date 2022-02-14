#pragma once

#include "SpeechTypes.h"
#include "SpeechResult.generated.h"

USTRUCT(BlueprintType)
struct FSpeechResultWord
{
	GENERATED_BODY()

	FSpeechResultWord()
	{
	}

	FSpeechResultWord(FString InWord, float InConfidence, float InStartTime, float InEndTime) : Word(InWord), Confidence(InConfidence), StartTime(InStartTime), EndTime(InEndTime)
	{
	}

	FString Word;
	float Confidence;
	float StartTime;
	float EndTime;

	void DebugPrint() const
	{
		UE_LOG(LogTemp, Warning, TEXT("Word: '%s', Confidence: %f, StartTime: %f, EndTime: %f"), *Word, Confidence, StartTime, EndTime);
	}
};

USTRUCT(BlueprintType)
struct FSpeechResult
{
	GENERATED_BODY()

	FORCEINLINE EResultTypes GetType() const
	{
		return this->Type;
	}

	FORCEINLINE FString GetText() const
	{
		return this->Text;
	}

	FORCEINLINE float GetConfidence() const
	{
		return this->Confidence;
	}

	FORCEINLINE TArray<FSpeechResultWord> GetWords() const
	{
		return this->Words;
	}

	FORCEINLINE void AddWord(const FString& InWord, double InConfidence = 1.0f, double InStartTime = 0.0f, double InEndTime = 0.0f)
	{
		this->WordMap.Add(InWord, this->Words.Add(FSpeechResultWord(InWord, InConfidence, InStartTime, InEndTime)));
	}

	FORCEINLINE bool Contains(const FString InWord) const
	{
		return this->WordMap.Contains(InWord);
	}

	FORCEINLINE bool ContainsAny(const TArray<FString>& InWords) const
	{
		for (int Index = 0; Index < InWords.Num(); ++Index)
		{
			const FString& Word = InWords[Index];
			if (this->WordMap.Contains(Word))
			{
				return true;
			}
		}

		return false;
	}

	FORCEINLINE bool ContainsAll(const TArray<FString>& InWords) const
	{
		for (int Index = 0; Index < InWords.Num(); ++Index)
		{
			const FString& Word = InWords[Index];
			if (!this->WordMap.Contains(Word))
			{
				return false;
			}
		}

		return true;
	}

	FORCEINLINE bool ContainsAllInOrder(const TArray<FString>& InWords) const
	{
		for (int Index = 0; Index < InWords.Num(); ++Index)
		{
			const FString& Word = InWords[Index];
			for (int Index2 = 0; Index2 < Words.Num(); ++Index2)
			{
				const FString& Word2 = Words[Index].Word;
				if (Word != Word2)
				{
					return false;
				}
			}
		}

		return true;
	}

	FORCEINLINE bool IsMatch(const TArray<FString>& InWords, EResultMatchTypes MatchType) const
	{
		switch (MatchType)
		{
			case EResultMatchTypes::Any: return this->ContainsAny(InWords);
			case EResultMatchTypes::All: return this->ContainsAll(InWords);
			case EResultMatchTypes::AllInOrder:  return this->ContainsAllInOrder(InWords);
			default: return false;
				break;
		}

		return false;
	}

	UPROPERTY(BlueprintReadOnly)
	EResultTypes Type;

	UPROPERTY(BlueprintReadOnly)
	FString Text;

	UPROPERTY(BlueprintReadOnly)
	float Confidence;

	UPROPERTY(BlueprintReadOnly)
	TArray<FSpeechResultWord> Words;

	TMultiMap<FString, int> WordMap;
};

typedef TFunction<bool(const FSpeechResult& InSpeechResult, const TArray<FString>& InWords)> FSpeechPredicate;

UCLASS()
class SPEECHRECOGNITIONCORE_API USpeechResultObject : public UObject
{
	GENERATED_BODY()

public:
	USpeechResultObject()
	{
	}

	FORCEINLINE void AddWord(FString InWord, double InConfidence = 1.0f, double InStartTime = 0.0f, double InEndTime = 0.0f)
	{
		this->WordMap.Add(InWord, this->Words.Add(FSpeechResultWord(InWord, InConfidence, InStartTime, InEndTime)));
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool Contains(FString Word)
	{
		return this->WordMap.Contains(Word);
	}

	int FindWord(FString Word)
	{
		return -1;
	}

	int FindSentence(FString Sentence)
	{
		TArray<FString> SentenceWords;
		Sentence.ParseIntoArray(SentenceWords, TEXT(" "), true);

		for (const FString& Word : SentenceWords)
		{
			TArray<int> Indices;
			WordMap.MultiFind(Word, Indices);
			for (int32 Index = 0; Index < Indices.Num(); ++Index)
			{
			}
		}

		return -1;
	}

	TArray<FSpeechResultWord> ExtractInstances(FString Word)
	{
		return TArray<FSpeechResultWord>();
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE EResultTypes GetType() const
	{
		return this->Type;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE FString GetText() const
	{
		return this->Text;
	}

	EResultTypes Type;
	FString Text;
	double Confidence;
	TArray<FSpeechResultWord> Words;

private:
	TMultiMap<FString, int> WordMap;
};

typedef TFunction<void(const TArray<FSpeechResult>& InResult)> FOnResultFunction;