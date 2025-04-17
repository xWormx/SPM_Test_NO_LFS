// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CableComponent.h"
#include "HeadMountedDisplayTypes.h"
#include "SGGrapplingHook.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API ASGGrapplingHook : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASGGrapplingHook();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FireGrapple();
	void ResetGrapple();
	void SetGrappleVisibility(bool bVisibility);
	void SetHeadConstraint(AActor* OtherActor);
	void EnableGrappling();
	void DisableGrappling();
	bool CanGrapple() const { return bCanGrapple; }
	bool HeadAttached() const { return bHeadAttached; }
	FVector GetAttachmentPoint() const { return AttachmentPoint; }
	FVector GetGrappleDirectionNormalized() const { return GrappleDirection; }
	float GetDragSpeed() const { return DragSpeed; }
	
	
private:

	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	
	UPROPERTY(EditDefaultsOnly)
	UCableComponent* CableComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASGGrappleHeadBase> GrappleHeadClass;

	UPROPERTY()
	ASGGrappleHeadBase* Head;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* GrappleHeadPosition;
	
	/* Maximum range that the hook can hit */
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	float MaxHookRange = 1000;

	/* Speed at which the character drags towards the grapple attachpoint */
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	float DragSpeed = 1000;

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	float HookCooldown = 3;

	FTimerHandle GrappleTimerHandle;

	float CableLengthWhenAttached = 0;

	bool bHeadAttached = false;
	bool bDidGrapple = false;
	bool bCanGrapple = true;
	
	FVector AttachmentPoint = FVector::ZeroVector;
	FVector PointOfDeparture = FVector::ZeroVector;
	FVector GrappleDirection = FVector::ZeroVector;

	
	AController* GetValidController() const;
	bool GrappleTrace(FHitResult& OutHitResult, AController* Controller);
	void StartCharacterLaunch(ACharacter* Character);
};
