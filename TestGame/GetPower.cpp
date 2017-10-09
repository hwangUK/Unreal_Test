
#include "GetPower.h"
#include "TestGameCharacter.h"
#include "Engine.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "GetPower.generated.h"


// Sets default values
AGetPower::AGetPower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	

	//
	PowerUpAmount = 10.0f;
	
	//
	bIsExist = true;
	
	//胶抛平皋浆 积己
	GPowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GPowerMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshObj(TEXT("/Game/Geometry/Meshes/1M_Cube"));
	GPowerMesh->SetStaticMesh(BoxMeshObj.Object);
	RootComponent = GPowerMesh;

	//面倒眉 积己
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->AttachTo(RootComponent);
	Trigger->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));	
	
}

// Called when the game starts or when spaばwned
void AGetPower::BeginPlay()
{
	Super::BeginPlay();
	Trigger->bGenerateOverlapEvents = true;
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UE_LOG(LogClass, Error, TEXT("START"));
	Get_GPMesh()->SetSimulatePhysics(true);

	//TextHUD = CreateDefaultSubobject<UUserWidget>(TEXT("TextHUD"));
	//static ConstructorHelpers::FObjectFinder<UUserWidget> LoadHUD_BP(TEXT("/Game/UMG/PickUPTEXT.PickUPTEXT"));	
}

float AGetPower::PowerUpFunc()
{
	return PowerUpAmount;
}

void AGetPower::WasPicked_Implementation()
{
	FString PickupDebugLog = GetName();
	UE_LOG(LogClass, Log, TEXT("PICKED %s"), *PickupDebugLog);

	Destroy();
}

void AGetPower::SetActive(bool IsActive)
{
	bIsExist = IsActive;
}

bool AGetPower::IsActive()
{
	return bIsExist;
}






//==================================================

void AGetPower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGetPower::NotifyActorBeginOverlap(AActor* Other)
{
	Super::NotifyActorBeginOverlap(Other);

	if (Other && (Other != this))
	{
		//Player->PlayerPowerLevel += 1;
		//UE_LOG(LogClass, Error, TEXT("PlayerPowerLevel : %d"), Player->PlayerPowerLevel);
		UE_LOG(LogClass, Error, TEXT("EVENT!!!!"))
		//Player	PlayerPowerLevel += 1;
		//ATestGameCharacter* Player = LoadClass<ATestGameCharacter>()
	}

	//if(Other)
}

void AGetPower::NotifyActorEndOverlap(AActor* Other)
{

}


