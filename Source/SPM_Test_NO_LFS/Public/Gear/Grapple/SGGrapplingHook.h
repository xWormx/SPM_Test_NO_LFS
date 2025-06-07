/*

Written by
----------------------
- Carl-Johan Larson Eliasson
- cael7567
	
*/

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CableComponent.h"
#include "SGGrapplingHook.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireGrapple);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCanGrapple, bool, bCanGrapple);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginCooldown, FTimerHandle&, GrappleTimerHandle);

class USGHUDGrapple;

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
	void SetMaxHookRange(float NewMaxHookRange) {	MaxHookRange = NewMaxHookRange; }
	void SetCoolDown(float NewCoolDown) { HookCooldown = NewCoolDown; }
	void EnableGrappling();
	void DisableGrappling();
	//void InitializeHUDGrapple();
	bool CanGrapple() const { return bCanGrapple; }
	bool HeadAttached() const { return bHeadAttached; }
	bool PlayerWantToTravel() const { return bStartTravel; }
	FVector GetAttachmentPoint() const { return AttachmentPoint; }
	FVector GetGrappleDirectionNormalized() const { return GrappleDirection; }
	float GetDragSpeed() const { return DragSpeed; }
	
private:

	UPROPERTY(VisibleAnywhere, Category = "UPROPERTY")
	USGHUDGrapple* HUDGrapple;
	
	UPROPERTY(EditAnywhere, Category = "UPROPERTY")
	USoundBase* GrappleFireSound;
	UPROPERTY(EditAnywhere, Category = "UPROPERTY")
	USoundBase* GrappleAttachmentSound;
	UPROPERTY(EditAnywhere, Category = "UPROPERTY")
	UParticleSystem* GrappleAttachmentParticles;
	
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

	/* If grappledirection is upwards z > 0, this is the extra impuls on Z-axis*/
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	float ExtraUpwardsImpuls = 10;

	/* Distance to the grapple point when the Grapplehook should reset and the Player released in the direction its getting dragged */
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	float GrappleReleaseDistance = 150;
	
	/* When player arrives at the attachment point, this impuls is added to the player */
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	float ImpulsAtArrival = 100000;

	/* The speed with which the grapplehead travels towards the target attechment point.*/
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	float HeadTravelSpeed = 10;
	
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	float HookCooldown = 3;

	FTimerHandle GrappleTimerHandle;
	
	float CableLengthWhenAttached = 0;

	bool bHUDGrappleInitialized = false;
	bool bHeadAttached = false;
	bool bDidGrapple = false;
	bool bCanGrapple = true;
	bool bStartTravel = false;

	FVector AttachmentPoint = FVector::ZeroVector;
	FVector PointOfDeparture = FVector::ZeroVector;
	FVector GrappleDirection = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere)
	AActor* GrappledActor;
	
	AController* GetValidController() const;
	bool GrappleTrace(FHitResult& OutHitResult, AController* Controller);
	void StartCharacterLaunch(ACharacter* Character);
	void UpdatePlayerPosition(ACharacter* Character, float DeltaTime);
	void UpdatePlayerPositionVelocityBased(ACharacter* Character, float DeltaTime);
	void UpdatePlayerPositionLocationBased(ACharacter* Character, float DeltaTime);
	void ResetAndLaunch(ACharacter* Character, float DeltaTime);
	bool AttachGrapple(AController* Controller,FHitResult& HitResult);
	void TravelDirectly(ACharacter* Character, FHitResult& HitResult);

public:
	FOnCanGrapple OnCanGrapple;
	FOnFireGrapple OnFireGrapple;
	FOnBeginCooldown OnBeginCooldown;
};
