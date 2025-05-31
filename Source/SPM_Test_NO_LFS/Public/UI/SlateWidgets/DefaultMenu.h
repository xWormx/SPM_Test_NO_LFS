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
		: _InMenuData(FMenuData())
		{}
		
	SLATE_ARGUMENT(FMenuData, InMenuData)
		
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void SetMenuData(const FMenuData& InMenuData);

private:
	FMenuData MenuData;
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
