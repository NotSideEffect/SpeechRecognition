
#pragma once

#include "SpeechResult.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SpeechResultFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class SPEECHRECOGNITION_API USpeechResultFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Speech Recognition|Result")
	static FORCEINLINE EResultTypes GetType(const FSpeechResult& InResult)
	{
		return InResult.GetType();
	}

	UFUNCTION(BlueprintPure, Category = "Speech Recognition|Result")
	static FORCEINLINE FString GetText(const FSpeechResult& InResult)
	{
		return InResult.GetText();
	}

	UFUNCTION(BlueprintPure, Category = "Speech Recognition|Result")
	static FORCEINLINE TArray<FSpeechResultWord> GetWords(const FSpeechResult& InResult)
	{
		return InResult.GetWords();
	}

	UFUNCTION(BlueprintPure, Category = "Speech Recognition|Result")
	static FORCEINLINE bool Contains(const FSpeechResult& InResult, const FString& InWord)
	{
		return InResult.Contains(InWord);
	}

	UFUNCTION(BlueprintPure, Category = "Speech Recognition|Result")
	static FORCEINLINE bool ContainsAny(const FSpeechResult& InResult, const TArray<FString>& InWords)
	{
		return InResult.ContainsAny(InWords);
	}

	UFUNCTION(BlueprintPure, Category = "Speech Recognition|Result")
	static FORCEINLINE bool ContainsAll(const FSpeechResult& InResult, const TArray<FString>& InWords)
	{
		return InResult.ContainsAll(InWords);
	}

	UFUNCTION(BlueprintPure, Category = "Speech Recognition|Result")
	static FORCEINLINE bool ContainsAllInOrder(const FSpeechResult& InResult, const TArray<FString>& InWords)
	{
		return InResult.ContainsAllInOrder(InWords);
	}

	UFUNCTION(BlueprintPure, Category = "Speech Recognition|Result")
	static FORCEINLINE bool IsMatch(const FSpeechResult& InResult, const TArray<FString>& InWords, EResultMatchTypes MatchType)
	{
		return InResult.IsMatch(InWords, MatchType);
	}
};
