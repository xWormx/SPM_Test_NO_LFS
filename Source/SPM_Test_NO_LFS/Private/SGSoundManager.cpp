// Fill out your copyright notice in the Description page of Project Settings.


#include "SGSoundManager.h"

#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"


// Sets default values
ASGSoundManager::ASGSoundManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AudioComponent  = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->bIsUISound = true;
	AudioComponent->SetupAttachment(RootComponent);
	RootComponent = AudioComponent;
}

// Called when the game starts or when spawned
void ASGSoundManager::BeginPlay()
{
	Super::BeginPlay();

	Super::BeginPlay();

	if (BackgroundMusicCue)
	{
		AudioComponent->SetSound(BackgroundMusicCue);
		AudioComponent->Play();
	}
	
}

// Called every frame
void ASGSoundManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

