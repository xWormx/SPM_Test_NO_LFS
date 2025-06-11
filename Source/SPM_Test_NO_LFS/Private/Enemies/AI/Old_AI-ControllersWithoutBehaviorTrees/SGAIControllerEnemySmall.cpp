
#include "Enemies/AI/Old_AI-ControllersWithoutBehaviorTrees/SGAIControllerEnemySmall.h"

#include "SPM_Test_NO_LFS.h"
#include "Components/CapsuleComponent.h"
#include "Enemies/AI/Old_AI-ControllersWithoutBehaviorTrees/SGAIControllerEnemyBig.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Components/SGEnemyShootAttackComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ASGAIControllerEnemySmall::ASGAIControllerEnemySmall()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASGAIControllerEnemySmall::BeginPlay()
{
	Super::BeginPlay();
}

void ASGAIControllerEnemySmall::HandleMovement()
{
	FVector Location = ControlledEnemy->GetActorLocation();
	FVector PlayerLocation = AttackTarget->GetActorLocation();

	float DistanceToPlayer = FVector::Dist(PlayerLocation, Location);
	
	
	if (DistanceToPlayer < AcceptanceRadius)
	{
		FVector DirectionAwayFromPlayer = (Location - PlayerLocation).GetSafeNormal();
		FVector MoveAwayLocation = Location + DirectionAwayFromPlayer * RetreatDistance; 
		
		MoveToLocation(MoveAwayLocation);
	}

	else
	{
		MoveToActor(AttackTarget, AcceptanceRadius);
	}
}


void ASGAIControllerEnemySmall::Tick(float DeltaTime)
{
	if (!ControlledEnemy)
	{
		BASIR_LOG(Error ,TEXT("Should Retreat Service, Controlled Enemy is Null"));
		return;
	}
	
	Super::Tick(DeltaTime);

	if (!bShouldAlwaysChaseTarget)
	{
		if (!LineOfSightTo(AttackTarget))
		{
			return;
		}
	}
	
	RotateTowardsTargetWhileNotMoving();
	
	if (CanAttackTarget())
	{
		ControlledEnemy->GetAttackComponent()->StartAttack(AttackTarget);
	}

	if (CanReachTarget(AttackTarget))
	{
		GetWorld()->GetTimerManager().ClearTimer(PatrolDelayTimerHandle);
		bShouldPatrol = false;
		HandleMovement();
	}
	else
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(PatrolDelayTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(
				PatrolDelayTimerHandle,
				this,
				&ASGAIControllerEnemySmall::PatrolDelay,
				3.f,
				false
			);
		}
		if (bShouldPatrol)
		{
			Patrol();
		}
		else
		{
			SetAttackTargetLocation();
			MoveToLocation(AttackTargetLocation);
		}
	}
}

bool ASGAIControllerEnemySmall::CanAttackTarget() const
{
	return Super::CanAttackTarget() & IsFacingTarget();
}