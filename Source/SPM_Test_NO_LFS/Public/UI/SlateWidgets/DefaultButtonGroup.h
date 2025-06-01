#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SWidgetData/WidgetConstructionData.h"
#include "Components/Widget.h"
#include "DefaultButtonGroup.generated.h"

class SDefaultButtonGroupWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDefaultButtonGroupWidget)
		: _InButtonGroupData(FButtonGroupData()){}
	SLATE_ARGUMENT(FButtonGroupData, InButtonGroupData)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void SetButtonGroupData(const FButtonGroupData& InButtonGroupData);
	
private:

	int32 ActiveButtonIndex = 0;
	FButtonGroupData ButtonGroupData;
};

UCLASS()
class UDefaultButtonGroupWidget : public UWidget
{
	GENERATED_BODY()
};