// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SGObjectiveInterface.h"
#include "GameFramework/Actor.h"
#include "SGObjectiveBase.generated.h"

class ASGGameObjectivesHandler;
class USGObjectiveToolTipWidget;
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
	virtual bool IsCompleted(ASGGameObjectivesHandler* ObjectivesHandler){ return false; }
	virtual void OnStart(ASGGameObjectivesHandler* ObjectiveHandler);
	virtual void OnCompleted(ASGGameObjectivesHandler* ObjectiveHandler);
	virtual void Update(ASGGameObjectivesHandler* ObjectiveHandler) {}
	virtual void DisplayStartToolTip(USGObjectiveToolTipWidget* ToolTipWidget);
	virtual void DisplayEndToolTip(USGObjectiveToolTipWidget* ToolTipWidget);
	virtual void DisplayToolTip(USGObjectiveToolTipWidget* ToolTipWidget, const FText& ToolTip);
	virtual FText GetObjectiveDescriptionToolTip();
	virtual FText GetObjectiveCompletedToolTip();
	virtual FText GetCurrentSubToolTip();
	virtual EObjectiveType GetObjectiveType() { return EObjectiveType::EOT_InvalidObjectiveType; }
	
	UPROPERTY(EditAnywhere)
	bool bIsActivated = false;
protected:
	ASGGameObjectivesHandler* GetObjectiveHandler() { return ObjectiveHandlerPersistent; }
	void SetObjectiveHandler(ASGGameObjectivesHandler* ObjectiveHandler) { ObjectiveHandlerPersistent = ObjectiveHandler; }
	
private:
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	ASGGameObjectivesHandler* ObjectiveHandlerPersistent;
	
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	FString ObjectiveDescriptionToolTip = "Default Objective Description Tooltip!";
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	FString ObjectiveCompletedToolTip = "Default Mission Completed ToolTip!";

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	FString CurrentSubToolTip = "Default CurrentSubToolTip!";
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	TArray<FString> ObjectiveSubToolTips;
	


};
