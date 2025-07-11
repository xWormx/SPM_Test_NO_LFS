/*

Written by
----------------------
- Carl-Johan Larson Eliasson
- cael7567
	
*/

#pragma once

#include "CoreMinimal.h"
#include "LevelSequence.h"
#include "Objectives/SGObjectiveBase.h"
#include "SGObjectivePodArrival.generated.h"

class ULevelSequencePlayer;
class ALevelSequenceActor;
class USphereComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaitForPodEventStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaitForPodEventEnd, UObject*, ObjectiveInterfaceImplementor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaitForPodEventPaused);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaitForPodEventUnPaused);
/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API ASGObjectivePodArrival : public ASGObjectiveBase, public ISGObjectiveInterface
{
	GENERATED_BODY()
	public:
	
	ASGObjectivePodArrival();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual bool IsCompleted() override;
	virtual void OnStart() override;
	virtual void OnCompleted() override;
	virtual void Update() override;
	virtual EObjectiveType GetObjectiveType() override { return EObjectiveType::EOT_PodArrival; }

	void ResetPodArrivalTimer();
	void SetWaitForPodTimeInSeconds(float InSecondsToWait) { TimeToWaitForPodPodSeconds = InSecondsToWait; }
	
	FOnWaitForPodEventStart OnWaitForPodEventStart;
	FOnWaitForPodEventEnd OnWaitForPodEventEnd;
	FOnWaitForPodEventPaused OnWaitForPodEventPaused;
	FOnWaitForPodEventUnPaused OnWaitForPodEventUnPaused;
	
private:
	UPROPERTY()
	bool bLandingZoneSearchStarted = false;
	
	UPROPERTY()
	bool bWaitForPodEventPaused = true;
	
	UPROPERTY()
	bool bWaitForPodEventStarted = false;
	
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	USceneComponent* Root;
	
	// Mesh for the LandingZone (or thing to defend)
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	UStaticMeshComponent* MeshLandingZone;

	// If Player goes outside this area the objective will Pause
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	USphereComponent* SphereRestrictiveArea;
	
	// Sphere for the area in which the player must be standing for the quest to activate
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	USphereComponent* SphereInteractArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta=(AllowPrivateAccess=true))
	UAnimationAsset* AnimationPodOpen;
	
	// Timer for the objective
	FTimerHandle TimerHandle;

	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	bool bWaitForPodDone = false;

	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	float TimeToWaitForPodPodSeconds = 10.0f;
	
	UFUNCTION()
	void StartMainPhase(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void OnTimeIsOut();

	UFUNCTION()
	void PauseMainPhase(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void UnPauseMainPhase(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
