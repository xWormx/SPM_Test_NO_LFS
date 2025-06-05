// Joel Larsson Wendt || jola6902

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGMusicPlayer.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API ASGMusicPlayer : public AActor
{
	GENERATED_BODY()
	
public:	
	ASGMusicPlayer();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Music_ObjectiveStarted(EObjectiveType ObjectiveType);
	
	UFUNCTION()
	void Music_ObjectiveCompleted(EObjectiveType ObjectiveType);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="editor",meta=(AllowPrivateAccess = "true"))
	USoundBase* BackdropMusic;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="editor",meta=(AllowPrivateAccess = "true"))
	USoundBase* MainThemeMusic;
	
	UAudioComponent* BackdropAudioComponent;
	UAudioComponent* MainThemeAudioComponent;

	FTimerHandle BackdropMusicFadeOutTimerHandle;
	FTimerHandle BackdropMusicFadeInTimerHandle;
	FTimerHandle MainThemeMusicFadeOutTimerHandle;
	FTimerHandle MainThemeMusicFadeInTimerHandle;
	
	FTimerHandle RockNRollDelayTimerHandle;

	void FadeOutBackdrop();
	void UpdateFadeOutBackdrop();
	
	void FadeOutMainTheme();
	void UpdateFadeOutMainTheme();

	void FadeInBackdrop();
	void UpdateFadeInBackdrop();

	void FadeInMainTheme();
	void UpdateFadeInMainTheme();
};
