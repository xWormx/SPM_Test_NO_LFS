
#include "Sound/SGSoundManager.h"

#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"



ASGSoundManager::ASGSoundManager()
{
	PrimaryActorTick.bCanEverTick = false;

	AudioComponent  = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->bIsUISound = true;
	AudioComponent->SetupAttachment(RootComponent);
	RootComponent = AudioComponent;
}

void ASGSoundManager::BeginPlay()
{
	Super::BeginPlay();

	if (BackgroundMusicCue)
	{
		AudioComponent->SetSound(BackgroundMusicCue);
		AudioComponent->Play();
	}
	
}

void ASGSoundManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

