// Fill out your copyright notice in the Description page of Project Settings.
// comment
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SGOpenAtObjectiveCompleteComponent.generated.h"


enum class EObjectiveType : uint8;
class ASGGameObjectivesHandler;
class ASGObjectiveBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPM_TEST_NO_LFS_API USGOpenAtObjectiveCompleteComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USGOpenAtObjectiveCompleteComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	
	UPROPERTY(EditAnywhere, Category=UPROPERTY)
	EObjectiveType ObjectiveTypeToWatch;

	UPROPERTY(EditAnywhere, Category=UPROPERTY)
	USoundBase* SoundOpen;
	
	UFUNCTION()
	void Open(EObjectiveType ObjectiveType);
};
