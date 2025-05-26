// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Components/SGEnemyChargeAttackComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
USGEnemyChargeAttackComponent::USGEnemyChargeAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USGEnemyChargeAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (OwnerCharacter)
	{
		if (UCapsuleComponent* OwnerControllerCapsuleComponent = OwnerCharacter->GetCapsuleComponent())
		{
			OwnerControllerCapsuleComponent->SetGenerateOverlapEvents(true);
			OwnerControllerCapsuleComponent->OnComponentHit.AddDynamic(this, &USGEnemyChargeAttackComponent::OnHit);
		}
	}
}


void USGEnemyChargeAttackComponent::StartAttack(AActor* Target)
{
	if (!Target || !bCanAttack || !OwnerCharacter)
	{
		return;
	}

	TargetActor = Target;

	RegisteredTargetLocation = Target->GetActorLocation();
	
	Super::StartAttack(Target);
	
	PerformAttack(Target);
}


void USGEnemyChargeAttackComponent::PerformAttack(AActor* Target)
{
	if (!OwnerCharacter || !Target)
	{
		return;
	}

	ChargeTowardsTarget(RegisteredTargetLocation);
	
}

void USGEnemyChargeAttackComponent::ChargeTowardsTarget(const FVector& TargetLocation)
{
	if (!OwnerCharacter)
	{
		return;
	}
	//Sound
	if (ChargeSound)
	{
		UGameplayStatics::SpawnSoundAttached(ChargeSound, OwnerCharacter->GetMesh(), "ChargeSound");
	}

	bIsCharging = true;
	
	FVector Direction = (TargetLocation - OwnerCharacter->GetActorLocation()).GetSafeNormal();
	FRotator NewRotation = Direction.Rotation();
	
	OwnerCharacter->SetActorRotation(NewRotation);
	OwnerCharacter->GetCharacterMovement()->Velocity = Direction * ChargeSpeed;
}

void USGEnemyChargeAttackComponent::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bIsCharging  || !OtherActor || !TargetActor)
	{
		return;
	}

	if (OtherActor->ActorHasTag("Enemy"))
	{
		return;
	}

	if (OtherActor == TargetActor)
	{
		float DamageApplied = UGameplayStatics::ApplyDamage(
			OtherActor,
			DamageAmount,
			GetOwner()->GetInstigatorController(),
			GetOwner(),
			DamageTypeClass
		);
	}
	
	bIsCharging = false;

	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	
	FaceTarget(TargetActor);
}


void USGEnemyChargeAttackComponent::FaceTarget(AActor* Target) const
{
	if (!OwnerCharacter || !Target)
	{
		return;
	}

	FVector Direction = (Target->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal();
	FRotator NewRotation = Direction.Rotation();
	NewRotation.Pitch = 0.f;
	NewRotation.Roll = 0.f;

	OwnerCharacter->SetActorRotation(NewRotation);
}




