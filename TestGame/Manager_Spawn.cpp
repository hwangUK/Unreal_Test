// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager_Spawn.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine.h"
#include "GetPower.h"

// Sets default values
AManager_Spawn::AManager_Spawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; 

	WhereToSpawnAI = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnAI"));
	RootComponent = WhereToSpawnAI;
}

// Called when the game starts or when spawned
void AManager_Spawn::BeginPlay()
{
	Super::BeginPlay();	

	//Timer ��ȯ @@@@
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AManager_Spawn::SpawnObject, FMath::RandRange(1.f,2.f),false);
	//&AManager_Spawn::
}

// Called every frame
void AManager_Spawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
}

FVector AManager_Spawn::GetRandomPoint()
{
	//������ġ���� �����ϴ� �Լ� KismetMathLibrary @@

	FVector AISpawnOrigin = WhereToSpawnAI->Bounds.Origin;
	FVector AISpawnExtend = WhereToSpawnAI->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(AISpawnOrigin, AISpawnExtend);
}

void AManager_Spawn::SpawnObject() 
{
	if (WhatToSpawn != nullptr) 
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			//���͸� ���� (param) @@@@
			FActorSpawnParameters SpawnParam;
			SpawnParam.Owner = this;
			SpawnParam.Instigator = Instigator;

			//Random Loaction ��
			FVector SpawnLocation = GetRandomPoint();

			//Random Yaw��
			FRotator SpawnRatation;
			SpawnRatation.Yaw = FMath::RandRange(0.0f, 360.0f);
			SpawnRatation.Pitch = FMath::RandRange(0.0f, 360.0f);
			SpawnRatation.Roll = FMath::RandRange(0.0f, 360.0f);

			//����@@@@
			AGetPower* const SpawnedActor = World->SpawnActor<AGetPower>(WhatToSpawn, SpawnLocation, SpawnRatation, SpawnParam);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &AManager_Spawn::SpawnObject, FMath::RandRange(1.f, 2.5f), false);
		}
	}
	
}