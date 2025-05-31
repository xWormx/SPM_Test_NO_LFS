#pragma once

#include "CoreMinimal.h"
#include "ButtonData.generated.h"

USTRUCT(BlueprintType)
struct FAlignmentData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Alignment Data")
	TEnumAsByte<EHorizontalAlignment> HorizontalAlignment = HAlign_Fill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Alignment Data")
	TEnumAsByte<EVerticalAlignment> VerticalAlignment = VAlign_Center;
};

USTRUCT(BlueprintType)
struct FTextData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Data")
	FText Text = FText::FromString("Default Text");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Data")
	FTextBlockStyle TextStyle = FTextBlockStyle();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Data")
	TEnumAsByte<ETextJustify::Type> TextJustification = ETextJustify::Center;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Data")
	float DefaultPadding = 5.f;

	void ApplyStyle(TSharedRef<STextBlock>& TextBlock) const;

	TSharedRef<STextBlock> CreateTextBlock() const;
};

USTRUCT(BlueprintType)
struct FButtonData
{
	GENERATED_BODY()

	FOnClicked OnClicked;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Data")
	FButtonStyle ButtonStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Data")
	FTextData TextData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Data")
	float DefaultPadding = 5.f;

	void ApplyStyle(TSharedRef<SButton>& Button) const;

	TSharedRef<SButton> CreateButton() const;
};

USTRUCT(BlueprintType)
struct FMenuData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Menu Data")
	FTextData TextData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Menu Data")
	TArray<FButtonData> ButtonDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Menu Data")
	float DefaultPadding = 10.f;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Menu Data")
	FSlateBrush BackgroundBrush;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Menu Data")
	FSlateColor BackgroundColor = FSlateColor(FLinearColor::Black);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Menu Data")
	TEnumAsByte<EOrientation> MenuOrientation = Orient_Vertical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Menu Data")
	TEnumAsByte<EOrientation> MenuButtonsOrientation = Orient_Vertical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Menu Data")
	FAlignmentData MenuAlignmentData = {HAlign_Fill, VAlign_Center};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Menu Data")
	FAlignmentData ButtonGroupAlignmentData = {HAlign_Fill, VAlign_Top};

};
