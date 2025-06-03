#include "Objectives/SGObjectiveCollectEmAll.h"
#include "Objectives/SGObjectiveToolTipWidget.h"
#include "Core/SGObjectiveHandlerSubSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Objectives/SGGameObjectivesHandler.h"
#include "Objectives/SGHorizontalBoxObjective.h"
#include "Pickups/SGPickUpObjectiveCollect.h"
#include "Utils/SGObjectPoolSubsystem.h"

void ASGObjectiveCollectEmAll::OnStart()
{
	Super::OnStart();
	FString StrCollected = FString::Printf(TEXT("%d/%d"), CurrentCollected, CollectGoal);

	ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->CreateProgressTextElement(
			GetObjectiveID(),
			FText::FromString(ObjectiveProgressText[0]),
			FText::FromString(StrCollected));
	
	ShowCollectables();
}

bool ASGObjectiveCollectEmAll::IsCompleted()
{
	return CurrentCollected == CollectGoal;
}

void ASGObjectiveCollectEmAll::OnCompleted()
{
	Super::OnCompleted();
	
	SetCurrentProgressElementCompleted("Completed!");
	
	USGHorizontalBoxObjective* HBoxObjective = ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->GetHorizontalBoxObjective(this, 0);
	
	HBoxObjective->PlayAnimationValueCompleted();
	
	HideCollectables();
}

void ASGObjectiveCollectEmAll::Update()
{
	CurrentCollected++;
	
	FString StrCollected = FString::Printf(TEXT("%d/%d"), CurrentCollected, CollectGoal);
	
	USGHorizontalBoxObjective* HBoxObjective = ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->GetHorizontalBoxObjective(this, 0);
	
	HBoxObjective->SetValue(FText::FromString(StrCollected));
}

void ASGObjectiveCollectEmAll::ShowCollectables() const
{
	if (!ensureMsgf(CollectableClass, TEXT("CollectableClass is not set in ASGObjectiveCollectEmAll")))
	{
		return;
	}
	USGObjectPoolSubsystem* ObjectPool = GetGameInstance()->GetSubsystem<USGObjectPoolSubsystem>();

	for (int i = 0; i < ObjectPool->GetPoolSize(CollectableClass); ++i)
	{
		TSubclassOf<AActor> BaseClass = CollectableClass;
		
		ASGPickUpObjectiveCollect* Collectable = Cast<ASGPickUpObjectiveCollect>(ObjectPool->GetPooledObject(BaseClass));
		
		Collectable->ReturnToStartLocation();
	}
}

void ASGObjectiveCollectEmAll::HideCollectables() const
{
	TArray<AActor*> AllCollectables;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASGPickUpObjectiveCollect::StaticClass(), AllCollectables);
	
	USGObjectPoolSubsystem* ObjectPool = GetGameInstance()->GetSubsystem<USGObjectPoolSubsystem>();
	
	for (AActor* Actor : AllCollectables)
	{
		ASGPickUpObjectiveCollect* Collectable = Cast<ASGPickUpObjectiveCollect>(Actor);
		if (!Collectable)
		{
			continue;
		}
		
		ObjectPool->ReturnObjectToPool(Collectable);
	}
}
