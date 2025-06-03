#pragma once
#include "CoreMinimal.h"
#include "WidgetConstructionData.h"
#include "StylingTheme.generated.h"

USTRUCT()
struct FStylingTheme : public FTableRowBase
{
	GENERATED_BODY()

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
