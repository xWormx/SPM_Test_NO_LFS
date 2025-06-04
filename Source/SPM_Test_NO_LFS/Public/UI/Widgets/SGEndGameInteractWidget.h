// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGEndGameInteractWidget.generated.h"

class USizeBox;
/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API USGEndGameInteractWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void ShowEscapeWidget();
	UFUNCTION()
	void HideEscapeWidget();
	
protected:
	
	virtual void NativeConstruct() override;
	

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget), meta=(AllowPrivateAccess=true))
	USizeBox* SizeBoxEscape;
	
};
