// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGSoundManager.generated.h"

class USoundCue;
class UAudioComponent;

UCLASS()
class SPM_TEST_NO_LFS_API ASGSoundManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASGSoundManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* BackgroundMusicCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	UAudioComponent* AudioComponent;
	
};
