#pragma once
#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "SWidgetData/WidgetConstructionData.h"
#include "Widgets/SCompoundWidget.h"
#include "DefaultButton.generated.h"
class UTextBlock;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClicked);

class SDefaultButtonWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDefaultButtonWidget)
		: _InButtonData(FButtonData()){}
	SLATE_ARGUMENT(FButtonData, InButtonData)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void SetButtonData(const FButtonData& InButtonData);

	TSharedPtr<SButton> GetButtonWidget() const;
	TSharedPtr<STextBlock> GetButtonTextWidget() const;

private:
	FButtonData ButtonData;
	TSharedPtr<SButton> ButtonWidget;
	TSharedPtr<STextBlock> ButtonTextWidget;

};

UCLASS()
class UDefaultButtonWidget : public UWidget
{
	GENERATED_BODY()

public:

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void SynchronizeProperties() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	void SetButtonData(const FButtonData& InButtonData);

	FOnButtonClicked OnButtonClicked;
protected:
	TSharedPtr<SDefaultButtonWidget> CustomWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty")
	FButtonData ButtonData;

};
