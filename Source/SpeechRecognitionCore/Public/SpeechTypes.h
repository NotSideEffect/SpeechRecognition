#pragma once

#include "SpeechTypes.generated.h"

struct FSessionInitOptions
{
	FString Locale;
	TArray<FString> Vocabulary;
};

struct FAudioBuffer
{
	TArrayView<const float> Frames;
	uint32_t SampleRate;
	uint32_t Channels;
	bool bInterleaved;
};

UENUM(BlueprintType)
enum class EResultTypes : uint8
{
	Error,
	Alternative,
	Partial,
	Final
};

UENUM(BlueprintType)
enum class EResultMatchTypes : uint8
{
	Any,
	All,
	AllInOrder
};

USTRUCT(BlueprintType)
struct FSpeechMatch
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	EResultMatchTypes MatchType;

	UPROPERTY(EditDefaultsOnly)
	TArray<FString> Words;
};