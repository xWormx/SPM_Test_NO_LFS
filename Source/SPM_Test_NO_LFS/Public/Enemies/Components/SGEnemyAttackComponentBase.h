// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SGEnemyAttackComponentBase.generated.h"


UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_TEST_NO_LFS_API USGEnemyAttackComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USGEnemyAttackComponentBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	virtual void StartAttack(AActor* Target);

protected:
	
	virtual void PerformAttack(AActor* Target);

	virtual void ResetAttackCooldown();

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	float DamageAmount = 10.0f;

	UPROPERTY(EditAnywhere)
	float AttackCooldown = 2.0f;

	bool bCanAttack = false;
	
	struct FTimerHandle CooldownTimerHandle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageType> DamageTypeClass;
	
	TObjectPtr<class ACharacter> OwnerCharacter;

	TObjectPtr<AActor> TargetActor;

	UPROPERTY(EditAnywhere)
	float AttackRadius = 10.f;

		
};
