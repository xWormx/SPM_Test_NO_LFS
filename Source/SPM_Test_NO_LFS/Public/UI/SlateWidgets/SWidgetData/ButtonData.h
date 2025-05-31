#pragma once

#include "CoreMinimal.h"
#include "ButtonData.generated.h"

USTRUCT(BlueprintType)
struct FTextData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Data")
	FText Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Data")
	FName StyleSetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Data")
	TEnumAsByte<ETextJustify::Type> TextJustification = ETextJustify::Center;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Data")
	float DefaultPadding = 5.f;
};

USTRUCT(BlueprintType)
struct FButtonData
{
	GENERATED_BODY()

	FOnClicked OnClicked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Data")
	FText ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Data")
	FName StyleSetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Data")
	FName StyleSetTextName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Data")
	TEnumAsByte<ETextJustify::Type> ButtonTextJustification = ETextJustify::Center;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Button Data")
	float DefaultPadding = 5.f;
};
