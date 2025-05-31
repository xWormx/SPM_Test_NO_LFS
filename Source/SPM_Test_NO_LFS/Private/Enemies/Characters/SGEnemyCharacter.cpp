#include "Enemies/Characters/SGEnemyCharacter.h"

#include "SPM_Test_NO_LFS.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CapsuleComponent.h"
#include "Components/SGHealthComponent.h"
#include "Core/SGUpgradeSubsystem.h"
#include "Enemies/AI/SGAIControllerEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utils/SGObjectPoolSubsystem.h"
#include "Utils/SGPickUpSubsystem.h"

ASGEnemyCharacter::ASGEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.5f;
	HealthComponent = CreateDefaultSubobject<USGHealthComponent>(TEXT("HealthComponent"));
	Tags.Add("Enemy");
}

void ASGEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnNoHealth.AddDynamic(this, &ASGEnemyCharacter::HandleDeath);
	}

	const float AnimationRate = FMath::FRandRange(0.8f, 1.2f);
	GetMesh()->GlobalAnimRateScale = AnimationRate;
	
	if (USGUpgradeSubsystem* UpgradeSubsystem = GetOwner()->GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>())
	{
		FName Category = TEXT("Hidden");
		FName MaxHealth = TEXT("MaxHealth");
		UpgradeSubsystem->BindAttribute(HealthComponent, MaxHealth, TEXT("EnemyHealth"), Category);
		UpgradeSubsystem->BindDependentAttribute(HealthComponent, TEXT("CurrentHealth"), false, HealthComponent, MaxHealth);
	}

	AIController = Cast<ASGAIControllerEnemy>(GetController());

	if (AIController)
	{
		BTComp = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent());
	}
}

void ASGEnemyCharacter::HandleDeath(float NewHealth)
{
	if (AIController)
	{
		if (BTComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("BT Stopped"));
			//BTComp->StopTree(EBTStopMode::Forced);
			BTComp->StopLogic(TEXT("Dead"));
		}
	}
	
	GetGameInstance()->GetSubsystem<USGPickUpSubsystem>()->DropItem(this);

	OnEnemyDied.Broadcast(this);
	OnEnemyDiedObjective.Broadcast(this);

	GetGameInstance()->GetSubsystem<USGObjectPoolSubsystem>()->ReturnObjectToPool(this);
	HealthComponent->SetCurrentHealth(HealthComponent->GetMaxHealth());

	if (DeathSound)
	{
		UGameplayStatics::SpawnSoundAttached(DeathSound, GetMesh(), "DeathSound");
	}
}

void ASGEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

USGEnemyAttackComponentBase* ASGEnemyCharacter::GetAttackComponent() const
{
	return AttackComponent;
}

void ASGEnemyCharacter::JumpToLocation(const FVector Destination)
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	MovementComp->bOrientRotationToMovement = false;
	MovementComp->bUseControllerDesiredRotation = false;
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore);
	//GetCharacterMovement()->bUseRVOAvoidance = false;

	const float BaseJumpZVelocity = MovementComp->JumpZVelocity;

	const FVector StartLocation = GetActorLocation();
	const FVector DirectionToTarget = (Destination - StartLocation).GetSafeNormal();
	
	FVector LaunchVelocity = DirectionToTarget * JumpHorizontalSpeed;
	
	const float HeightDifference = Destination.Z - StartLocation.Z;
	float VerticalLaunchVelocity = BaseJumpZVelocity;

	if (HeightDifference > 0.f)
	{
		VerticalLaunchVelocity += HeightDifference * 2.f;
	}

	LaunchVelocity.Z = VerticalLaunchVelocity * 1.5f;
	
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, Destination);
	SetActorRotation(LookAtRotation);
	
	LaunchCharacter(LaunchVelocity, true, true);
	
	GetWorld()->GetTimerManager().SetTimer(
		JumpTimerHandle,
		this,
		&ASGEnemyCharacter::AdjustJumpRotation,
		1.0f,
		false
	);
}

void ASGEnemyCharacter::AdjustJumpRotation()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	//GetCharacterMovement()->bUseRVOAvoidance = true;
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Block);
}

void ASGEnemyCharacter::ApplyPush(const FVector& PushDirection, float PushStrength)
{
	// Get current rotation before push
	FRotator CurrentRotation = GetActorRotation();

	// Apply push force
	LaunchCharacter(PushDirection.GetSafeNormal() * PushStrength, true, true);

	// Set up a timer to restore rotation after physics update
	GetWorldTimerManager().SetTimerForNextTick([this, CurrentRotation]()
	{
		SetActorRotation(CurrentRotation);
	});
}

