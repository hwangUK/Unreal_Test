// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TestGameGameMode.h"
#include "TestGameCharacter.h"
#include "Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

ATestGameGameMode::ATestGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClass(TEXT("/Script/TestGame.TestGameCharacter"));
	if (PlayerPawnClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnClass.Class;
	}
	
}

void ATestGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	//
	ATestGameCharacter* MyCharacter = Cast<ATestGameCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter)
	{
		MyTime = 1.0f;
			//(MyCharacter->PlayerPowerLevel * 2.f);
	}

	if (HUDWidgetClass != nullptr)
	{
		NowWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (NowWidget != nullptr)
		{
			NowWidget->AddToViewport();
		}
	}
}

void ATestGameGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MyTimer();

	//ATestGameCharacter* MyCharacter = Cast<ATestGameCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	//if (MyCharacter)
	//{
	//	if (MyCharacter->???????? > 0)
	//	{

	//	}
	//}
	
}

float ATestGameGameMode::MyTimer() 
{
	
	MyTime -= 0.0005f;
	if (MyTime <= 0.0000f)
	{
		GameOverFunc();
	}
	UE_LOG(LogClass, Log, TEXT("TIME  : %f"), MyTime);
	return MyTime;
}

void ATestGameGameMode::GameOverFunc()
{	

	UGameplayStatics::OpenLevel(GetWorld(), "GameOver");

	UE_LOG(LogClass, Log, TEXT("GAMEOVER"));
}