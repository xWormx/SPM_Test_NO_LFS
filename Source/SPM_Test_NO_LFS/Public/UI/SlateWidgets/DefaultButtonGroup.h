#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SWidgetData/WidgetConstructionData.h"
#include "Components/Widget.h"
#include "DefaultButtonGroup.generated.h"

class SDefaultButtonWidget;

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

	FButtonGroupData ButtonGroupData;
	TSharedPtr<SStackBox> ButtonGroupStackBox;
	TArray<TSharedPtr<SDefaultButtonWidget>> ButtonWidgets;
};

///⚠️WIP - Not ready for UMG editor. Use SDefaultButtonGroupWidget directly through C++ code instead.
UCLASS()
class UDefaultButtonGroupWidget : public UWidget
{
	GENERATED_BODY()
};
