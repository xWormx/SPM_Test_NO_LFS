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

				USGHorizontalBoxObjective* HBoxObjective = ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->GetHorizontalBoxObjective(this, 1);
				if (HBoxObjective == nullptr)
				{
					CALLE_LOG(Error, TEXT("Hbox was null"));
				}
				else
				{
					HBoxObjective->SetValue(FText::FromString(TimeLeftStr));	
				}
				
			}
		}
	}
}

void ASGObjectivePodArrival::OnStart()
{
	Super::OnStart();
	
	ObjectiveHandlerSubSystem->RegisterPodArrival(this);
	
	ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->CreateProgressTextElement(
		GetObjectiveID(),
		FText::FromString(ObjectiveProgressText[0]),
		FText::FromString("Not Completed!"));
	
	bLandingZoneSearchStarted = true;
}

bool ASGObjectivePodArrival::IsCompleted()
{
	return bWaitForPodDone;
}

void ASGObjectivePodArrival::OnCompleted()
{
	Super::OnCompleted();


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

			SetCurrentProgressElementCompleted("Completed!");
			
			USGHorizontalBoxObjective* HBoxObjective = ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->GetHorizontalBoxObjective(this, 0);
			if (HBoxObjective == nullptr)
			{
				CALLE_LOG(Error, TEXT("ASGPodArrival: HBox was nullptr"));
				return;
			}
			HBoxObjective->PlayAnimationValueCompleted();
			
			AdvanceCurrentObjectiveStep();

			ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->CreateProgressTextElement(
						GetObjectiveID(),
						FText::FromString(ObjectiveProgressText[1]),
						FText::FromString(TEXT("00 : 00")));
			OnWaitForPodEventStart.Broadcast();
		}
	}
}

void ASGObjectivePodArrival::ResetPodArrivalTimer()
{
	// Stop and reset the timer
	GetWorldTimerManager().ClearTimer(TimerHandle);

	// Reset internal state (optional, depending on your needs)
	bWaitForPodEventStarted = false;
	bWaitForPodEventPaused = false;
	bWaitForPodDone = false;

	// Log or trigger reset logic if needed
	UE_LOG(LogTemp, Warning, TEXT("Pod arrival timer has been reset."));
}

void ASGObjectivePodArrival::OnTimeIsOut()
{
	ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->Display(GetObjectiveCompletedToolTip());
	
	bWaitForPodDone = true;
	
	SetCurrentProgressElementCompleted("Pod is here!");
	
	USGHorizontalBoxObjective* HBoxObjective = ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->GetHorizontalBoxObjective(this, 1);
	if (HBoxObjective == nullptr)
	{
		CALLE_LOG(Error, TEXT("ASGPodArrival: HBox was nullptr"));
		return;
	}
	HBoxObjective->PlayAnimationValueCompleted();
	
	// Detta måste ske med en delay annars kommer animation för progress window elementet (HorizontalBoxObjective) att avbrytas
	// Då bara en PlayAnimation verkar kunna va igång samtidgt.
	FTimerHandle DelayHandle;
	GetWorldTimerManager().SetTimer(DelayHandle, [this]()
	{
		OnWaitForPodEventEnd.Broadcast(this);
	}, 1.0f, false); // Justera delay så att den matchar animationen
	
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
		USGHorizontalBoxObjective* HBoxObjective = ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->GetHorizontalBoxObjective(this, 1);
		if (HBoxObjective == nullptr)
		{
			CALLE_LOG(Error, TEXT("HBox was nullptr"));
			return;
		}
		HBoxObjective->SetValue(FText::FromString("Return to Pod!"));
		HBoxObjective->PlayUpdateValueAnimation();
		
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
		USGHorizontalBoxObjective* HBoxObjective = ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->GetHorizontalBoxObjective(this, 1);
		if (HBoxObjective == nullptr)
		{
			CALLE_LOG(Error, TEXT("HBox was nullptr"));
			return;
		}
		HBoxObjective->PlayUpdateValueAnimation();
		bWaitForPodEventPaused = false;
		
		GetWorldTimerManager().UnPauseTimer(TimerHandle);
		
		OnWaitForPodEventUnPaused.Broadcast();
	}
}
