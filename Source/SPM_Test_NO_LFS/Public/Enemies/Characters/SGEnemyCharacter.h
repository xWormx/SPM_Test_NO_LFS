#pragma once

#include "CoreMinimal.h"
#include "Objectives/SGObjectiveInterface.h"
#include "GameFramework/Character.h"
#include "SGEnemyCharacter.generated.h"

class USGEnemyHealthBarWidget;
class UBehaviorTreeComponent;
class ASGAIControllerEnemy;
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

	virtual void SetBehaviorTreeEnabled(bool bEnabled);

	virtual EObjectiveType GetObjectiveType() override { return EObjectiveType::EOT_KillAllEnemies | EObjectiveType::EOT_FinalSweep; }
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemyDied OnEnemyDied;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemyDiedObjective OnEnemyDiedObjective;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Enemy", meta = (AllowPrivateAccess = "true"))
	float MaxJumpHeight = 280.f;

	UFUNCTION(BlueprintCallable)
	USGEnemyAttackComponentBase* GetAttackComponent();

	UFUNCTION(BlueprintCallable)
	virtual void JumpToLocation(const FVector Destination);

	virtual const float GetJumpVelocity(float VerticalLaunchVelocity, float HeightDifference) const;

	UFUNCTION(BlueprintCallable)
	virtual void AdjustJumpRotation();

	FTimerHandle JumpTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float JumpHorizontalSpeed = 600.f;

	UFUNCTION(BlueprintCallable)
	void HandleDeathBarOnHurt(float NewHealth,  float MaxHealth, UWidgetComponent* HealthBar, UProgressBar* HealthProgressBar);

	UFUNCTION(BlueprintCallable)
	void SetHealthBarVisible(UWidgetComponent* HealthBar, bool bVisible);

	FTimerHandle HealthBarDelayTimerHandle;

	UFUNCTION(BlueprintCallable)
	virtual void HandleDeathBarOnDeath(UWidgetComponent* HealthBar, UProgressBar* HealthProgressBar);

	void PlayDeathEffect() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Enemy", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USGHealthComponent> HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Enemy", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USGEnemyAttackComponentBase> AttackComponent;

	ASGAIControllerEnemy* AIController;

	UPROPERTY(EditAnywhere, Category = "DeathEffect")
	class UNiagaraSystem* DeathEffect;

public:
	UFUNCTION(BlueprintCallable, Category = "UFunction - Enemy")
	void ApplyPush(const FVector& PushDirection, float PushStrength = 500.f);

	UPROPERTY(EditAnywhere, Category = "SoundFX")
	class USoundBase* DeathSound;
	
};

