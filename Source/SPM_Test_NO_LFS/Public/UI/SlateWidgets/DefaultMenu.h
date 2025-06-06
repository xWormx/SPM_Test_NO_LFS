#pragma once
#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "SWidgetData/WidgetConstructionData.h"
#include "Widgets/SCompoundWidget.h"
#include "DefaultMenu.generated.h"

class SDefaultButtonGroupWidget;

class SDefaultMenu : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDefaultMenu)
		: _InMenuData(FMenuData())
		{}
		
	SLATE_ARGUMENT(FMenuData, InMenuData)
		
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void SetMenuData(const FMenuData& InMenuData);
	TSharedPtr<SDefaultButtonGroupWidget> GetButtonGroupWidget() const;
private:
	FMenuData MenuData;

	TSharedPtr<SOverlay> RootOverlayPanel;
	TSharedPtr<SStackBox> MenuElementsPanel;

	TSharedPtr<SDefaultButtonGroupWidget> ButtonGroupWidget;
	TSharedPtr<STextBlock> MenuHeaderWidget;

};

///⚠️WIP - Not ready for UMG editor. Use SDefaultMenu directly through C++ code instead.
UCLASS()
class UDefaultMenuWidget : public UWidget
{
	GENERATED_BODY()

public:
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void SynchronizeProperties() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	void SetMenuData(const FMenuData& InMenuData);

protected:
	TSharedPtr<SDefaultMenu> CustomWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty")
	FMenuData MenuData;
};
