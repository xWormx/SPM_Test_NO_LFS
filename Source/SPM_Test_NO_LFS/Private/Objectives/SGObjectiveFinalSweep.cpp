// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGObjectiveFinalSweep.h"
#include "Objectives/SGObjectiveToolTipWidget.h"
#include "Components/BoxComponent.h"
#include "Core/SGObjectiveHandlerSubSystem.h"
#include "Objectives/SGHorizontalBoxObjective.h"
#include "Player/SGPlayerCharacter.h"

ASGObjectiveFinalSweep::ASGObjectiveFinalSweep()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	RootComponent = SceneRoot;

	SceneLocation = CreateDefaultSubobject<USceneComponent>("SceneLocation");
	SceneLocation->SetupAttachment(SceneRoot);
	
	EscapeTriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("EscapeTriggerZone"));
	EscapeTriggerZone->SetupAttachment(SceneLocation);
}

void ASGObjectiveFinalSweep::BeginPlay()
{
	Super::BeginPlay();
	EscapeTriggerZone->OnComponentBeginOverlap.AddDynamic(this, &ASGObjectiveFinalSweep::EndGame);
	EscapeTriggerZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool ASGObjectiveFinalSweep::IsCompleted()
{
	return Super::IsCompleted();
}

void ASGObjectiveFinalSweep::OnStart()
{
	Super::OnStart();
/*
	HorizontalBoxProgressElement.Add(ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->CreateProgressTextElement(
		GetObjectiveID(),
		FText::FromString(ObjectiveProgressText[0]),
		FText::FromString("0")));
*/
	ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->CreateProgressTextElement(
	GetObjectiveID(),
	FText::FromString(ObjectiveProgressText[0]),
	FText::FromString("0"));
	EscapeTriggerZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ASGObjectiveFinalSweep::OnCompleted()
{
	Super::OnCompleted();
}

void ASGObjectiveFinalSweep::Update()
{
	Super::Update();
	IncrementEnemiesKilled();
	FString StrEnemiesKilled = FString::Printf(TEXT("%d"), CurrentEnemiesKilled);
	//HorizontalBoxProgressElement[GetCurrentProgressStep()]->SetValue(FText::FromString(StrEnemiesKilled));
	USGHorizontalBoxObjective* HBoxObjective = ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->GetHorizontalBoxObjective(this, 0);
	HBoxObjective->SetValue(FText::FromString(StrEnemiesKilled));
}

void ASGObjectiveFinalSweep::IncrementEnemiesKilled()
{
	CurrentEnemiesKilled++;
}

void ASGObjectiveFinalSweep::EndGame(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASGPlayerCharacter* Player = Cast<ASGPlayerCharacter>(OtherActor);
	if (Player == nullptr)
	{
		return;
	}
	USGObjectiveHandlerSubSystem* ObjectiveHandler = GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>();
	if (ObjectiveHandler == nullptr)
		return;

	ObjectiveHandler->OnEndGame.Broadcast();
		
	CALLE_LOG(Error, TEXT("Game Over - Call some EndGame function here!"));
}