#include "Enemies/AI/SGAIControllerEnemyFlying.h"

#include "Enemies/Components/SGEnemyChargeAttackComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

ASGAIControllerEnemyFlying::ASGAIControllerEnemyFlying()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASGAIControllerEnemyFlying::BeginPlay()
{
	Super::BeginPlay();
}

void ASGAIControllerEnemyFlying::HandleMovement()
{
	if (!AttackTarget)
	{
		return;
	}

	if (!LineOfSightTo(AttackTarget))
	{
		return;
	}
	
	float TargetZ = AttackTarget->GetActorLocation().Z;	
	float HoverZ = TargetZ + FMath::Sin(GetWorld()->TimeSeconds * HoverSpeed) * HoverAmplitude;
	
	FVector CurrentLocation = GetPawn()->GetActorLocation();
	CurrentLocation.Z = FMath::FInterpTo(CurrentLocation.Z, HoverZ, GetWorld()->GetDeltaSeconds(), 2.0f);
	GetPawn()->SetActorLocation(CurrentLocation, true);
		
	if (CanAttackTarget())
	{
		if (ControlledCharacter)
		{
			if (USGEnemyChargeAttackComponent* ChargeAttackComponent = ControlledCharacter->FindComponentByClass<USGEnemyChargeAttackComponent>())
			{
				ChargeAttackComponent->StartAttack(AttackTarget);
			}
		}
	}
	else
	{
		FlyTowardsTarget();
	}
}

void ASGAIControllerEnemyFlying::FlyTowardsTarget()
{
	if (!AttackTarget)
	{
		return;
	}
	FVector ToPlayer = AttackTarget->GetActorLocation() - GetPawn()->GetActorLocation();
	float Distance = ToPlayer.Size();

	ACharacter* AIOwner = Cast<ACharacter>(GetPawn());
	
	if (Distance > DesiredDistance)
	{
		FVector Direction = ToPlayer.GetSafeNormal();
		
		FVector NewVelocity = Direction * AIOwner->GetCharacterMovement()->MaxFlySpeed;
		AIOwner->GetCharacterMovement()->Velocity = NewVelocity;

		FRotator LookRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		AIOwner->SetActorRotation(LookRotation);
	}
	else
	{
		AIOwner->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	}
}

void ASGAIControllerEnemyFlying::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HandleMovement();
}