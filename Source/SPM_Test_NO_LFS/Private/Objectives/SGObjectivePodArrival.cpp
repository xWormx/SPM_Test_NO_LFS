// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGObjectivePodArrival.h"
#include "Objectives/SGObjectiveToolTipWidget.h"
#include "Player/SGPlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Core/SGObjectiveHandlerSubSystem.h"
#include "Objectives/SGHorizontalBoxObjective.h"


ASGObjectivePodArrival::ASGObjectivePodArrival()
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

void ASGObjectivePodArrival::BeginPlay()
{
	Super::BeginPlay();
	SphereInteractArea->OnComponentBeginOverlap.AddDynamic(this, &ASGObjectivePodArrival::StartMainPhase);
	SphereRestrictiveArea->OnComponentBeginOverlap.AddDynamic(this, &ASGObjectivePodArrival::UnPauseMainPhase);
	SphereRestrictiveArea->OnComponentEndOverlap.AddDynamic(this, &ASGObjectivePodArrival::PauseMainPhase);
}

void ASGObjectivePodArrival::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bWaitForPodEventStarted && !bWaitForPodEventPaused)
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

void ASGObjectivePodArrival::OnStart()
{
	Super::OnStart();
	ObjectiveHandlerSubSystem->RegisterPodArrival(this);
	HorizontalBoxProgressElement.Add(ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->CreateProgressTextElement(FText::FromString(ObjectiveProgressText[0]), FText::FromString("Not Completed!")));
	bLandingZoneSearchStarted = true;
}
  
bool ASGObjectivePodArrival::IsCompleted()
{
	return bWaitForPodDone;
}

void ASGObjectivePodArrival::OnCompleted()
{
	Super::OnCompleted();
	HorizontalBoxProgressElement[1]->ShowSucceed();
	HorizontalBoxProgressElement[1]->SetKeyAndValueOpacity(0.5);
	HorizontalBoxProgressElement[1]->SetValue(FText::FromString("Pod is here!"));
}

void ASGObjectivePodArrival::Update()
{
}


void ASGObjectivePodArrival::StartMainPhase(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ASGPlayerCharacter* Player = Cast<ASGPlayerCharacter>(OtherActor);
	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("PLAYER IS BY THE POD START DEFENDING!"));	
		//if (GetObjectiveHandler() && !bDefendEventStarted)
		if (ObjectiveHandlerSubSystem && !bWaitForPodEventStarted && bLandingZoneSearchStarted)
		{
			bWaitForPodEventStarted = true;
			bWaitForPodEventPaused = false;
			//GetObjectiveHandler()->GetObjectiveToolTipWidget()->Display(GetCurrentSubToolTip());
			ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->Display(GetCurrentSubToolTip());
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ASGObjectivePodArrival::OnTimeIsOut, TimeToWaitForPodPodSeconds, false);
			AdvanceCurrentObjectiveStep();
			if (HorizontalBoxProgressElement.IsEmpty())
				return;
			HorizontalBoxProgressElement[0]->ShowSucceed();
			HorizontalBoxProgressElement[0]->SetValue(FText::FromString("Completed!"));
			HorizontalBoxProgressElement[0]->SetKeyAndValueOpacity(0.5);
			
			//HorizontalBoxProgressElement.Add(GetObjectiveHandler()->GetObjectiveToolTipWidget()->CreateProgressTextElement(FText::FromString("Defend the Pod!"), FText::FromString(TEXT("00 : 00"))));
			HorizontalBoxProgressElement.Add(ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->CreateProgressTextElement(FText::FromString(ObjectiveProgressText[1]), FText::FromString(TEXT("00 : 00"))));

			OnWaitForPodEventStart.Broadcast();
		}
	}
}

void ASGObjectivePodArrival::OnTimeIsOut()
{
	UE_LOG(LogTemp, Warning, TEXT("Time is out to defend the pod, success or loss?"));
	//GetObjectiveHandler()->GetObjectiveToolTipWidget()->Display(GetObjectiveCompletedToolTip());
	ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->Display(GetObjectiveCompletedToolTip());
	bWaitForPodDone = true;
	OnWaitForPodEventEnd.Broadcast(this);
}

void ASGObjectivePodArrival::PauseMainPhase(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASGPlayerCharacter* Player = Cast<ASGPlayerCharacter>(OtherActor);
	if (Player == nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s (Pause): Wasn't not the Player!"), *GetActorLabel());
		return;
	}
		
	if (bWaitForPodEventStarted && !bWaitForPodDone)
	{
		HorizontalBoxProgressElement[1]->SetValue(FText::FromString("Return to Pod!"));
		HorizontalBoxProgressElement[1]->PlayUpdateValueAnimation();
		bWaitForPodEventPaused = true;
		GetWorldTimerManager().PauseTimer(TimerHandle);
		OnWaitForPodEventPaused.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("WAIT FOR POD PAUSED!"));
	}
		
}

void ASGObjectivePodArrival::UnPauseMainPhase(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ASGPlayerCharacter* Player = Cast<ASGPlayerCharacter>(OtherActor);
	if (Player == nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s (UnPause): Wasn't not the Player!"), *GetActorLabel());
		return;
	}
	
	if (bWaitForPodEventStarted && !bWaitForPodDone)
	{
		HorizontalBoxProgressElement[1]->PlayUpdateValueAnimation();
		bWaitForPodEventPaused = false;
		GetWorldTimerManager().UnPauseTimer(TimerHandle);
		OnWaitForPodEventUnPaused.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("WAIT FOR POD UNPAUSED!"));
	}
}
