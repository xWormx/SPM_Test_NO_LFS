#include "UI/SGWidgetFactory.h"

#include "UI/SlateWidgets/SWidgetData/StyleSetData.h"
#include "UI/SlateWidgets/SWidgetData/WidgetConstructionData.h"

FButtonData SGWidgetFactory::MenuButton(const FText& ButtonText, const FOnClicked& OnClicked, const bool bPrimary)
{
	FName ButtonStyleName = bPrimary ? StyleNames::MenuPrimaryButton() : StyleNames::MenuSecondaryButton();
	FName TextStyleName = bPrimary ? StyleNames::MenuPrimaryButtonText() : StyleNames::MenuSecondaryButtonText();

	FButtonData Button = FButtonData();
	Button.OnClicked = OnClicked;
	Button.ButtonStyle = FStyleSetData::Get().GetWidgetStyle<FButtonStyle>(ButtonStyleName);
	Button.TextData = CreateTextData(ButtonText, TextStyleName);

	return Button;
}

FTextData SGWidgetFactory::CreateTextData(const FText& Text, const FName StyleName)
{
	FTextData TextData;
	TextData.Text = Text;
	TextData.TextStyle = FStyleSetData::Get().GetWidgetStyle<FTextBlockStyle>(StyleName);
	return TextData;
}

FTextData SGWidgetFactory::CreateHeader(const FText& Text, [[maybe_unused]] bool bMenu)
{
	return CreateTextData(Text, StyleNames::MenuHeaderText());
}

FBackgroundData SGWidgetFactory::Background(const FSlateColor& BackgroundColor, const FAlignmentData& AlignmentData)
{
	FBackgroundData BackgroundData;
	BackgroundData.BackgroundColor = BackgroundColor;
	BackgroundData.BackgroundAlignment = AlignmentData;
	return BackgroundData;
}

FButtonGroupData SGWidgetFactory::ButtonGroup(const TArray<FButtonData>& ButtonDataArray, const EOrientation Orientation, const FAlignmentData& AlignmentData)
{
	FButtonGroupData ButtonGroupData;
	ButtonGroupData.ButtonDataArray = ButtonDataArray;
	ButtonGroupData.Orientation = Orientation;
	ButtonGroupData.Alignment = AlignmentData;
	return ButtonGroupData;
}

FMenuData SGWidgetFactory::CreateMenuData(const FText& TextData, const FButtonGroupData& ButtonGroupData,const FAlignmentData MenuAlignmentData)
{
	FMenuData MenuData;
	MenuData.HeaderElement =  CreateHeader(TextData);
	MenuData.ButtonGroupElement = ButtonGroupData;
	MenuData.ElementAlignment = MenuAlignmentData;
	return MenuData;
}
