// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "GameFramework/GameModeBase.h"
#include "HTTPTestGameMode.generated.h"

UENUM(BlueprintType)
enum class ECity : uint8
{
	EC_Tokyo UMETA(DisplayName = "Tokyo"),
	EC_London UMETA(DisplayName = "London"),
	EC_NewYork UMETA(DisplayName = "New York"),
	EC_Default UMETA(DisplayName = "Default"),
};

UCLASS(minimalapi)
class AHTTPTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHTTPTestGameMode();

	void BeginPlay() override;

protected:
	UFUNCTION(BlueprintCallable)
	void SendHTTPGet();
	//Handle the HttpRequest Response
	void OnGetTimeResponse(TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> HttpRequest, TSharedPtr<IHttpResponse, ESPMode::ThreadSafe> Response, bool bConnectedSuccesfully);

	void SwitchOnCity();

	void BreakTime();

	void SecondCounterCallback();

public:
	UFUNCTION(BlueprintCallable)
	FText GetCurrentTime();

	UFUNCTION(BlueprintCallable)
	void SetCurrentCity(ECity CurrentCity);

private:

	class FHttpModule* Http;

	ECity City;

	FString CityURL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Time, meta = (AllowPrivateAccess = "true"))
	FDateTime Time;

	int32 Hour;
	int32 Minute;
	int32 Second;

	FTimerHandle SecondCounter;
	float SecondCountFloat = 1.f;
};



