#pragma once

#include "CoreMinimal.h"
#include "Objectives/SGObjectiveInterface.h"
#include "GameFramework/Character.h"
#include "SGEnemyCharacter.generated.h"

class USGEnemyAttackComponentBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDied, ASGEnemyCharacter*, DeadEnemy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDiedObjective, UObject*, ObjectiveInterfaceImplementor);

UCLASS()
class SPM_TEST_NO_LFS_API ASGEnemyCharacter : public ACharacter, public ISGObjectiveInterface
{
	GENERATED_BODY()

public:
	ASGEnemyCharacter();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void HandleDeath(float NewHealth);

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual EObjectiveType GetObjectiveType() override { return EObjectiveType::EOT_KillAllEnemies | EObjectiveType::EOT_FinalSweep; }
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemyDied OnEnemyDied;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemyDiedObjective OnEnemyDiedObjective;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Enemy", meta = (AllowPrivateAccess = "true"))
	float JumpStrength = 500.f; 

	UFUNCTION(BlueprintCallable)
	USGEnemyAttackComponentBase* GetAttackComponent() const;

	UFUNCTION(BlueprintCallable)
	virtual void JumpToLocation(const FVector Destination);

	UFUNCTION(BlueprintCallable)
	virtual void AdjustJumpRotation();

	FTimerHandle JumpTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float JumpHorizontalSpeed = 600.f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Enemy", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USGHealthComponent> HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Enemy", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USGEnemyAttackComponentBase> AttackComponent;

public:
	UFUNCTION(BlueprintCallable, Category = "UFunction - Enemy")
	void ApplyPush(const FVector& PushDirection, float PushStrength = 500.f);



};

