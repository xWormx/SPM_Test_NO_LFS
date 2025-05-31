
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGMainHUDWidget.generated.h"

class USGDifficultyBarWidget;
class USGHUDGrapple;
class USGWeaponsHUD;

UCLASS()
class SPM_TEST_NO_LFS_API USGMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void PauseAndHide();
	void PlayAndShow();
	
	USGWeaponsHUD* GetWeaponsWidget() const
	{
		return WeaponsWidget.Get();
	}
	USGHUDGrapple* GetGrappleHookWidget() const
	{
		return GrappleHookWidget.Get();
	}
	USGDifficultyBarWidget* GetDifficultyWidget() const
	{
		return DifficultyWidget.Get();
	}

	void StartDifficultyBar();
protected:
	UPROPERTY(BlueprintReadOnly, Category ="Uproperty - HUD", meta=(BindWidget))
	TWeakObjectPtr<USGWeaponsHUD> WeaponsWidget;

	UPROPERTY(BlueprintReadOnly, Category ="Uproperty - HUD", meta=(BindWidget))
	TWeakObjectPtr<USGHUDGrapple> GrappleHookWidget;

	UPROPERTY(BlueprintReadOnly, Category ="Uproperty - HUD", meta=(BindWidget))
	TWeakObjectPtr<USGDifficultyBarWidget> DifficultyWidget;

private:
	bool HasFirstQuestStarted = false;
};
