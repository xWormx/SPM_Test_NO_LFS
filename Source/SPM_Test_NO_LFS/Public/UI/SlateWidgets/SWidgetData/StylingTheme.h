#pragma once
#include "CoreMinimal.h"
#include "WidgetConstructionData.h"
#include "Slate/SlateBrushAsset.h"
#include "StylingTheme.generated.h"

USTRUCT(BlueprintType)
struct FStylingIcon
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName IconName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USlateBrushAsset* Icons;
};

USTRUCT()
struct FStylingTheme : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icons")
	TArray<FStylingIcon> StylingIcons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Elements")
	FTextBlockStyle Header;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Elements")
	FTextBlockStyle Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Elements")
	FTextBlockStyle PrimaryButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Elements")
	FTextBlockStyle SecondaryButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons")
	FButtonStyle PrimaryButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons")
	FButtonStyle SecondaryButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Background")
	FBackgroundData StartMenuBackground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Background")
	FBackgroundData PauseMenuBackground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Background")
	FBackgroundData GameOverMenuBackground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Background")
	FBackgroundData VictoryMenuBackground;
};
