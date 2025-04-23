// Fill out your copyright notice in the Description page of Project Settings.


#include "SGEnemyCharacterFlying.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/Core/Analytics/NetStatsUtils.h"

ASGEnemyCharacterFlying::ASGEnemyCharacterFlying()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
	
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);

	GetCharacterMovement()->MaxFlySpeed = 600.f;

	GetMesh()->SetEnableGravity(false);
}

void ASGEnemyCharacterFlying::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
}

void ASGEnemyCharacterFlying::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	float TargetZ = StartLocation.Z;

	if (Target)
	{
		TargetZ = Target->GetActorLocation().Z;
	}

	
	float HoverZ = TargetZ + FMath::Sin(GetWorld()->TimeSeconds * HoverSpeed) * HoverAmplitude;

	
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Z = FMath::FInterpTo(CurrentLocation.Z, HoverZ, DeltaTime, 2.0f);
	SetActorLocation(CurrentLocation, true);

	
	if (!Target)
	{
		ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (Player && FVector::Dist(Player->GetActorLocation(), GetActorLocation()) <= DetectionRadius)
		{
			Target = Player;
		}
	}

	
	if (Target)
	{
		FlyTowardsPlayer();
	}
}

void ASGEnemyCharacterFlying::FlyTowardsPlayer()
{
	float DesiredDistance = 300.f; 
	if (!Target)
	{
		return;
	}

	FVector ToPlayer = Target->GetActorLocation() - GetActorLocation();
	float Distance = ToPlayer.Size();
	
	if (Distance > DesiredDistance)
	{
		FVector Direction = ToPlayer.GetSafeNormal();
		FVector NewVelocity = Direction * GetCharacterMovement()->MaxFlySpeed;
		GetCharacterMovement()->Velocity = NewVelocity;

		FRotator LookRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		SetActorRotation(LookRotation);
	}
	else
	{
		GetCharacterMovement()->Velocity = FVector::ZeroVector;
	}
}
