// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GetPower.generated.h"

UCLASS()
class TESTGAME_API AGetPower : public AActor
{
	GENERATED_BODY()

private:

	UPROPERTY()
		class UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "GetPowerUKH", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* GPowerMesh;


public:	
	// Sets default values for this actor's properties
	AGetPower();

	//bIsExist 값 반환
	UFUNCTION(BlueprintPure, category = "GetPowerUKH", meta = (AllowPrivateAccess = "true"))
		bool IsActive();

	// bIsExist 세팅 다른클래스에서도 접근
	UFUNCTION(BlueprintCallable, category = "GetPowerUKH", meta = (AllowPrivateAccess = "true"))
		void SetActive(bool IsActive);
	
	UFUNCTION()
	void WasPicked_Implementation();
	//

	FORCEINLINE class UStaticMeshComponent* Get_GPMesh() const { return GPowerMesh; }

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD", Meta = (BlueprintProtected = "true"))
	//	UUserWidget* TextHUD;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		bool bIsExist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Power" , Meta = (BlueprintProtected = "true"))
	float PowerUpAmount;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* Other) override;

	virtual void NotifyActorEndOverlap(AActor* Other) override;

	UObject* Player;

	//어디서든 파워값을 얻어온다
	float PowerUpFunc();

};
