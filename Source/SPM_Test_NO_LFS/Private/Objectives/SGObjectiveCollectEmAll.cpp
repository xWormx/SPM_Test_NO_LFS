#include "Objectives/SGObjectiveCollectEmAll.h"

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
	HorizontalBoxProgressElement.Add(ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->CreateProgressTextElement(FText::FromString(ObjectiveProgressText[0]), FText::FromString(StrCollected)));
	ShowCollectables();
}

bool ASGObjectiveCollectEmAll::IsCompleted()
{
	return CurrentCollected == CollectGoal;
}

void ASGObjectiveCollectEmAll::OnCompleted()
{
	Super::OnCompleted();

	USGHorizontalBoxObjective* CollectProgressText = ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->GetHorizontalBoxAtIndex(1);

	SetCurrentProgressElementCompleted("Completed!");

	HideCollectables();
}

void ASGObjectiveCollectEmAll::Update()
{
	CurrentCollected++;
	FString StrCollected = FString::Printf(TEXT("%d/%d"), CurrentCollected, CollectGoal);
	HorizontalBoxProgressElement[0]->SetValue(FText::FromString(StrCollected));
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
