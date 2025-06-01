/*

Written by
----------------------
- Carl-Johan Larson Eliasson
- cael7567
	
*/

#pragma once

#include "CoreMinimal.h"
#include "SGObjectiveInterface.h"
#include "SGObjectiveToolTipWidget.h"
#include "GameFramework/Actor.h"
#include "SGObjectiveBase.generated.h"

class USGObjectiveSaveData;
class USGObjectiveHandlerSubSystem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectiveStart, AActor*, Actor);
class ASGGameObjectivesHandler;
class USGObjectiveToolTipWidget;
USTRUCT()
struct FObjectiveBaseSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FString> ObjectiveProgressText;

	UPROPERTY()
	int32 CurrentSubObjectiveStep;
	
	UPROPERTY()
	int32 ObjectiveID;
	
	UPROPERTY()
	FString ObjectiveDescriptionToolTip;
	
	UPROPERTY()
	FString ObjectiveCompletedToolTip;
	
	UPROPERTY()
	FString CurrentSubToolTip;
	
	UPROPERTY()
	TArray<FString> ObjectiveSubToolTips;
};

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
	
	
	virtual void LoadCompleted();
	virtual USGObjectiveSaveData* Save();
	void SaveBaseData(USGObjectiveSaveData* SaveData);
	
	virtual bool IsCompleted(){ return false; }
	virtual void OnStart();
	virtual void OnCompleted();
	virtual void Update();

	virtual void DisplayStartToolTip(USGObjectiveToolTipWidget* ToolTipWidget);
	virtual void DisplayEndToolTip(USGObjectiveToolTipWidget* ToolTipWidget);
	virtual void DisplayToolTip(USGObjectiveToolTipWidget* ToolTipWidget, const FText& ToolTip);

	virtual FText GetObjectiveDescriptionToolTip();
	virtual FText GetObjectiveCompletedToolTip();
	virtual FText GetCurrentSubToolTip();
	virtual EObjectiveType GetObjectiveType() { return EObjectiveType::EOT_None; }


	void SetCurrentProgressElementCompleted(FString InTextCompleted);
	void SetStartDescriptionTooltipText(FString InStartDescriptionTooltipText) { ObjectiveDescriptionToolTip = InStartDescriptionTooltipText; }
	void SetCompletedDescriptionTooltipText(FString InCompletedDescriptionTooltipText) { ObjectiveCompletedToolTip = InCompletedDescriptionTooltipText; }
	
	void AdvanceCurrentObjectiveStep() { CurrentSubObjectiveStep++; }
	
	const int32& GetCurrentProgressStep() const { return CurrentSubObjectiveStep; }

	
	void AddProgressBarText(const TArray<FString>& InProgressBarText) { ObjectiveProgressText = InProgressBarText; }
	void AddSubTooltips(const TArray<FString>& InSubTooltips) { ObjectiveSubToolTips = InSubTooltips; }
	

	FOnObjectiveStart OnObjectiveStart;
	
protected:
		
	UPROPERTY(VisibleAnywhere, Category=UPROPERTY)
	USGObjectiveHandlerSubSystem* ObjectiveHandlerSubSystem;
	
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	TArray<USGHorizontalBoxObjective*> HorizontalBoxProgressElement;

	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	TArray<FString> ObjectiveProgressText;

	
private:
	UPROPERTY(VisibleAnywhere)
	int32 CurrentSubObjectiveStep;

	
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	FString ObjectiveDescriptionToolTip = "Default Objective Description Tooltip!";
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	FString ObjectiveCompletedToolTip = "Default Mission Completed ToolTip!";
	
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	FString CurrentSubToolTip = "Default CurrentSubToolTip!";
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	TArray<FString> ObjectiveSubToolTips;
};

