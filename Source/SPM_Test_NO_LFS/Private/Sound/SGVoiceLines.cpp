// Joel Larsson Wendt || jola6902

#include "Sound/SGVoiceLines.h"
#include "Components/AudioComponent.h"
#include "Gear/Weapons/jola6902_GunsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SGPlayerCharacter.h"
#include "Core/SGObjectiveHandlerSubSystem.h"

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
	ObjectiveHandlerRef = GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>();

	BindDelegateHandlers();
}

void ASGVoiceLines::PlaySound(USoundBase* Sound, float Cooldown)
{
	if (AudioComponent->IsPlaying()) return;
	if (!CooldownMap.Contains(Sound) || CooldownMap[Sound] > 0.f) return;
	
	CooldownMap[Sound] = Cooldown;
	
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

	if (ObjectiveHandlerRef)
	{
		ObjectiveHandlerRef->OnObjectiveStartedWithType.AddUniqueDynamic(this, &ASGVoiceLines::Voice_ObjectiveStarted);
		ObjectiveHandlerRef->OnObjectiveCompletedWithType.AddUniqueDynamic(this, &ASGVoiceLines::Voice_ObjectiveCompleted);
	}
}

void ASGVoiceLines::Voice_Fluff(ASGEnemyCharacter* Enemy)
{
	PlaySound(Sounds[0], 10.f);
}

void ASGVoiceLines::Voice_Reload(int32 GunIndex, ASGGun* Gun)
{
	PlaySound(Sounds[1], 30.f);
}

void ASGVoiceLines::Voice_ObjectiveStarted(EObjectiveType ObjectiveType)
{
	if (ObjectiveType == EObjectiveType::EOT_PodArrival)
	{
		
	}
}

void ASGVoiceLines::Voice_ObjectiveCompleted(EObjectiveType ObjectiveType)
{
	PlaySound(Sounds[2], 60.f);
}