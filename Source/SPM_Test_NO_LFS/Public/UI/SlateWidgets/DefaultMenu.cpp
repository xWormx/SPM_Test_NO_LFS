#include "DefaultMenu.h"
#include "DefaultButton.h"

void SDefaultMenu::Construct(const FArguments& InArgs)
{
	MenuData = InArgs._InMenuData;

	TSharedRef<SImage> BackgroundImage = SNew(SImage).ColorAndOpacity(MenuData.BackgroundColor);

	TSharedRef<SStackBox> ButtonPanel = SNew(SStackBox).Orientation(MenuData.MenuButtonsOrientation);
	for (FButtonData ButtonData : MenuData.ButtonDataArray)
	{
		ButtonPanel->AddSlot()
		           .HAlign(MenuData.ButtonGroupAlignmentData.HorizontalAlignment)
		           .VAlign(MenuData.ButtonGroupAlignmentData.VerticalAlignment)
		           .Padding(MenuData.DefaultPadding)
				   .AutoSize()
		[
			SNew(SDefaultButtonWidget).InButtonData(ButtonData)
		];
	} //TODO:: Turn into Default Widget (eg. SDefaultButtonGroup)

	ButtonPanel->SetOrientation(MenuData.MenuButtonsOrientation);
	TSharedRef<SStackBox> MenuElements = SNew(SStackBox).Orientation(MenuData.MenuOrientation)
		+ SStackBox::Slot().HAlign(MenuData.MenuAlignmentData.HorizontalAlignment).VAlign(MenuData.MenuAlignmentData.VerticalAlignment).Padding(MenuData.DefaultPadding)
		[
			MenuData.TextData.CreateTextBlock()
		]
		+ SStackBox::Slot().HAlign(MenuData.MenuAlignmentData.HorizontalAlignment).VAlign(MenuData.MenuAlignmentData.VerticalAlignment).Padding(MenuData.DefaultPadding)
	[
		ButtonPanel
	];
	MenuElements->SetOrientation(MenuData.MenuOrientation);

	ChildSlot
	[
		SNew(SOverlay)
			+ SOverlay::Slot().HAlign(MenuData.BackgroundAlignment.HorizontalAlignment).VAlign(MenuData.BackgroundAlignment.VerticalAlignment)
			[
				BackgroundImage
			]
		+ SOverlay::Slot()
		.HAlign(MenuData.MenuAlignmentData.HorizontalAlignment).VAlign(MenuData.MenuAlignmentData.VerticalAlignment).Padding(MenuData.DefaultPadding)
		[
			MenuElements
		]
	];
}

void SDefaultMenu::SetMenuData(const FMenuData& InMenuData)
{
	MenuData = InMenuData;
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
		RebuildWidget();
		//TODO: Change to setters. Need a better way to access individual elements.
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
	if (CustomWidgetInstance)
	{
		RebuildWidget();
	}
}
