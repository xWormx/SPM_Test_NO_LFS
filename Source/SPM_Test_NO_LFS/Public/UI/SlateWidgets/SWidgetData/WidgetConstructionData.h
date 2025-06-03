#pragma once

#include "CoreMinimal.h"
#include "WidgetConstructionData.generated.h"

USTRUCT(BlueprintType)
struct FAlignmentData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Alignment Data")
	TEnumAsByte<EHorizontalAlignment> Horizontal = HAlign_Fill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Alignment Data")
	TEnumAsByte<EVerticalAlignment> Vertical = VAlign_Center;
};

USTRUCT(BlueprintType)
struct FTextData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Text Data")
	FText Text = FText::FromString("Default Text");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Text Data")
	FTextBlockStyle TextStyle = FTextBlockStyle();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Text Data")
	TEnumAsByte<ETextJustify::Type> TextJustification = ETextJustify::Center;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Text Data")
	FMargin Margin = FMargin(5.f);

	void ApplyStyle(TSharedRef<STextBlock>& TextBlock) const;

	TSharedRef<STextBlock> CreateTextBlock() const;
};

USTRUCT(BlueprintType)
struct FButtonData
{
	GENERATED_BODY()

	FOnClicked OnClicked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Data")
	FButtonStyle ButtonStyle = FButtonStyle();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Data")
	FTextData TextData = FTextData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Data")
	FMargin Margin = FMargin(5.f);

	void ApplyStyle(TSharedRef<SButton>& Button) const;

	TSharedRef<SButton> CreateButton() const;
};

USTRUCT(BlueprintType)
struct FBackgroundData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Background Data")
	FAlignmentData BackgroundAlignment = {HAlign_Fill, VAlign_Fill};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Background Data")
	FSlateColor BackgroundColor = FSlateColor(FLinearColor::Black);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Background Data")
	FSlateBrush BackgroundBrush = FSlateBrush();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Background Data")
	FMargin Margin = FMargin(0.f);

	void ApplyStyle(TSharedRef<SOverlay>& Overlay) const;
	TSharedRef<SOverlay>CreateBackgroundOverlay() const;
	TSharedRef<SImage>CreateBackgroundImage() const;
};

USTRUCT(BlueprintType)
struct FButtonGroupData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Group Data")
	TArray<FButtonData> ButtonDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Group Data")
	FMargin Margin = FMargin(20.f, 10.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Group Data")
	TEnumAsByte<EOrientation> Orientation = Orient_Vertical;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Group Data")
	FAlignmentData Alignment = {HAlign_Fill, VAlign_Top};

};

USTRUCT(BlueprintType)
struct FMenuData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Menu Data")
	FTextData HeaderElement = FTextData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Menu Data")
	FButtonGroupData ButtonGroupElement = FButtonGroupData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Menu Data")
	FBackgroundData BackgroundData = FBackgroundData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Menu Data")
	FMargin ElementMargin = FMargin(10.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Menu Data")
	TEnumAsByte<EOrientation> ElementOrientation = Orient_Vertical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Menu Data")
	FAlignmentData ElementAlignment = {HAlign_Fill, VAlign_Center};
};
