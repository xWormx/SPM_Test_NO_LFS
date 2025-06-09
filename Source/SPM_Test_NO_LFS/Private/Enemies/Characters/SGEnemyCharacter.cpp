#include "Enemies/Characters/SGEnemyCharacter.h"

#include "NiagaraFunctionLibrary.h"
#include "SPM_Test_NO_LFS.h"
#include "Components/ProgressBar.h"
#include "Components/SGHealthComponent.h"
#include "Components/WidgetComponent.h"
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
}

void ASGEnemyCharacter::HandleDeath(float NewHealth)
{
	SetBehaviorTreeEnabled(false);
	GetGameInstance()->GetSubsystem<USGPickUpSubsystem>()->DropItem(this);

	BASIR_LOG (Warning, TEXT("Enemy Died: %s"), *GetName());
	OnEnemyDied.Broadcast(this);
	OnEnemyDiedObjective.Broadcast(this);

	
	HealthComponent->SetCurrentHealth(HealthComponent->GetMaxHealth());

	if (DeathSound)
	{
		UGameplayStatics::SpawnSoundAttached(DeathSound, GetMesh(), "DeathSound");
	}

	PlayDeathEffect();
	
	GetGameInstance()->GetSubsystem<USGObjectPoolSubsystem>()->ReturnObjectToPool(this);
}

void ASGEnemyCharacter::PlayDeathEffect() const
{
	if (DeathEffect)
	{
		GetCharacterMovement()->StopMovementImmediately();
		const FVector DeathEffectLocation = GetActorLocation();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			DeathEffect,
			DeathEffectLocation
			);
	}
}

void ASGEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASGEnemyCharacter::SetBehaviorTreeEnabled(bool bEnabled)
{

	if (!AIController)
	{
		BASIR_LOG(Warning, TEXT("No AIController"));
		return;
	}

	AIController->SetBehaviorTreeEnabled(bEnabled);
}

USGEnemyAttackComponentBase* ASGEnemyCharacter::GetAttackComponent()
{
	return AttackComponent;
}

void ASGEnemyCharacter::JumpToLocation(const FVector Destination)
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	

	const float BaseJumpZVelocity = MovementComp->JumpZVelocity;

	const FVector StartLocation = GetActorLocation();
	const FVector DirectionToTarget = (Destination - StartLocation).GetSafeNormal();
	
	FVector LaunchVelocity = DirectionToTarget * JumpHorizontalSpeed;
	
	const float HeightDifference = Destination.Z - StartLocation.Z;
	float VerticalLaunchVelocity = BaseJumpZVelocity;

	BASIR_LOG(Warning, TEXT("Height Difference for jump is: %f"), HeightDifference);

	if (HeightDifference > MaxJumpHeight)
	{
		return;
	}

	MovementComp->bOrientRotationToMovement = false;
	MovementComp->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bUseRVOAvoidance = false;
	GetMesh()->bPauseAnims = true;

	VerticalLaunchVelocity = GetJumpVelocity(VerticalLaunchVelocity, HeightDifference);

	LaunchVelocity.Z = VerticalLaunchVelocity;
	
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, Destination);
	SetActorRotation(LookAtRotation);
	
	LaunchCharacter(LaunchVelocity, true, true);
	
	GetWorld()->GetTimerManager().SetTimer(
		JumpTimerHandle,
		this,
		&ASGEnemyCharacter::AdjustJumpRotation,
		1.f,
		false
	);
}

const float ASGEnemyCharacter::GetJumpVelocity(float VerticalLaunchVelocity, float HeightDifference) const
{
	if (HeightDifference < 0.f)
	{
		return VerticalLaunchVelocity;
	}

	float LauchZVelocity = 0.f;
	if (HeightDifference > 200)
	{
		LauchZVelocity = (VerticalLaunchVelocity * 2.f);
	}
	else if (HeightDifference > 100.f)
	{
		LauchZVelocity = (VerticalLaunchVelocity * 1.75f);
		
	}
	else if (HeightDifference > 0.f)
	{
		LauchZVelocity = (VerticalLaunchVelocity * 1.5f);
	}

	return LauchZVelocity;
}

void ASGEnemyCharacter::AdjustJumpRotation()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetMesh()->bPauseAnims = false;
}

void ASGEnemyCharacter::HandleDeathBarOnHurt(float NewHealth,  float MaxHealth, UWidgetComponent* HealthBar, UProgressBar* HealthProgressBar)
{
	float NewPercentage = NewHealth / MaxHealth;
	
	HealthProgressBar->SetPercent(NewPercentage);

	HealthProgressBar->SetFillColorAndOpacity(FColor::Green);

	SetHealthBarVisible(HealthBar, true);

	if (NewHealth < (MaxHealth/3.0))
	{
		HealthProgressBar->SetFillColorAndOpacity(FColor::Red);
		if (GetWorld()->GetTimerManager().IsTimerActive(HealthBarDelayTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(HealthBarDelayTimerHandle);
		}
	}
	else
	{
		HealthProgressBar->SetFillColorAndOpacity(FColor::Green);
		bool bVisible = false;
		GetWorld()->GetTimerManager().SetTimer(
			HealthBarDelayTimerHandle,
			[this, HealthBar, bVisible] ()
			{
				SetHealthBarVisible(HealthBar, bVisible);
			},
			2.f,
			false
			);
	}
}

void ASGEnemyCharacter::SetHealthBarVisible(UWidgetComponent* HealthBar, bool bVisible)
{
	HealthBar->SetVisibility(bVisible);
}

void ASGEnemyCharacter::HandleDeathBarOnDeath(UWidgetComponent* HealthBar, UProgressBar* HealthProgressBar)
{
	SetHealthBarVisible(HealthBar, false);
	HealthProgressBar->SetFillColorAndOpacity(FColor::Green);
}

void ASGEnemyCharacter::ApplyPush(const FVector& PushDirection, float PushStrength)
{
	FRotator CurrentRotation = GetActorRotation();
	
	LaunchCharacter(PushDirection.GetSafeNormal() * PushStrength, true, true);
	
	GetWorldTimerManager().SetTimerForNextTick([this, CurrentRotation]()
	{
		SetActorRotation(CurrentRotation);
	});
}

