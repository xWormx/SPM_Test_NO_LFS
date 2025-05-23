#include "Player/SGPlayerCharacter.h"

#include "jola6902_GunsComponent.h"

#include "Gear/Grapple/SGGrapplingHook.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SGHealthComponent.h"
#include "Components/Counters/SGCounterComponentAmmo.h"
#include "Components/Counters/SGCounterComponentHealth.h"
#include "Components/Counters/SGCounterComponentOrbs.h"

#include "Core/SGGameInstance.h"

#include "Enemies/Characters/SGEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Player/SGPlayerController.h"
#include "SaveGame/SGSaveGame.h"


ASGPlayerCharacter::ASGPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PlayerController = Cast<ASGPlayerController>(GetController());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	
	if (WeaponMesh && CameraComponent)
	{
		CameraComponent->SetupAttachment(RootComponent);
		WeaponMesh->SetupAttachment(CameraComponent);
	}
	
	GrapplingHookPosition = CreateDefaultSubobject<USceneComponent>(TEXT("GrapplingHookPosition"));
	GrapplingHookPosition->SetupAttachment(GetRootComponent());

    if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
	{
		CapsuleComp->SetNotifyRigidBodyCollision(true); // Enable hit events
		CapsuleComp->OnComponentHit.AddDynamic(this, &ASGPlayerCharacter::OnComponentHit);
	}

	GunsComponent = CreateDefaultSubobject<Ujola6902_GunsComponent>(TEXT("Guns Component"));
	HealthComponent = CreateDefaultSubobject<USGHealthComponent>(TEXT("Health Component"));
	HealthShieldComponent = CreateDefaultSubobject<USGCounterComponentHealth>(TEXT("Health Shield Counter"));
	OrbsComponent = CreateDefaultSubobject<USGCounterComponentOrbs>(TEXT("Orbs Counter"));
	AmmoComponent = CreateDefaultSubobject<USGCounterComponentAmmo>(TEXT("Ammo Counter"));
}

float ASGPlayerCharacter::GetRage() const
{
	return Rage;
}

void ASGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->AirControl = AirControl;

	if ((GrapplingHook = GetWorld()->SpawnActor<ASGGrapplingHook>(GrapplingHookClass)))

	USGGameInstance* GameIns = Cast<USGGameInstance>(GetGameInstance());

	/*if (GameIns)
	{
		UseSavedGame(GameIns->GetSaveGame()->PlayerStats);
	}
	else
	{
		BASIR_LOG(Warning, TEXT("GameInstance was not found!"));
	}*/
	
	GrapplingHook = GetWorld()->SpawnActor<ASGGrapplingHook>(GrapplingHookClass);
	if (GrapplingHook)
	{
		GrapplingHook->SetActorLocation(GrapplingHookPosition->GetComponentLocation());
		// Grapplinghooken verkar vara omvänt roterad från början, debugkameran visar att den är riktad bakåt
		// Så vi roterar den 180 grader för att den ska vara riktad framåt från början.
		GrapplingHook->AddActorLocalRotation(FRotator(0, 180, 0));
		GrapplingHook->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
		GrapplingHook->SetOwner(this);
		OnGrapplingHookReady.Broadcast(GrapplingHook);
	}

	if (GunsComponent)
	{
		OnGunComponentReady.Broadcast(GunsComponent);
	}

	if (HealthComponent)
	{
		OnPlayerIsReady.Broadcast(this);
	}

	if (AmmoComponent)
	{
		OnAmmoComponentReady.Broadcast(AmmoComponent);
	}

	OnPlayerIsReady.Broadcast(this);

 }

void ASGPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASGPlayerCharacter::OnComponentHit([[maybe_unused]] UPrimitiveComponent* HitComponent, AActor* OtherActor, [[maybe_unused]] UPrimitiveComponent* OtherComp, [[maybe_unused]] FVector NormalImpulse, [[maybe_unused]] const FHitResult& Hit)
{
	ASGEnemyCharacter* Enemy = Cast<ASGEnemyCharacter>(OtherActor);
	if (!Enemy)
	{
		return;
	}

	FVector PushDirection = Enemy->GetActorLocation() - GetActorLocation();
	PushDirection.Z = 0;
	Enemy->ApplyPush(PushDirection, PushStrength);
}

FPlayerStats ASGPlayerCharacter::GetPlayerStats()
{
	FPlayerStats PlayerStats;
	PlayerStats.PlayerTransform = GetActorTransform();

	if (PlayerController)
	{
		PlayerStats.ScorePoints = PlayerController->GetScorePoint();
	}
	else
	{
		PlayerStats.ScorePoints = 0;
	}
	return PlayerStats;
}

void ASGPlayerCharacter::UseSavedGame(FPlayerStats SavedStats)
{
	SetActorTransform(SavedStats.PlayerTransform);
	if (PlayerController)
	{
		PlayerController->SetScorePoint(SavedStats.ScorePoints);
	}
}


