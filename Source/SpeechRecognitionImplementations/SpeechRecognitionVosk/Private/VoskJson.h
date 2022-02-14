#pragma once

#include "CoreMinimal.h"
#include "Json.h"
#include "SpeechResult.h"
#include "UObject/ObjectMacros.h"

class VoskJsonHelper
{
public:
	static bool ParseJson(const FString& InString, TArray<FSpeechResult>& OutResults)
	{
		UE_LOG(LogTemp, Verbose, TEXT("ParseJson..."));
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(InString);

		if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
		{
			const TArray<TSharedPtr<FJsonValue>>* Alternatives;
			if (JsonObject->TryGetArrayField(TEXT("alternatives"), Alternatives))
			{
				for (TSharedPtr<FJsonValue> AlternativeJsonValue : *Alternatives)
				{
					OutResults.Add(GetResult(AlternativeJsonValue->AsObject()));
				}
				return true;
			}
			else
			{
				OutResults.Add(GetResult(JsonObject));
				return true;
			}
		}

		UE_LOG(LogTemp, Error, TEXT("Failed to parse Json!"));
		return false;
	}

private:
	static FSpeechResult GetResult(TSharedPtr<FJsonObject> JsonObject)
	{
		FSpeechResult Result;

		if (!JsonObject->TryGetStringField(TEXT("text"), Result.Text))
		{
			if (JsonObject->TryGetStringField(TEXT("partial"), Result.Text))
			{
				Result.Type = EResultTypes::Partial;
				return Result;
			}

			UE_LOG(LogTemp, Error, TEXT("Invalid Object"));
			return Result;
		}

		double ResultConfidence;
		if (JsonObject->TryGetNumberField(TEXT("confidence"), ResultConfidence))
		{
			Result.Confidence = ResultConfidence;
			Result.Type = EResultTypes::Alternative;
		}
		else
		{
			Result.Type = EResultTypes::Final;
		}

		const TArray<TSharedPtr<FJsonValue>>* Words;
		if (JsonObject->TryGetArrayField(TEXT("result"), Words))
		{
			for (TSharedPtr<FJsonValue> WordJsonValue : *Words)
			{
				const TSharedPtr<FJsonObject>& WordObject = WordJsonValue->AsObject();
				FString Word;
				double Confidence = -1.0;
				double StartTime = -1.0;
				double EndTime = -1.0;

				WordObject->TryGetNumberField(TEXT("conf"), Confidence);

				bool Success = WordObject->TryGetStringField(TEXT("word"), Word) && WordObject->TryGetNumberField(TEXT("start"), StartTime) && WordObject->TryGetNumberField(TEXT("end"), EndTime);
				if (Success)
				{
					Result.AddWord(Word, Confidence, StartTime, EndTime);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Invalid Word"));
				}
			}
		}

		return Result;
	}
};