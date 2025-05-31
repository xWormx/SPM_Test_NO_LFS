#pragma once
#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "SWidgetData/ButtonData.h"
#include "Widgets/SCompoundWidget.h"
#include "DefaultMenu.generated.h"


class SDefaultMenu : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDefaultMenu)
		: _InTextData(FTextData())
		, _InButtonDataArray(TArray<FButtonData>())
		{}
		
	SLATE_ARGUMENT(FTextData, InTextData)
	SLATE_ARGUMENT(TArray<FButtonData> , InButtonDataArray)
		
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void SetTextData(const FTextData& InTextData);
	void SetButtonDataArray(const TArray<FButtonData>& InButtonDataArray);

private:
	FTextData TextData;
	TArray<FButtonData> ButtonDataArray;	
};

/*
UCLASS()
class UDefaultMenuWidget : public UWidget
{
	GENERATED_BODY()

public:

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void SynchronizeProperties() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	void SetButtonDataArray(const TArray<FButtonData>& InButtonDataArray);

protected:
	TSharedPtr<SDefaultMenu> CustomWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty")
	FTextData TextData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty")
	TArray<FButtonData> ButtonDataArray;
};
*/
