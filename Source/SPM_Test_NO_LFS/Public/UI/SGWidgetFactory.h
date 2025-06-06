#pragma once
#include "SlateWidgets/SWidgetData/WidgetConstructionData.h"


class SGWidgetFactory
{
public:

	static FButtonData MenuButton(const FText& ButtonText, const FOnClicked& OnClicked, bool bPrimary = false);

	static FTextData CreateTextData(const FText& Text, const FName StyleName);

	static FTextData CreateHeader(const FText& Text, bool bMenu = true);

	static FBackgroundData Background(const FSlateColor& BackgroundColor, const FAlignmentData& AlignmentData);

	static FButtonGroupData ButtonGroup(const TArray<FButtonData>& ButtonDataArray,const EOrientation Orientation = Orient_Vertical, const FAlignmentData& AlignmentData ={HAlign_Fill, VAlign_Top});

	static FMenuData CreateMenuData(const FText& TextData, const FButtonGroupData& ButtonGroupData,const FBackgroundData& BackgroundData,FAlignmentData MenuAlignmentData);
	static FMenuData CreateMenuData(const FText& TextData, const FButtonGroupData& ButtonGroupData, FAlignmentData MenuAlignmentData = {HAlign_Fill, VAlign_Center});
};
