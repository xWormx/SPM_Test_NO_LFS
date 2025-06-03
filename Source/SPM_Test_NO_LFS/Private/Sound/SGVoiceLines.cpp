// Joel Larsson Wendt || jola6902

#include "Sound/SGVoiceLines.h"
#include "Components/AudioComponent.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Math/UnrealMathUtility.h"

ASGVoiceLines::ASGVoiceLines()
{
	PrimaryActorTick.bCanEverTick = true;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;
	RootComponent = AudioComponent;

	BindDelegateHandlers();
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

void ASGVoiceLines::PlaySound(USoundBase* Sound)
{
	if (AudioComponent->IsPlaying()) return;
	if (!CooldownMap.Contains(Sound) || CooldownMap[Sound] > 0.f) return;
	
	CooldownMap[Sound] = 5.f;
	
	
	AudioComponent->SetSound(Sound);
	AudioComponent->Play();
}

// Delegate handling
void ASGVoiceLines::BindDelegateHandlers() const
{
	
}


void ASGVoiceLines::PlayFluffCue(ASGEnemyCharacter* Enemy)
{
	PlaySound(Sounds[0]);
}