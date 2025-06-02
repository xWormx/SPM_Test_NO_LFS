// Joel Larsson Wendt || jola6902

#include "Sound/SGVoiceLines.h"
#include "Components/AudioComponent.h"

ASGVoiceLines::ASGVoiceLines()
{
	PrimaryActorTick.bCanEverTick = true;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;
	RootComponent = AudioComponent;
}

void ASGVoiceLines::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CooldownMap.IsEmpty()) return;
	
	for (TPair<USoundBase*, float>& Pair : CooldownMap)
	{
		if (Pair.Value > 0.f)
		{
			Pair.Value -= DeltaTime;
			if (Pair.Value < 0.f)
			{
				Pair.Value = 0.f;
			}
		}
	}
}

void ASGVoiceLines::BeginPlay()
{
	Super::BeginPlay();

	for (USoundBase* Sound : Sounds)
	{
		CooldownMap.Add(Sound, 0.f);
	}
}

void ASGVoiceLines::PlaySound(USoundBase* Sound) const
{
	if (AudioComponent->IsPlaying()) return;
	
	AudioComponent->SetSound(Sound);
	AudioComponent->Play();
}