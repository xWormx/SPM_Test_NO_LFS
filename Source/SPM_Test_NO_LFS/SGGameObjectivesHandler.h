// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGGameObjectivesHandler.generated.h"

UENUM(BlueprintType)
enum class EObjectiveType : uint8
{
	EOT_KillAllEnemies, EOT_CollectAndPlace, EOT_DefendThePod
};

UCLASS()
class SPM_TEST_NO_LFS_API ASGGameObjectivesHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASGGameObjectivesHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	EObjectiveType CurrentObjective;

	UPROPERTY(EditAnywhere)
	EObjectiveType StartObjective;
};
