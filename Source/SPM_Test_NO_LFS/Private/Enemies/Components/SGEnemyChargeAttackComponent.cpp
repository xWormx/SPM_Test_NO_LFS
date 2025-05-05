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
	PrimaryComponentTick.bCanEverTick = true;

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


// Called every frame
void USGEnemyChargeAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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

	BackUpBeforeCharge();

	ChargeTowardsTarget(RegisteredTargetLocation);

	/*FVector Start = GetOwner()->GetActorLocation();
	FVector End = Target->GetActorLocation();*/

	/*FHitResult Hit;

	FCollisionQueryParams Params;

	Params.AddIgnoredActor(OwnerCharacter);

	bool bHit = GetWorld()->SweepSingleByChannel(
		Hit,
		Start,
		End,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	if (bHit && Hit.GetActor() == Target)
	{
		UGameplayStatics::ApplyDamage(
			Target,
			DamageAmount,
			GetOwner()->GetInstigatorController(),
			GetOwner(),
			DamageTypeClass
			);
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, AttackRadius, 12, FColor::Red, false, 1.f);
	}
	else
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.f);
		DrawDebugSphere(GetWorld(), End, AttackRadius, 12, FColor::Blue, false, 1.f);
	}*/
	
}

void USGEnemyChargeAttackComponent::ChargeTowardsTarget(const FVector& TargetLocation)
{
	if (!OwnerCharacter)
	{
		return;
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

		if (DamageApplied > 0)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, AttackRadius, 12, FColor::Red, false, 3.f);
		}
		if (DamageApplied < 1)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, AttackRadius, 12, FColor::Blue, false, 3.f);
		}
	}
	bIsCharging = false;

	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	
	FaceTarget(TargetActor);

	
}

void USGEnemyChargeAttackComponent::BackUpBeforeCharge()
{
	if (!OwnerCharacter)
	{
		return;
	}
	
	FVector Forward = OwnerCharacter->GetActorForwardVector();
	FVector NewLocation = OwnerCharacter->GetActorLocation() - Forward * 50.f;

	OwnerCharacter->SetActorLocation(NewLocation);
}

void USGEnemyChargeAttackComponent::FaceTarget(AActor* Target)
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




