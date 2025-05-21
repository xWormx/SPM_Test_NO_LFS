// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGObjectiveDefendThePod.h"
#include "Objectives/SGGameObjectivesHandler.h"
#include "Objectives/SGObjectiveToolTipWidget.h"
#include "Player/SGPlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Core/SGObjectiveHandlerSubSystem.h"
#include "Objectives/SGHorizontalBoxObjective.h"


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
		//if (GetObjectiveHandler()->GetObjectiveToolTipWidget()->GetTimerAnimationFinished() || true)
		if (ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->GetTimerAnimationFinished() || true)
		{
			float TimeLeft = GetWorldTimerManager().GetTimerRemaining(TimerHandle);
			if (TimeLeft >= 0)
			{
				int Minutes = TimeLeft / 60;
				int Seconds = TimeLeft - (Minutes * 60);
				FString TimeLeftStr = FString::Printf(TEXT("%02d : %02d"), Minutes, Seconds);
		
				HorizontalBoxProgressElement[1]->SetValue(FText::FromString(TimeLeftStr));
			}
		}
	}
}
/*
void ASGObjectiveDefendThePod::OnStart(ASGGameObjectivesHandler* ObjectiveHandler)
{
	Super::OnStart(ObjectiveHandler);
	ObjectiveHandler->RegisterDefendThePod(this);
	HorizontalBoxProgressElement.Add(ObjectiveHandler->GetObjectiveToolTipWidget()->CreateProgressTextElement(FText::FromString("Find the Pod!"), FText::FromString("Not Completed!")));
}
  
bool ASGObjectiveDefendThePod::IsCompleted(ASGGameObjectivesHandler* ObjectiveHandler)
{
	return bDefendedThePod;
}

void ASGObjectiveDefendThePod::OnCompleted(ASGGameObjectivesHandler* ObjectiveHandler)
{
	Super::OnCompleted(ObjectiveHandler);
	HorizontalBoxProgressElement[1]->ShowSucceed();
	HorizontalBoxProgressElement[1]->SetKeyAndValueOpacity(0.5);
	HorizontalBoxProgressElement[1]->SetValue(FText::FromString("Completed!"));
}

void ASGObjectiveDefendThePod::Update(ASGGameObjectivesHandler* ObjectiveHandler)
{
}
*/
void ASGObjectiveDefendThePod::OnStart()
{
	Super::OnStart();
	ObjectiveHandlerSubSystem->RegisterDefendThePod(this);
	HorizontalBoxProgressElement.Add(ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->CreateProgressTextElement(FText::FromString(ObjectiveProgressText[0]), FText::FromString("Not Completed!")));
}
  
bool ASGObjectiveDefendThePod::IsCompleted()
{
	return bDefendedThePod;
}

void ASGObjectiveDefendThePod::OnCompleted()
{
	Super::OnCompleted();
	HorizontalBoxProgressElement[1]->ShowSucceed();
	HorizontalBoxProgressElement[1]->SetKeyAndValueOpacity(0.5);
	HorizontalBoxProgressElement[1]->SetValue(FText::FromString("Completed!"));
}

void ASGObjectiveDefendThePod::Update()
{
}


void ASGObjectiveDefendThePod::StartMainPhase(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ASGPlayerCharacter* Player = Cast<ASGPlayerCharacter>(OtherActor);
	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("PLAYER IS BY THE POD START DEFENDING!"));	
		//if (GetObjectiveHandler() && !bDefendEventStarted)
		if (ObjectiveHandlerSubSystem && !bDefendEventStarted)
		{
			bDefendEventStarted = true;
			bDefendEventPaused = false;
			//GetObjectiveHandler()->GetObjectiveToolTipWidget()->Display(GetCurrentSubToolTip());
			ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->Display(GetCurrentSubToolTip());
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ASGObjectiveDefendThePod::OnTimeIsOut, TimeToDefendPodSeconds, false);
			AdvanceCurrentObjectiveStep();
	
			HorizontalBoxProgressElement[0]->ShowSucceed();
			HorizontalBoxProgressElement[0]->SetValue(FText::FromString("Completed!"));
			HorizontalBoxProgressElement[0]->SetKeyAndValueOpacity(0.5);
			
			//HorizontalBoxProgressElement.Add(GetObjectiveHandler()->GetObjectiveToolTipWidget()->CreateProgressTextElement(FText::FromString("Defend the Pod!"), FText::FromString(TEXT("00 : 00"))));
			HorizontalBoxProgressElement.Add(ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->CreateProgressTextElement(FText::FromString(ObjectiveProgressText[1]), FText::FromString(TEXT("00 : 00"))));

			OnDefendEventStart.Broadcast();
		}
	}
}

void ASGObjectiveDefendThePod::OnTimeIsOut()
{
	UE_LOG(LogTemp, Warning, TEXT("Time is out to defend the pod, success or loss?"));
	//GetObjectiveHandler()->GetObjectiveToolTipWidget()->Display(GetObjectiveCompletedToolTip());
	ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->Display(GetObjectiveCompletedToolTip());
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
		
	if (bDefendEventStarted && !bDefendedThePod)
	{
		HorizontalBoxProgressElement[1]->SetValue(FText::FromString("Return to Pod!"));
		HorizontalBoxProgressElement[1]->PlayUpdateValueAnimation();
		bDefendEventPaused = true;
		GetWorldTimerManager().PauseTimer(TimerHandle);
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
	
	if (bDefendEventStarted && !bDefendedThePod)
	{
		HorizontalBoxProgressElement[1]->PlayUpdateValueAnimation();
		bDefendEventPaused = false;
		GetWorldTimerManager().UnPauseTimer(TimerHandle);
		OnDefendEventUnPaused.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("DEFEND THE POD UNPAUSED!"));
	}
}
