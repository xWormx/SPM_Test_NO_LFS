#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGHUDGrapple.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API USGHUDGrapple : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void PlayValidTargetAnimation();

protected:
	UPROPERTY(BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* ValidTargetAnimation;

private:
	UFUNCTION()
	void OnValidTargetAnimationEnd();

	FWidgetAnimationDynamicEvent ValidTargetAnimationEnd;
};
