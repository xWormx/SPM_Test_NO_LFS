// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGObjectiveInterface.h"
#include "SGObjectiveToolTipWidget.h"
#include "GameFramework/Actor.h"
#include "SGObjectiveBase.generated.h"

class USGObjectiveHandlerSubSystem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectiveStart, AActor*, Actor);
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
/*
	virtual bool IsCompleted(ASGGameObjectivesHandler* ObjectiveHandler){ return false; }
	virtual void OnStart(ASGGameObjectivesHandler* ObjectiveHandler);
	virtual void OnCompleted(ASGGameObjectivesHandler* ObjectiveHandler);
	virtual void Update(ASGGameObjectivesHandler* ObjectiveHandler) {}
*/
	virtual bool IsCompleted(){ return false; }
	virtual void OnStart();
	virtual void OnCompleted();
	virtual void Update() {}
	virtual void DisplayStartToolTip(USGObjectiveToolTipWidget* ToolTipWidget);
	virtual void DisplayEndToolTip(USGObjectiveToolTipWidget* ToolTipWidget);
	virtual void DisplayToolTip(USGObjectiveToolTipWidget* ToolTipWidget, const FText& ToolTip);
	virtual FText GetObjectiveDescriptionToolTip();
	virtual FText GetObjectiveCompletedToolTip();
	virtual FText GetCurrentSubToolTip();
	virtual EObjectiveType GetObjectiveType() { return EObjectiveType::EOT_None; }


	void SetObjectiveID(int32 NewObjectiveID) {	ObjectiveID = NewObjectiveID; }
	void SetCurrentProgressText(FString NewCurrentProgressText);
	FString GetCurrentProgressSubText() { return ProgressText.SubText[GetCurrentProgressStep() - 1]; } // för att kunna loopa över första elementet när vi progress step = 1.
	void AdvanceCurrentObjectiveStep() { CurrentSubObjectiveStep++; }
	const int32& GetCurrentProgressStep() const { return CurrentSubObjectiveStep; }
	const int32& GetObjectiveID() const { return ObjectiveID; }
	FProgressText GetProgressText() { return ProgressText; }

	void SetStartDescriptionTooltipText(FString InStartDescriptionTooltipText) { ObjectiveDescriptionToolTip = InStartDescriptionTooltipText; }
	void SetCompletedDescriptionTooltipText(FString InCompletedDescriptionTooltipText) { ObjectiveCompletedToolTip = InCompletedDescriptionTooltipText; }
	void AddProgressBarText(const TArray<FString>& InProgressBarText) { ObjectiveProgressText = InProgressBarText; }
	void AddSubTooltips(const TArray<FString>& InSubTooltips) { ObjectiveSubToolTips = InSubTooltips; }
	
	UPROPERTY(EditAnywhere)
	bool bIsActivated = false;

	FOnObjectiveStart OnObjectiveStart;
	
protected:
	/*
	ASGGameObjectivesHandler* GetObjectiveHandler() { return ObjectiveHandlerPersistent; }
	void SetObjectiveHandler(ASGGameObjectivesHandler* ObjectiveHandler) { ObjectiveHandlerPersistent = ObjectiveHandler; }
*/
		
	UPROPERTY(VisibleAnywhere, Category=UPROPERTY)
	USGObjectiveHandlerSubSystem* ObjectiveHandlerSubSystem;
	
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	TArray<USGHorizontalBoxObjective*> HorizontalBoxProgressElement;

	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	TArray<FString> ObjectiveProgressText;
	
private:
	UPROPERTY(VisibleAnywhere)
	int32 CurrentSubObjectiveStep;
	UPROPERTY(VisibleAnywhere)
	int32 ObjectiveID;
	UPROPERTY(VisibleAnywhere)
	FProgressText ProgressText;

	/*
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	ASGGameObjectivesHandler* ObjectiveHandlerPersistent;
	*/
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	FString ObjectiveDescriptionToolTip = "Default Objective Description Tooltip!";
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	FString ObjectiveCompletedToolTip = "Default Mission Completed ToolTip!";


	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	FString CurrentSubToolTip = "Default CurrentSubToolTip!";
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	TArray<FString> ObjectiveSubToolTips;
	
	

};

