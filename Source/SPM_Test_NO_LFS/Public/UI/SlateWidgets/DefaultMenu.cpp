#include "DefaultMenu.h"
#include "DefaultButton.h"
#include "DefaultButtonGroup.h"
#include "SPM_Test_NO_LFS.h"

void SDefaultMenu::Construct(const FArguments& InArgs)
{
	MenuData = InArgs._InMenuData;

	ChildSlot
	[
		SNew(SOverlay)
	];

	SetMenuData(MenuData);
}

void SDefaultMenu::SetMenuData(const FMenuData& InMenuData)
{
	MenuData = InMenuData;

	EHorizontalAlignment HorizontalAlignment = MenuData.MenuAlignmentData.HorizontalAlignment;
	EVerticalAlignment VerticalAlignment = MenuData.MenuAlignmentData.VerticalAlignment;
	float Padding = MenuData.DefaultPadding;

	if (ChildSlot.GetWidget()->GetType() != "SOverlay")
	{
		return;
	}

	TSharedRef<SOverlay> Overlay = StaticCastSharedRef<SOverlay>(ChildSlot.GetWidget());
	if (Overlay->GetChildren()->Num() == 0)
	{
		EMMA_LOG(Warning, TEXT("SDefaultMenu::SetMenuData: Overlay has no children, creating new SStackBox."));
		TSharedRef<SStackBox> MenuElements = SNew(SStackBox).Orientation(MenuData.MenuOrientation)
			+ SStackBox::Slot().HAlign(HorizontalAlignment).VAlign(VerticalAlignment).Padding(Padding)
			[
				MenuData.TextData.CreateTextBlock()
			]
			+ SStackBox::Slot().HAlign(HorizontalAlignment).VAlign(VerticalAlignment).Padding(Padding)
			[
				SNew(SDefaultButtonGroupWidget).InButtonGroupData(MenuData.ButtonGroupData)
			];

		Overlay->AddSlot()
		       .HAlign(MenuData.BackgroundAlignment.HorizontalAlignment).VAlign(MenuData.BackgroundAlignment.VerticalAlignment)
		[
			SNew(SImage).ColorAndOpacity(MenuData.BackgroundColor)
		];
		Overlay->AddSlot().HAlign(HorizontalAlignment).VAlign(VerticalAlignment).Padding(Padding)
		[
			MenuElements
		];
		MenuElements->SetOrientation(MenuData.MenuOrientation);
	}
	else
	{
		EMMA_LOG(Warning,TEXT("SDefaultMenu::SetMenuData: Overlay already has children, updating existing SStackBox."));
		constexpr int32 BackgroundIndex = 0; // Assuming the first child is the SImage
		constexpr int32 StackBoxIndex = 1; // Assuming the second child is the SStackBox

		if (Overlay->GetChildren()->Num() < StackBoxIndex || Overlay->GetChildren()->GetChildAt(StackBoxIndex)->GetType() != "SStackBox")
		{
			EMMA_LOG(Warning, TEXT("SDefaultMenu::SetMenuData: Overlay does not have a valid SStackBox at index %d."),StackBoxIndex);
			return;
		}

		TSharedRef<SImage> BackgroundImage = StaticCastSharedRef<SImage>(Overlay->GetChildren()->GetChildAt(BackgroundIndex));
		Overlay->Slot(BackgroundIndex).HAlign(MenuData.BackgroundAlignment.HorizontalAlignment).VAlign(MenuData.BackgroundAlignment.VerticalAlignment).Padding(Padding);
		BackgroundImage->SetColorAndOpacity(MenuData.BackgroundColor);

		TSharedRef<SStackBox> MenuElements = StaticCastSharedRef<SStackBox>(Overlay->GetChildren()->GetChildAt(StackBoxIndex));
		MenuElements->Slot().HAlign(HorizontalAlignment).VAlign(VerticalAlignment).Padding(Padding);
		
		TSharedRef<STextBlock> MenuHeader = StaticCastSharedRef<STextBlock>(MenuElements->GetChildren()->GetChildAt(0));
		MenuData.TextData.ApplyStyle(MenuHeader);

		TSharedRef<SDefaultButtonGroupWidget> ButtonGroupWidget = StaticCastSharedRef<SDefaultButtonGroupWidget>(MenuElements->GetChildren()->GetChildAt(1));
		ButtonGroupWidget->SetButtonGroupData(MenuData.ButtonGroupData);
		MenuElements->SetOrientation(MenuData.MenuOrientation);
	}
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
	SynchronizeProperties();
}
