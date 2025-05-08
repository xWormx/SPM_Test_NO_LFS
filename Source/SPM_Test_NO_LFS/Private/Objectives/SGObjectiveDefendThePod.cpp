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

	SphereInteractArea = CreateDefaultSubobject<USphereComponent>("SphereInteractAArea");
	SphereInteractArea->SetupAttachment(MeshToDefend);
	
	MeshRestrictiveFloor = CreateDefaultSubobject<UStaticMeshComponent>("MeshRestrictiveFloor");
	MeshRestrictiveFloor->SetupAttachment(MeshToDefend);

	SphereRestrictiveArea = CreateDefaultSubobject<USphereComponent>("SphereRestrictiveArea");
	SphereRestrictiveArea->SetupAttachment(MeshRestrictiveFloor);
}

void ASGObjectiveDefendThePod::BeginPlay()
{
	Super::BeginPlay();
	SphereInteractArea->OnComponentBeginOverlap.AddDynamic(this, &ASGObjectiveDefendThePod::StartMainPhase);
	SphereRestrictiveArea->OnComponentBeginOverlap.AddDynamic(this, &ASGObjectiveDefendThePod::UnPauseMainPhase);
	SphereRestrictiveArea->OnComponentEndOverlap.AddDynamic(this, &ASGObjectiveDefendThePod::PauseMainPhase);
}

void ASGObjectiveDefendThePod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bDefendEventStarted && !bDefendEventPaused)
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
	ObjectiveHandler->RegisterDefendThePod(this);
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
			bDefendEventPaused = false;
			GetObjectiveHandler()->GetObjectiveToolTipWidget()->DisplayTimer(FText::FromString("00:00"));
			GetObjectiveHandler()->GetObjectiveToolTipWidget()->Display(GetCurrentSubToolTip());
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ASGObjectiveDefendThePod::OnTimeIsOut, TimeToDefendPodSeconds, false);
			AdvanceCurrentObjectiveStep();
			GetObjectiveHandler()->GetObjectiveToolTipWidget()->SetProgressWindowText(this);

			OnDefendEventStart.Broadcast();
		}
	}
}

void ASGObjectiveDefendThePod::OnTimeIsOut()
{
	UE_LOG(LogTemp, Warning, TEXT("Time is out to defend the pod, success or loss?"));
	GetObjectiveHandler()->GetObjectiveToolTipWidget()->Display(GetObjectiveCompletedToolTip());
	bDefendedThePod = true;
	OnDefendEventEnd.Broadcast(this);
}

void ASGObjectiveDefendThePod::PauseMainPhase(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASGPlayerCharacter* Player = Cast<ASGPlayerCharacter>(OtherActor);
	if (Player == nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s (Pause): Wasn't not the Player!"), *GetActorLabel());
		return;
	}
		
	if (bDefendEventStarted)
	{
		bDefendEventPaused = true;
		GetWorldTimerManager().PauseTimer(TimerHandle);
		GetObjectiveHandler()->GetObjectiveToolTipWidget()->DisplayTimer(FText::FromString("PAUSED"));
		OnDefendEventPaused.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("DEFEND THE POD PAUSED!"));
	}
		
}

void ASGObjectiveDefendThePod::UnPauseMainPhase(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ASGPlayerCharacter* Player = Cast<ASGPlayerCharacter>(OtherActor);
	if (Player == nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s (UnPause): Wasn't not the Player!"), *GetActorLabel());
		return;
	}
	
	if (bDefendEventStarted)
	{
		bDefendEventPaused = false;
		GetWorldTimerManager().UnPauseTimer(TimerHandle);
		OnDefendEventUnPaused.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("DEFEND THE POD UNPAUSED!"));
	}
}
