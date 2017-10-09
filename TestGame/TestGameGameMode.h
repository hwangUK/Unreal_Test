// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestGameGameMode.generated.h"

UCLASS(minimalapi)
class ATestGameGameMode : public AGameModeBase
{
	GENERATED_BODY()
		
public:
	ATestGameGameMode();	
	
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	// ���ѽð� Ÿ�̸�
	UFUNCTION(BlueprintCallable, Category = Time)
		float MyTimer();

	UPROPERTY(BlueprintReadWrite, Category = Time, Meta = (BlueprintProtected = "true"))
		 float MyTime;

	//���ӿ����� ȣ��
	UFUNCTION(BlueprintCallable, Category = Time)
		void GameOverFunc();

protected:	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Time, Meta = (BlueprintProtected = "true"))
		bool IsFinish;

	//HUD ���� �Լ��� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Time, Meta = (BlueprintProtected = "true"))
	TSubclassOf<class  UUserWidget> HUDWidgetClass;

	UPROPERTY()
	class UUserWidget* NowWidget;
};