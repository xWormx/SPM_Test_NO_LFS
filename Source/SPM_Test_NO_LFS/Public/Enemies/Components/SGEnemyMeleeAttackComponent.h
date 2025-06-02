// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGEnemyAttackComponentBase.h"
#include "SGEnemyMeleeAttackComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPM_TEST_NO_LFS_API USGEnemyMeleeAttackComponent : public USGEnemyAttackComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USGEnemyMeleeAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	virtual void StartAttack(AActor* Target) override;

protected:

	virtual void PerformAttack(AActor* Target) override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		FVector NormalImpulse,
		const FHitResult& Hit
		);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio", meta=(AllowPrivateAccess="true"))
	class USoundBase* MeleeSound;
};
