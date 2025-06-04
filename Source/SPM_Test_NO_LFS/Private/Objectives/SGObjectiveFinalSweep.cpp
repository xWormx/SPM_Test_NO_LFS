// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGObjectiveFinalSweep.h"
#include "Objectives/SGObjectiveToolTipWidget.h"
#include "Components/BoxComponent.h"
#include "Core/SGObjectiveHandlerSubSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Objectives/SGHorizontalBoxObjective.h"
#include "Player/SGPlayerCharacter.h"
#include "Player/SGPlayerController.h"
#include "UI/SGMainHUD.h"

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
	EscapeTriggerZone->OnComponentBeginOverlap.AddDynamic(this, &ASGObjectiveFinalSweep::EnableEscapeWithPod);
	EscapeTriggerZone->OnComponentEndOverlap.AddDynamic(this, &ASGObjectiveFinalSweep::DisableEscapeWithPod);
	EscapeTriggerZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TArray<AActor*> TaggedActors;
	
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("EscapePod"), TaggedActors);

	if (TaggedActors.Num() > 0 && EscapeTriggerZone)
	{
		AActor* PodMeshActor = TaggedActors[0];
		USceneComponent* MeshRoot = PodMeshActor->GetRootComponent();

		if (MeshRoot)
		{
			FVector HalfBoxHeight = FVector(0, 0, 300);
			EscapeTriggerZone->SetWorldLocation(MeshRoot->GetComponentLocation() + HalfBoxHeight);
			EscapeTriggerZone->AttachToComponent(MeshRoot, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
	
	ASGPlayerController* Controller = Cast<ASGPlayerController>(GetWorld()->GetFirstPlayerController());
	if (Controller == nullptr)
		return;

	Controller->BindToEscapePod(this);
	
	ASGMainHUD* MainHUD = Cast<ASGMainHUD>(Controller->GetHUD());

	if (MainHUD == nullptr)
		return;

	MainHUD->BindToEndGameInteractEvents(this);
}

bool ASGObjectiveFinalSweep::IsCompleted()
{
	return Super::IsCompleted();
}

void ASGObjectiveFinalSweep::OnStart()
{
	Super::OnStart();

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
	USGHorizontalBoxObjective* HBoxObjective = ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->GetHorizontalBoxObjective(this, 0);
	HBoxObjective->SetValue(FText::FromString(StrEnemiesKilled));
}


void ASGObjectiveFinalSweep::IncrementEnemiesKilled()
{
	CurrentEnemiesKilled++;
}

void ASGObjectiveFinalSweep::EnableEscapeWithPod(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASGPlayerCharacter* Player = Cast<ASGPlayerCharacter>(OtherActor);
	if (Player == nullptr)
	{
		return;
	}
	
	USGObjectiveHandlerSubSystem* ObjectiveHandler = GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>();
	if (ObjectiveHandler == nullptr)
		return;

	ASGPlayerController* Controller = Cast<ASGPlayerController>(Player->GetController());
	if (Controller == nullptr)
		return;

	ASGMainHUD* MainHUD = Cast<ASGMainHUD>(Controller->GetHUD());
	if (MainHUD == nullptr)
		return;

	OnEscapeWithPodEnabled.Broadcast();
}

void ASGObjectiveFinalSweep::DisableEscapeWithPod(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASGPlayerCharacter* Player = Cast<ASGPlayerCharacter>(OtherActor);
	if (Player == nullptr)
	{
		return;
	}
	
	USGObjectiveHandlerSubSystem* ObjectiveHandler = GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>();
	if (ObjectiveHandler == nullptr)
		return;

	ASGPlayerController* Controller = Cast<ASGPlayerController>(Player->GetController());
	if (Controller == nullptr)
		return;

	ASGMainHUD* MainHUD = Cast<ASGMainHUD>(Controller->GetHUD());
	if (MainHUD == nullptr)
		return;

	OnEscapeWithPodDisabled.Broadcast();
}
