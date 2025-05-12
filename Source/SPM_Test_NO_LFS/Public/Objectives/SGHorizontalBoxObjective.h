// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGHorizontalBoxObjective.generated.h"

class UOverlay;
class USizeBox;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API USGHorizontalBoxObjective : public UUserWidget
{
	GENERATED_BODY()
public:
	// Create the necessary variables for your TextBlocks and other widgets
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlockKey;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlockValue;

	UPROPERTY(meta = (BindWidget))
	UImage* ImageBackGround;

	UPROPERTY(meta = (BindWidget))
	UImage* ImageSucceed;

	UPROPERTY(meta = (BindWidget))
	UImage* ImageFail;

	void SetKeyValue(FText KeyText, FText ValueText);
	void SetKey(FText KeyText);
	void SetValue(FText ValueText);
	void SetKeyOpacity(float Opacity);
	void SetValueOpacity(float Opacity);
	void SetKeyAndValueOpacity(float Opacity);
	void SetImage(UTexture2D* NewTexture);
	void ShowBackground();
	void HideBackground();
	void ShowSucceed();
	void HideSucceed();
	void ShowFail();
	void HideFail();
	
private:
};


