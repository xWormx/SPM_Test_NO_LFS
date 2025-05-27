// Fill out your copyright notice in the Description page of Project Settings.

// comment
#include "Objectives/SGObjectivePodArrival.h"

#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "Objectives/SGObjectiveToolTipWidget.h"
#include "Player/SGPlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Core/SGObjectiveHandlerSubSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Objectives/SGHorizontalBoxObjective.h"


ASGObjectivePodArrival::ASGObjectivePodArrival()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	MeshPod = CreateDefaultSubobject<USkeletalMeshComponent>("MeshPod");
	MeshPod->SetupAttachment(Root);

	SphereInteractArea = CreateDefaultSubobject<USphereComponent>("SphereInteractAArea");
	SphereInteractArea->SetupAttachment(Root);
	
	MeshLandingZone = CreateDefaultSubobject<UStaticMeshComponent>("MeshLandingZone");
	MeshLandingZone->SetupAttachment(Root);

	SphereRestrictiveArea = CreateDefaultSubobject<USphereComponent>("SphereRestrictiveArea");
	SphereRestrictiveArea->SetupAttachment(MeshLandingZone);
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
		if (ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->GetTimerAnimationFinished() || true)
		{
			float TimeLeft = GetWorldTimerManager().GetTimerRemaining(TimerHandle);
			if (TimeLeft >= 0)
			{
				int Minutes = TimeLeft / 60;
				int Seconds = TimeLeft - (Minutes * 60);
				FString TimeLeftStr = FString::Printf(TEXT("%02d : %02d"), Minutes, Seconds);

				int ProgressStep = GetCurrentProgressStep();
				HorizontalBoxProgressElement[ProgressStep]->SetValue(FText::FromString(TimeLeftStr));
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
	if (AnimationPodOpen)
		MeshPod->PlayAnimation(AnimationPodOpen, false);
		
	return bWaitForPodDone;
}

void ASGObjectivePodArrival::OnCompleted()
{
	Super::OnCompleted();

	SetCurrentProgressElementCompleted("Pod is here!");
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
		if (ObjectiveHandlerSubSystem && !bWaitForPodEventStarted && bLandingZoneSearchStarted)
		{
			bWaitForPodEventStarted = true;
			bWaitForPodEventPaused = false;
			ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->Display(GetCurrentSubToolTip());

			GetWorldTimerManager().SetTimer(TimerHandle, this, &ASGObjectivePodArrival::OnTimeIsOut, TimeToWaitForPodPodSeconds, false);

			if (HorizontalBoxProgressElement.IsEmpty())
				return;
			
			SetCurrentProgressElementCompleted("Completed!");

			AdvanceCurrentObjectiveStep();
			
			HorizontalBoxProgressElement.Add(ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->CreateProgressTextElement(FText::FromString(ObjectiveProgressText[1]), FText::FromString(TEXT("00 : 00"))));

			OnWaitForPodEventStart.Broadcast();
		}
	}
}

void ASGObjectivePodArrival::OnTimeIsOut()
{
	ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->Display(GetObjectiveCompletedToolTip());
	
	bWaitForPodDone = true;
	
	OnWaitForPodEventEnd.Broadcast(this);
}

void ASGObjectivePodArrival::PauseMainPhase(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASGPlayerCharacter* Player = Cast<ASGPlayerCharacter>(OtherActor);
	if (Player == nullptr)
	{
		return;
	}
		
	if (bWaitForPodEventStarted && !bWaitForPodDone)
	{
		int ProgressStep = GetCurrentProgressStep();
		HorizontalBoxProgressElement[ProgressStep]->SetValue(FText::FromString("Return to Pod!"));
		HorizontalBoxProgressElement[ProgressStep]->PlayUpdateValueAnimation();
		
		bWaitForPodEventPaused = true;
		
		GetWorldTimerManager().PauseTimer(TimerHandle);
		
		OnWaitForPodEventPaused.Broadcast();
	}
		
}

void ASGObjectivePodArrival::UnPauseMainPhase(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ASGPlayerCharacter* Player = Cast<ASGPlayerCharacter>(OtherActor);
	if (Player == nullptr)
	{
		return;
	}
	
	if (bWaitForPodEventStarted && !bWaitForPodDone)
	{
		int ProgressStep = GetCurrentProgressStep();
		HorizontalBoxProgressElement[ProgressStep]->PlayUpdateValueAnimation();
		
		bWaitForPodEventPaused = false;
		
		GetWorldTimerManager().UnPauseTimer(TimerHandle);
		
		OnWaitForPodEventUnPaused.Broadcast();
	}
}
