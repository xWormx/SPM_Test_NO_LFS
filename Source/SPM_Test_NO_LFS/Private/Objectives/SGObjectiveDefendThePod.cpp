// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGObjectiveDefendThePod.h"
#include "Objectives/SGGameObjectivesHandler.h"
#include "Objectives/SGObjectiveToolTipWidget.h"
#include "Player/SGPlayerCharacter.h"
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

void ASGObjectiveDefendThePod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bDefendEventStarted)
	{
		if (GetObjectiveHandler()->GetObjectiveToolTipWidget()->GetTimerAnimationFinished())
		{
			float TimeLeft = GetWorldTimerManager().GetTimerRemaining(TimerHandle);
			if (TimeLeft >= 0)
			{
				int Minutes = TimeLeft / 60;
				int Seconds = TimeLeft - (Minutes * 60);
				FString TimeLeftStr = FString::Printf(TEXT("%02d : %02d"), Minutes, Seconds);
		
				GetObjectiveHandler()->GetObjectiveToolTipWidget()->DisplayTimer(FText::FromString(TimeLeftStr));	
			}
		}
	}
}

void ASGObjectiveDefendThePod::OnStart(ASGGameObjectivesHandler* ObjectiveHandler)
{
	Super::OnStart(ObjectiveHandler);
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
			OnDefendEventStart.Broadcast();
			GetObjectiveHandler()->GetObjectiveToolTipWidget()->DisplayTimer(FText::FromString("00:00"));
			GetObjectiveHandler()->GetObjectiveToolTipWidget()->Display(GetCurrentSubToolTip());
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ASGObjectiveDefendThePod::OnTimeIsOut, TimeToDefendPodSeconds, false);
			AdvanceCurrentObjectiveStep();
			GetObjectiveHandler()->GetObjectiveToolTipWidget()->SetProgressWindowText(this);
		}
	}
}

void ASGObjectiveDefendThePod::OnTimeIsOut()
{
	UE_LOG(LogTemp, Warning, TEXT("Time is out to defend the pod, success or loss?"));
	GetObjectiveHandler()->GetObjectiveToolTipWidget()->Display(GetObjectiveCompletedToolTip());
}

