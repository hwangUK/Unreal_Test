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

	//bIsExist �� ��ȯ
	UFUNCTION(BlueprintPure, category = "GetPowerUKH", meta = (AllowPrivateAccess = "true"))
		bool IsActive();

	// bIsExist ���� �ٸ�Ŭ���������� ����
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

	//��𼭵� �Ŀ����� ���´�
	float PowerUpFunc();

};
