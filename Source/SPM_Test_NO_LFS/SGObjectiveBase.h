// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGObjectiveBase.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API ASGObjectiveBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASGObjectiveBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void ActivateObjective();
	virtual bool CheckProgress(){ return false; }
	virtual void Update(){}
	virtual FText GetToolTipText();
	UPROPERTY(EditAnywhere)
	bool bIsActivated = false;

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	FString ObjectiveToolTip;


};
