// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Manager_Spawn.generated.h"

UCLASS()
class TESTGAME_API AManager_Spawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AManager_Spawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//�������� ��ȯ�Ǵ� ���� ��
	UFUNCTION(BlueprintPure, Category = "Spawn")
		FVector GetRandomPoint();

	//���� �ڽ� ����
	FORCEINLINE class UBoxComponent* GetSpawnAIBox() const{ return WhereToSpawnAI; }



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//GetPowerŬ����Ÿ���� ��ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AGetPower> WhatToSpawn;

	//TimerHandle
	FTimerHandle SpawnTimer;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* WhereToSpawnAI;

	UFUNCTION()
		void SpawnObject();
	
};
