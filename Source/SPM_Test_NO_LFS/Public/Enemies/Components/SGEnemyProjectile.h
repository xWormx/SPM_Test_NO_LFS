// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGEnemyProjectile.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API ASGEnemyProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASGEnemyProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override; 

	UFUNCTION()
	void OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							 bool bFromSweep, const FHitResult& SweepResult);

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpeed = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifeSpan = 3.0f;
};
