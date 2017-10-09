// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TestGameCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GetPower.h"
#include "Blueprint/UserWidget.h"
#include "Engine.h"

//////////////////////////////////////////////////////////////////////////
// ATestGameCharacter

ATestGameCharacter::ATestGameCharacter()
{

	PlayerPowerLevel = 1;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	//RootComponent = RootSceneComponent;

	//캐릭터 초기값 설정 파워레벨
	CharacterStartPowerLv = 5.f;
	CharacterNowPowerLv = CharacterStartPowerLv;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//
	PlayerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerSphere"));
	PlayerSphere->AttachTo(RootComponent);
	PlayerSphere->SetSphereRadius(PlayerSphere->GetScaledSphereRadius() + 55.0f);

	//캐릭터 생성과 메쉬 불러오기
	PlayerSkeletalMesh = CreateDefaultSubobject<USkeletalMesh>(TEXT("PlayerSkeletalMesh"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMeshFinder(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin"));
	if (PlayerMeshFinder.Succeeded()) {
		GetMesh()->SetSkeletalMesh(PlayerMeshFinder.Object);
	}

	//AnimBluePrint Load
	//anim = CreateDefaultSubobject<UAnimInstance>(TEXT("AnimInstance"));
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP(TEXT("/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP"));	
	if (AnimBP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimBP.Object->GeneratedClass);
	}

	//캐릭터 메쉬 Transform값 변경 
	FRotator PlayerMeshRotation;
	PlayerMeshRotation.Yaw = 270.f;
	PlayerMeshRotation.Pitch = 0.0f;
	PlayerMeshRotation.Roll = 0.0f;

	FVector PlayerMeshLocation;
	PlayerMeshLocation.X = 0.0f;
	PlayerMeshLocation.Y = 0.0f;
	PlayerMeshLocation.Z = -90.0f;

	GetMesh()->AddLocalRotation(PlayerMeshRotation);
	GetMesh()->SetRelativeLocation(PlayerMeshLocation);

	//FollowCam Transform값 설정
	FRotator FollowCamRotation;
	FollowCamRotation.Yaw = 0.0f;
	FollowCamRotation.Pitch = 330.0f;
	FollowCamRotation.Roll = 0.0f;

	FVector FollowCamLocation;
	FollowCamLocation.X = 0.0f;
	FollowCamLocation.Y = 0.0f;
	FollowCamLocation.Z = 200.0f;

	FollowCamera->AddLocalRotation(FollowCamRotation);
	FollowCamera->SetRelativeLocation(FollowCamLocation);

	
	//MainUI = CreateDefaultSubobject<UUserWidget>(TEXT("HUD"));
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATestGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	//this->PullTrigger_Event();

}

void ATestGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//
	PlayerInputComponent->BindAction("Collect", IE_Pressed, this, &ATestGameCharacter::PickUpItem);
	//
	PlayerInputComponent->BindAction("ItemOpen", IE_Pressed, this, &ATestGameCharacter::OpenInventory);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATestGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATestGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATestGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATestGameCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ATestGameCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ATestGameCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ATestGameCharacter::OnResetVR);
}


void ATestGameCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATestGameCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ATestGameCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ATestGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATestGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATestGameCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATestGameCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

//

void ATestGameCharacter::PickUpItem()
{
	TArray<AActor*> CollectedActor;

	CollectedPower = 0.f;
	PlayerSphere->GetOverlappingActors(CollectedActor);	
	

	for (int32 i = 0; i < CollectedActor.Num(); i++)
	{
		AGetPower* const PowerActor = Cast<AGetPower>(CollectedActor[i]);
		if (PowerActor && !PowerActor->IsPendingKill() && PowerActor->IsActive())
		{
			PowerActor->WasPicked_Implementation();			
			if (PowerActor) {
				//캐릭터에 파워값 추가하기
				CollectedPower += PowerActor->PowerUpFunc();
				UE_LOG(LogClass, Error, TEXT("CollectedPower %f"), CollectedPower);
			}
			PowerActor->SetActive(false);
		}
	} 
	if (CollectedPower > 0.f) {
		CharacterUpdatePowerFunc(CollectedPower);
	}
}

float ATestGameCharacter::CharacterUpdatePowerFunc(float howMuchPower)
{
	CharacterNowPowerLv += howMuchPower;
	UE_LOG(LogClass, Error, TEXT("%f"), CharacterNowPowerLv);
	GetCharacterMovement()->MaxWalkSpeed += (CharacterNowPowerLv * 0.001f);
	return CharacterNowPowerLv;
}

void ATestGameCharacter::OpenInventory()
{

	UE_LOG(LogClass, Error, TEXT("ITEMOPEN@@!!!@!"));
	
	//UBlueprintGeneratedClass* bpGC = LoadObject<UBlueprintGeneratedClass>(nullptr, TEXT("/ Game / UMG / HUD.HUD"));


	UClass* bpGC = LoadObject<UClass>(this, TEXT("/Game/UMG/HUD.HUD_C"));
	if (bpGC)
	{
		UUserWidget* MyInven = CreateWidget<UUserWidget>(GetWorld(), bpGC);
	}


		//myInventory = CreateDefaultSubobject<UUserWidget>(TEXT("InvenUI"));
		//static ConstructorHelpers::FObjectFinder<UUserWidget> GetInven(TEXT("/Game/UMG/HUD.HUD"));
		//CreateWidget(myInventory);

		//if (myInventory)
		//{
		//	myInventory->SetVisibility(ESlateVisibility::Visible);
		//	myInventory->AddToViewport();
		//}

		//bShowMouseCursor = true;
	
	
}
