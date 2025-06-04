/*

Written by
----------------------
- Carl-Johan Larson Eliasson
- cael7567
	
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Objectives/SGObjectiveBase.h"
#include "SGObjectiveFinalSweep.generated.h"


class UBoxComponent;

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEscapeWithPodEnabled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEscapeWithPodDisabled);

UCLASS()
class SPM_TEST_NO_LFS_API ASGObjectiveFinalSweep : public ASGObjectiveBase, public ISGObjectiveInterface
{
	GENERATED_BODY()
	
public:
	ASGObjectiveFinalSweep();
	virtual void BeginPlay() override;
	
	virtual bool IsCompleted() override;
	virtual void OnStart() override;
	virtual void OnCompleted() override;
	virtual void Update() override;
	virtual EObjectiveType GetObjectiveType() override { return EObjectiveType::EOT_FinalSweep; }

	void IncrementEnemiesKilled();

	FOnEscapeWithPodEnabled OnEscapeWithPodEnabled;
	FOnEscapeWithPodDisabled OnEscapeWithPodDisabled;
	
private:
	UPROPERTY(VisibleAnywhere, Category = "UPROPERTY", Meta = (AllowPrivateAccess = true))
	uint32 CurrentEnemiesKilled;

	UPROPERTY(VisibleAnywhere, Category = "UPROPERTY", Meta = (AllowPrivateAccess = true))
	USceneComponent* SceneRoot;
	
	UPROPERTY(VisibleAnywhere, Category = "UPROPERTY", Meta = (AllowPrivateAccess = true))
	USceneComponent* SceneLocation;
	
	UPROPERTY(VisibleAnywhere, Category = "UPROPERTY", Meta = (AllowPrivateAccess = true))
	UBoxComponent* EscapeTriggerZone;

	UFUNCTION()
	void EnableEscapeWithPod(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void DisableEscapeWithPod(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};


