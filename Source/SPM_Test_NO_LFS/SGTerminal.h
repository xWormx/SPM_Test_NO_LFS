// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "SGPlayerCharacter.h"
#include "SGTerminal.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API ASGTerminal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASGTerminal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* Capsule;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	TSubclassOf<UUserWidget> HUDTerminalClass;
	
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	UUserWidget* HUDTerminal;
	
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
