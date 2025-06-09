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
	ASGSoundManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* BackgroundMusicCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	UAudioComponent* AudioComponent;
	
};
