// Joel Larsson Wendt || jola6902

#include "Sound/SGVoiceLines.h"
#include "Components/AudioComponent.h"

ASGVoiceLines::ASGVoiceLines()
{
	PrimaryActorTick.bCanEverTick = true;

	AudioComponent  = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;
	RootComponent = AudioComponent;
}

void ASGVoiceLines::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASGVoiceLines::BeginPlay()
{
	Super::BeginPlay();
}