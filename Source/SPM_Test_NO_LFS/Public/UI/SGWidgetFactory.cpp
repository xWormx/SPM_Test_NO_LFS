#include "SGWidgetFactory.h"
#include "SlateWidgets/SWidgetData/StyleSetData.h"

FButtonData SGWidgetFactory::MenuButton(const FText& ButtonText, const FOnClicked& OnClicked, const bool bPrimary)
{
	FName ButtonStyleName = bPrimary ? StyleNames::MenuPrimaryButton() : StyleNames::MenuButton();
	FName TextStyleName = bPrimary ? StyleNames::MenuPrimaryButtonText() : StyleNames::MenuButtonText();

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

FTextData SGWidgetFactory::CreateHeader(const FText& Text, bool bMenu)
{
	FName StyleName = StyleNames::MenuHeaderText();
	FTextData TextData;
	TextData.Text = Text;
	TextData.TextStyle = FStyleSetData::Get().GetWidgetStyle<FTextBlockStyle>(StyleName);
	return TextData;
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

FMenuData SGWidgetFactory::CreateMenuData(const FText& TextData, const FButtonGroupData& ButtonGroupData, const FBackgroundData& BackgroundData, const FAlignmentData MenuAlignmentData)
{
	FMenuData MenuData;
	MenuData.HeaderElement =  CreateHeader(TextData);
	MenuData.ButtonGroupElement = ButtonGroupData;
	MenuData.BackgroundData = BackgroundData;
	MenuData.ElementAlignment = MenuAlignmentData;
	return MenuData;
}
