// Fill out your copyright notice in the Description page of Project Settings.


#include "SGAIControllerEnemyFlying.h"

#include <ThirdParty/ShaderConductor/ShaderConductor/External/DirectXShaderCompiler/include/dxc/DXIL/DxilConstants.h>

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

ASGAIControllerEnemyFlying::ASGAIControllerEnemyFlying()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASGAIControllerEnemyFlying::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetPawn()->GetActorLocation();
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
	
	
	FlyTowardsTarget();
	
}

void ASGAIControllerEnemyFlying::FlyTowardsTarget()
{
	float DesiredDistance = 300.f; 

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
