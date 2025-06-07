/*

Written by
----------------------
- Carl-Johan Larson Eliasson
- cael7567
	
*/

#pragma once

#include "CoreMinimal.h"
#include "SGObjectiveBase.h"
#include "SGObjectiveDefendThePod.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDefendEventStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDefendEventEnd, UObject*, ObjectiveInterfaceImplementor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDefendEventPaused);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDefendEventUnPaused);

class USphereComponent;
/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API ASGObjectiveDefendThePod : public ASGObjectiveBase, public ISGObjectiveInterface
{
	GENERATED_BODY()
	
public:
	
	ASGObjectiveDefendThePod();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual bool IsCompleted() override;
	virtual void OnStart() override;
	virtual void OnCompleted() override;
	virtual void Update() override;
	virtual EObjectiveType GetObjectiveType() { return EObjectiveType::EOT_DefendThePod; }

	FOnDefendEventStart OnDefendEventStart;
	FOnDefendEventEnd OnDefendEventEnd;
	FOnDefendEventPaused OnDefendEventPaused;
	FOnDefendEventUnPaused OnDefendEventUnPaused;
private:

	UPROPERTY()
	bool bDefendEventPaused = true;
	
	UPROPERTY()
	bool bDefendEventStarted = false;
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	USceneComponent* Root;
	
	// Mesh for the Pod (or thing to defend) - OR Should this be a separat class with HP and things, with TSubclassOf<>?
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	UStaticMeshComponent* MeshToDefend;

	// Are the follwing 2 needed?
	// Mesh for the Pod (or thing to defend)
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	UStaticMeshComponent* MeshRestrictiveFloor;

	// If Player goes outside this area the objective will Pause
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	USphereComponent* SphereRestrictiveArea;
	
	// Sphere for the area in which the player must be standing for the quest to activate
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	USphereComponent* SphereInteractArea;
	
	// Timer for the objective
	FTimerHandle TimerHandle;

	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	bool bDefendedThePod = false;

	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	float TimeToDefendPodSeconds = 10.0f;
	
	UFUNCTION()
	void StartMainPhase(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnTimeIsOut();

	UFUNCTION()
	void PauseMainPhase(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void UnPauseMainPhase(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
