// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GetPower.h"
#include "TestGameCharacter.generated.h"

UCLASS(config=Game)
class ATestGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Trigger, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* PlayerSphere;

	class USceneComponent* RootSceneComponent;

	//NOWPOWER
	UPROPERTY(VisibleAnywhere, Category = Power)
    	float CharacterNowPowerLv;

	//캐릭터 스켈레탈 메쉬 불러오기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GetPowerUKH, meta = (AllowPrivateAccess = "true"))
		class USkeletalMesh* PlayerSkeletalMesh;

	

	

public:
	ATestGameCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	//캐릭터 파워업 함수
	UFUNCTION(BlueprintCallable, Category = PickUp)
		float CharacterUpdatePowerFunc(float howMuchPower);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp)
	float CollectedPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
		float NowRemainTime;

	//UFUNCTION()
	//	UUserWidget* MainUI;
	
	//UFUNCTON(BlueprintCallable)
	//	void PullTrigger_Event();

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	//STARTPOWER
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Power)
	float CharacterStartPowerLv;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	UFUNCTION(BlueprintCallable, Category = PickUp)
	void PickUpItem();

	//위젯생성
	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = Widgets)
		TSubclassOf<UUserWidget> wInventory;

	UUserWidget* myInventory;
	UUserWidget* GetInven;
	

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void OpenInventory();

	

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//
	FORCEINLINE class USphereComponent* GetPlayerSphere() const { return PlayerSphere; }

	UPROPERTY()
		int32 PlayerPowerLevel;

	virtual void BeginPlay() override;
};

