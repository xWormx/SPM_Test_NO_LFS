// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Components/SGEnemyShootAttackComponent.h"

#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
USGEnemyShootAttackComponent::USGEnemyShootAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USGEnemyShootAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	bCanAttack = true;
}


// Called every frame
void USGEnemyShootAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USGEnemyShootAttackComponent::StartAttack(AActor* Target)
{
	if (!Target || !bCanAttack || !OwnerCharacter)
	{
		return;
	}
	Super::StartAttack(Target);
	PerformAttack(Target);
}

void USGEnemyShootAttackComponent::PerformAttack(AActor* Target)
{
	Super::PerformAttack(Target);

	FVector Start = GetOwner()->GetActorLocation();
	FVector End = Target->GetActorLocation();

	FHitResult Hit;

	FCollisionQueryParams Params;

	Params.AddIgnoredActor(GetOwner());

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
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.f);
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, AttackRadius, 12, FColor::Red, false, 1.f);
	}
	else
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.f);
		DrawDebugSphere(GetWorld(), End, AttackRadius, 12, FColor::Blue, false, 1.f);
	}
}

