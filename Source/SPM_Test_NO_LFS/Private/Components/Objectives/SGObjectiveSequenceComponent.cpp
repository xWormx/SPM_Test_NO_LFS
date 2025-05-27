// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Objectives/SGObjectiveSequenceComponent.h"

#include "LevelSequencePlayer.h"
#include "Core/SGObjectiveHandlerSubSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USGObjectiveSequenceComponent::USGObjectiveSequenceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USGObjectiveSequenceComponent::BeginPlay()
{
	Super::BeginPlay();

	USGObjectiveHandlerSubSystem* ObjectiveHandlerSubSystem = GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>();
	if (ObjectiveHandlerSubSystem)
	{
		ObjectiveHandlerSubSystem->OnObjectiveCompletedWithType.AddDynamic(this, &USGObjectiveSequenceComponent::PlaySequence);
	}
}


// Called every frame
void USGObjectiveSequenceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USGObjectiveSequenceComponent::PlaySequence(EObjectiveType ObjectiveType)
{
	if ((ObjectiveType & ObjectiveTypeToWatch) != EObjectiveType::EOT_None)
	{
		ALevelSequenceActor* OutActor;
		ULevelSequencePlayer* LandingSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
			GetWorld(),
			SequenceToPlay,
			FMovieSceneSequencePlaybackSettings(),
			OutActor
			);
		LandingSequencePlayer->Play();
		int i = 0;
		if (SoundDuringSequence)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), SoundDuringSequence);
		}
	}
}

