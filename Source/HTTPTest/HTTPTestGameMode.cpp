// Copyright Epic Games, Inc. All Rights Reserved.

#include "HTTPTestGameMode.h"
#include "HTTPTestCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Http.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/KismetMathLibrary.h"

AHTTPTestGameMode::AHTTPTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	City = ECity::EC_Tokyo;
	Http = &FHttpModule::Get();
}

void AHTTPTestGameMode::BeginPlay()
{
	Super::BeginPlay();
	SendHTTPGet();
	GetWorldTimerManager().SetTimer(SecondCounter,this,&AHTTPTestGameMode::SecondCounterCallback,SecondCountFloat);
}

void AHTTPTestGameMode::SendHTTPGet()
{

	SwitchOnCity();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

	Request->SetURL(CityURL);
	Request->SetVerb("GET");
	Request->OnProcessRequestComplete().BindUObject(this, &AHTTPTestGameMode::OnGetTimeResponse);

	Request->SetHeader("User-Agent", "X-UnrealEngine-Agent");
	//Let request know what response we are expecting
	Request->SetHeader("Content-Type", "application/json");
	//Sending the request
	Request->ProcessRequest();

}

void AHTTPTestGameMode::OnGetTimeResponse(TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> HttpRequest, TSharedPtr<IHttpResponse, ESPMode::ThreadSafe> Response, bool bConnectedSuccesfully)
{
	TSharedPtr<FJsonObject> JsonObject;

	if (Response->GetResponseCode() == 200)
	{
		const FString ResponseBody = Response->GetContentAsString();
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, ResponseBody);
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);
	
		if (FJsonSerializer::Deserialize(Reader,JsonObject))
		{
			UKismetMathLibrary::DateTimeFromIsoString(*JsonObject->GetStringField("dateTime"), Time);
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("DATE TIME: %s"),*Time.ToString()));
		}


	}
}

void AHTTPTestGameMode::SwitchOnCity()
{
	CityURL = FString("https://timeapi.io/api/Time/current/zone?timeZone=");

	switch (City)
	{
	case ECity::EC_Tokyo:
		CityURL.Append("Asia/Tokyo");
		break;
	case ECity::EC_London:
		CityURL.Append("Europe/London");
		break;
	case ECity::EC_NewYork:
		CityURL.Append("America/New_York");
		break;
	case ECity::EC_Default:
		break;
	default:
		break;
	}
}

void AHTTPTestGameMode::BreakTime()
{
	int32 Year;
	int32 Month;
	int32 Day;
	int32 MiliSecond;

	UKismetMathLibrary::BreakDateTime(Time,Year,Month,Day,Hour,Minute,Second,MiliSecond);


}

FText AHTTPTestGameMode::GetCurrentTime()
{
	FString Hours;
	FString Minutes;
	FString Seconds;

	if (Hour < 10)
	{
		Hours = FString("0").Append(FString::FromInt(Hour));
	}
	else
	{
		Hours = FString::FromInt(Hour);
	}
	if (Minute < 10)
	{
		Hours = FString("0").Append(FString::FromInt(Minute));
	}
	else
	{
		Minutes = FString::FromInt(Minute);
	}
	if (Second < 10)
	{
		Seconds = FString("0").Append(FString::FromInt(Second));
	}
	else
	{
		Seconds = FString::FromInt(Second);
	}

	FString ReturnString = Hours.Append(FString(": ")).Append(Minutes).Append(FString(": ").Append(Seconds));
	return FText::FromString(ReturnString);
}

void AHTTPTestGameMode::SetCurrentCity(ECity CurrentCity)
{
	City = CurrentCity;
	SendHTTPGet();
}

void AHTTPTestGameMode::SecondCounterCallback()
{
	Time += FTimespan::FromSeconds(1);
	BreakTime();

	GetWorldTimerManager().SetTimer(SecondCounter, this, &AHTTPTestGameMode::SecondCounterCallback, SecondCountFloat);
}
