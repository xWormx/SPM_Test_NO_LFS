// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CableComponent.h"
#include "GrapplingHook.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API AGrapplingHook : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrapplingHook();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FireGrapple();
	void Deactivate();
	bool IsActivated() const { return bIsActivated; }
	FVector GetAttachmentPoint() const { return AttachmentPoint; }
	float GetDragSpeed() const { return DragSpeed; }
private:
	
	UPROPERTY(EditAnywhere)
	UCableComponent* CableComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* GrappleHead;
	
	UPROPERTY(EditAnywhere)
	float MaxHookRange = 1000;

	UPROPERTY(EditAnywhere)
	float DragSpeed = 1000;

	bool bIsActivated = false;
	bool bDidGrapple =false;
	
	FVector AttachmentPoint = FVector::ZeroVector;
	FVector PointOfDeparture = FVector::ZeroVector;
};
