// Fill out your copyright notice in the Description page of Project Settings.


#include "SGObjectiveDefendThePod.h"

#include "SGGameObjectivesHandler.h"
#include "SGObjectiveToolTipWidget.h"
#include "SGPlayerCharacter.h"
#include "Components/SphereComponent.h"


ASGObjectiveDefendThePod::ASGObjectiveDefendThePod()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	MeshToDefend = CreateDefaultSubobject<UStaticMeshComponent>("MeshToDefend");
	MeshToDefend->SetupAttachment(Root);

	MeshRestrictiveFloor = CreateDefaultSubobject<UStaticMeshComponent>("MeshRestrictiveFloor");
	MeshRestrictiveFloor->SetupAttachment(MeshToDefend);

	SphereRestrictiveArea = CreateDefaultSubobject<USphereComponent>("SphereRestrictiveArea");
	SphereRestrictiveArea->SetupAttachment(MeshRestrictiveFloor);
	
}

void ASGObjectiveDefendThePod::BeginPlay()
{
	Super::BeginPlay();
	SphereRestrictiveArea->OnComponentBeginOverlap.AddDynamic(this, &ASGObjectiveDefendThePod::StartMainPhase);
}

void ASGObjectiveDefendThePod::OnStart(ASGGameObjectivesHandler* ObjectiveHandler)
{
	Super::OnStart(ObjectiveHandler);
	SetObjectiveHandler(ObjectiveHandler);
	// CurrentPhase = Phases[0];
}
  
bool ASGObjectiveDefendThePod::IsCompleted(ASGGameObjectivesHandler* ObjectivesHandler)
{
	return bDefendedThePod;
}

void ASGObjectiveDefendThePod::Update(ASGGameObjectivesHandler* ObjectivesHandler)
{
	/*
	1. Overlap with PodArea
	2. SpawnEnemies
	
		if(CurrentPhase->Completed())
			Remove CurrentPhase
			CurrentPhase = Phases[0]; 
	 */
}

void ASGObjectiveDefendThePod::StartMainPhase(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ASGPlayerCharacter* Player = Cast<ASGPlayerCharacter>(OtherActor);
	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("PLAYER IS BY THE POD START DEFENDING!"));	
		if (GetObjectiveHandler() && !bDefendEventStarted)
		{
			bDefendEventStarted = true;
			GetObjectiveHandler()->GetObjectiveToolTipWidget()->DisplayTimer(FText::FromString("Timer: 00:00"));
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ASGObjectiveDefendThePod::OnTimeIsOut, TimeToDefendPodSeconds, false);
		}
	}
}

void ASGObjectiveDefendThePod::OnTimeIsOut()
{
	UE_LOG(LogTemp, Warning, TEXT("Time is out to defend the pod, success or loss?"));
}

