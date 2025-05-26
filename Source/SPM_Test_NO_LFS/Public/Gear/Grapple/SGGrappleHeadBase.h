// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGGrappleHeadBase.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API ASGGrappleHeadBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASGGrappleHeadBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetConstraintTo(AActor* OtherActor);
private:
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* GrappleHeadMesh;
	
	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	class UPhysicsConstraintComponent* PhysicsConstraint;
};
