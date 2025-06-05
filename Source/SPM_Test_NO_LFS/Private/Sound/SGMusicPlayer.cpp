// Joel Larsson Wendt || jola6902

#include "Sound/SGMusicPlayer.h"
#include "Components/AudioComponent.h"
#include "Core/SGObjectiveHandlerSubSystem.h"

ASGMusicPlayer::ASGMusicPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASGMusicPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASGMusicPlayer::Music_ObjectiveStarted(EObjectiveType ObjectiveType)
{
	// Fade out BackdropMusic
	// Fade in MainThemeMusic
	if (BackdropAudioComponent && MainThemeAudioComponent)
	{
		GetWorldTimerManager().ClearTimer(RockNRollDelayTimerHandle);
		GetWorldTimerManager().SetTimer(RockNRollDelayTimerHandle, this, &ASGMusicPlayer::FadeInMainTheme, 5.00f, false);
	}
}

void ASGMusicPlayer::Music_ObjectiveCompleted(EObjectiveType ObjectiveType)
{
	// Fade in BackdropMusic
	// Fade out MainThemeMusic
	if (BackdropAudioComponent && MainThemeAudioComponent)
	{
		FadeOutMainTheme();
		FadeInBackdrop();
	}
}

void ASGMusicPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	BackdropAudioComponent = NewObject<UAudioComponent>(this);
	BackdropAudioComponent->RegisterComponent();
	BackdropAudioComponent->SetSound(BackdropMusic);
	BackdropAudioComponent->SetVolumeMultiplier(1.0f);
	BackdropAudioComponent->bAutoActivate = false;
	BackdropAudioComponent->bIsUISound = true;
	BackdropAudioComponent->bAllowSpatialization = false;
	BackdropAudioComponent->bAutoDestroy = false;
	BackdropAudioComponent->Play();

	MainThemeAudioComponent = NewObject<UAudioComponent>(this);
	MainThemeAudioComponent->RegisterComponent();
	MainThemeAudioComponent->SetSound(MainThemeMusic);
	MainThemeAudioComponent->SetVolumeMultiplier(0.01f);
	MainThemeAudioComponent->bAutoActivate = false;
	MainThemeAudioComponent->bIsUISound = true;
	MainThemeAudioComponent->bAllowSpatialization = false;
	MainThemeAudioComponent->bAutoDestroy = false;
	MainThemeAudioComponent->Play();

}

// Fade out
void ASGMusicPlayer::FadeOutBackdrop()
{
	GetWorldTimerManager().ClearTimer(BackdropMusicFadeOutTimerHandle);
	GetWorldTimerManager().SetTimer(BackdropMusicFadeOutTimerHandle, this, &ASGMusicPlayer::UpdateFadeOutBackdrop, 0.01f, true);
}

void ASGMusicPlayer::UpdateFadeOutBackdrop()
{
	float CurrentVolume = BackdropAudioComponent->VolumeMultiplier;
	float NewVolume = FMath::Max(CurrentVolume - 0.01f, 0.01f);
	BackdropAudioComponent->SetVolumeMultiplier(NewVolume);

	if (FMath::IsNearlyEqual(NewVolume, 0.01f, 0.001f))
	{
		GetWorldTimerManager().ClearTimer(BackdropMusicFadeOutTimerHandle);
	}
}

void ASGMusicPlayer::FadeOutMainTheme()
{
	GetWorldTimerManager().ClearTimer(MainThemeMusicFadeOutTimerHandle);
	GetWorldTimerManager().SetTimer(MainThemeMusicFadeOutTimerHandle, this, &ASGMusicPlayer::UpdateFadeOutMainTheme, 0.01f, true);
}

void ASGMusicPlayer::UpdateFadeOutMainTheme()
{
	float CurrentVolume = MainThemeAudioComponent->VolumeMultiplier;
	float NewVolume = FMath::Max(CurrentVolume - 0.01f, 0.01f);
	MainThemeAudioComponent->SetVolumeMultiplier(NewVolume);

	if (FMath::IsNearlyEqual(NewVolume, 0.01f, 0.001f))
	{
		GetWorldTimerManager().ClearTimer(MainThemeMusicFadeOutTimerHandle);
	}
}

// Fade in
void ASGMusicPlayer::FadeInBackdrop()
{
	GetWorldTimerManager().ClearTimer(BackdropMusicFadeInTimerHandle);
	GetWorldTimerManager().SetTimer(BackdropMusicFadeInTimerHandle, this, &ASGMusicPlayer::UpdateFadeInBackdrop, 0.01f, true);
}

void ASGMusicPlayer::UpdateFadeInBackdrop()
{
	float CurrentVolume = BackdropAudioComponent->VolumeMultiplier;
	float NewVolume = FMath::Min(CurrentVolume + 0.01f, 1.0f);
	BackdropAudioComponent->SetVolumeMultiplier(NewVolume);

	if (FMath::IsNearlyEqual(NewVolume, 1.0f, 0.001f))
	{
		GetWorldTimerManager().ClearTimer(BackdropMusicFadeInTimerHandle);
	}
}

void ASGMusicPlayer::FadeInMainTheme()
{
	FadeOutBackdrop();
	
	GetWorldTimerManager().ClearTimer(MainThemeMusicFadeInTimerHandle);
	GetWorldTimerManager().SetTimer(MainThemeMusicFadeInTimerHandle, this, &ASGMusicPlayer::UpdateFadeInMainTheme, 0.01f, true);
}

void ASGMusicPlayer::UpdateFadeInMainTheme()
{
	float CurrentVolume = MainThemeAudioComponent->VolumeMultiplier;
	float NewVolume = FMath::Min(CurrentVolume + 0.01f, 1.0f);
	MainThemeAudioComponent->SetVolumeMultiplier(NewVolume);

	if (FMath::IsNearlyEqual(NewVolume, 1.0f, 0.001f))
	{
		GetWorldTimerManager().ClearTimer(MainThemeMusicFadeInTimerHandle);
	}
}