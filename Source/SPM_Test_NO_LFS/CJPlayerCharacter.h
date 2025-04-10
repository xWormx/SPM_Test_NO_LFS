// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CJPlayerCharacter.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API ACJPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACJPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	float GetRage() const { return Rage; };
	void FireGrapple();
	
private:
	float Rage = 66.f;
	bool DidGrapple = false;
	FVector GrappleLocation = FVector::ZeroVector;
	FVector GrappleStartLocation = FVector::ZeroVector;
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	USceneComponent* GrapplingHookPosition;
	
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	TSubclassOf<class AGrapplingHook> GrapplingHookClass;

	UPROPERTY()
	AGrapplingHook* GrapplingHook;

	
};
