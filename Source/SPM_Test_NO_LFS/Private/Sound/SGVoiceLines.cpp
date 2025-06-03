// Joel Larsson Wendt || jola6902

#include "Sound/SGVoiceLines.h"
#include "Components/AudioComponent.h"
#include "Gear/Weapons/jola6902_GunsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SGPlayerCharacter.h"

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

	PlayerRef = Cast<ASGPlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	BindDelegateHandlers();
}

void ASGVoiceLines::PlaySound(USoundBase* Sound)
{
	if (AudioComponent->IsPlaying()) return;
	if (!CooldownMap.Contains(Sound) || CooldownMap[Sound] > 0.f) return;
	
	CooldownMap[Sound] = 10.f;
	
	AudioComponent->SetSound(Sound);
	AudioComponent->Play();
}

// Delegate handling
void ASGVoiceLines::BindDelegateHandlers() const
{
	if (PlayerRef->GunsComponent)
	{
		PlayerRef->GunsComponent->OnReload.AddUniqueDynamic(this, &ASGVoiceLines::Voice_Reload);
	}
}

void ASGVoiceLines::Voice_Fluff(ASGEnemyCharacter* Enemy)
{
	PlaySound(Sounds[0]);
}

void ASGVoiceLines::Voice_Reload(int32 GunIndex, ASGGun* Gun)
{
	PlaySound(Sounds[1]);
}

void ASGVoiceLines::Voice_FindTerminal()
{
	PlaySound(Sounds[2]);
}