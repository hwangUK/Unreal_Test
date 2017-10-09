// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (wInventory)
	{
		myInventory = CreateWidget<UUserWidget>(this, wInventory);

		if (myInventory)
		{
			myInventory->AddToViewport();
			myInventory->SetVisibility(ESlateVisibility::Visible);
		}

		bShowMouseCursor = true;
	}
}




