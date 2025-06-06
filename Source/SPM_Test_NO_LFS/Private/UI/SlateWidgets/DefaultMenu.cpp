#include "../../../Public/UI/SlateWidgets/DefaultMenu.h"
#include "../../../Public/UI/SlateWidgets/DefaultButton.h"
#include "../../../Public/UI/SlateWidgets/DefaultButtonGroup.h"
#include "SPM_Test_NO_LFS.h"

void SDefaultMenu::Construct(const FArguments& InArgs)
{
	MenuData = InArgs._InMenuData;

	MenuHeaderWidget = MenuData.HeaderElement.CreateTextBlock();
	ButtonGroupWidget = SNew(SDefaultButtonGroupWidget).InButtonGroupData(MenuData.ButtonGroupElement);

	TEnumAsByte<EHorizontalAlignment> InHAlignment = MenuData.ElementAlignment.Horizontal;
	TEnumAsByte<EVerticalAlignment> InVAlignment = MenuData.ElementAlignment.Vertical;
	FMargin InPadding = MenuData.ElementMargin;

	MenuElementsPanel = SNew(SStackBox)
		+ SStackBox::Slot()
		.HAlign(InHAlignment)
		.VAlign(InVAlignment)
		.Padding(InPadding)
		[
			MenuHeaderWidget.ToSharedRef()
		]
		+ SStackBox::Slot()
		.HAlign(InHAlignment)
		.VAlign(InVAlignment)
		.Padding(InPadding)
		[
			ButtonGroupWidget.ToSharedRef()
		];

	MenuElementsPanel->SetOrientation(MenuData.ElementOrientation);

	RootOverlayPanel = MenuData.BackgroundData.CreateBackgroundOverlay();
	RootOverlayPanel->AddSlot()
	                .HAlign(InHAlignment)
	                .VAlign(InVAlignment)
	                .Padding(InPadding)
	[
		MenuElementsPanel.ToSharedRef()
	];

	ChildSlot
	[
		RootOverlayPanel.ToSharedRef()
	];

	//SetMenuData(MenuData);
}

void SDefaultMenu::SetMenuData(const FMenuData& InMenuData)
{
	MenuData = InMenuData;

	TSharedRef<SOverlay> RootOverlayPanelRef = RootOverlayPanel.ToSharedRef();
	MenuData.BackgroundData.ApplyStyle(RootOverlayPanelRef);

	TSharedRef<STextBlock> HeaderRef = MenuHeaderWidget.ToSharedRef();
	MenuData.HeaderElement.ApplyStyle(HeaderRef);

	ButtonGroupWidget->SetButtonGroupData(MenuData.ButtonGroupElement);

	for (int32 SlotIndex = 0; SlotIndex < MenuElementsPanel->GetChildren()->Num(); SlotIndex++)
	{
		SStackBox::FSlot& CurrentSlot = MenuElementsPanel->GetSlot(SlotIndex);
		CurrentSlot.SetHorizontalAlignment(MenuData.ElementAlignment.Horizontal);
		CurrentSlot.SetVerticalAlignment(MenuData.ElementAlignment.Vertical);
		CurrentSlot.SetPadding(MenuData.ElementMargin);
	}
}

TSharedPtr<SDefaultButtonGroupWidget> SDefaultMenu::GetButtonGroupWidget() const
{
	return ButtonGroupWidget;
}

void UDefaultMenuWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	CustomWidgetInstance.Reset();
}

void UDefaultMenuWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (CustomWidgetInstance)
	{
		CustomWidgetInstance->SetMenuData(MenuData);
	}
}

TSharedRef<SWidget> UDefaultMenuWidget::RebuildWidget()
{
	CustomWidgetInstance = SNew(SDefaultMenu).InMenuData(MenuData);
	return CustomWidgetInstance.ToSharedRef();
}

void UDefaultMenuWidget::SetMenuData(const FMenuData& InMenuData)
{
	MenuData = InMenuData;
	SynchronizeProperties();
}
