#include "Objectives/SGObjectiveCollectEmAll.h"

#include "Kismet/GameplayStatics.h"
#include "Objectives/SGGameObjectivesHandler.h"
#include "Objectives/SGHorizontalBoxObjective.h"
#include "Pickups/SGPickUpObjectiveCollect.h"
#include "Utils/SGObjectPoolSubsystem.h"

void ASGObjectiveCollectEmAll::OnStart(ASGGameObjectivesHandler* ObjectiveHandler)
{
	Super::OnStart(ObjectiveHandler);
	FString StrCollected = FString::Printf(TEXT("%d/%d"), CurrentCollected, CollectGoal);
	ObjectiveHandler->GetObjectiveToolTipWidget()->AddProgressTextElement(FText::FromString("Fueldrods:"), FText::FromString(StrCollected));

	ShowCollectables();
}

bool ASGObjectiveCollectEmAll::IsCompleted(ASGGameObjectivesHandler* ObjectiveHandler)
{
	return CurrentCollected == CollectGoal;
}

void ASGObjectiveCollectEmAll::OnCompleted(ASGGameObjectivesHandler* ObjectiveHandler)
{
	Super::OnCompleted(ObjectiveHandler);

	USGHorizontalBoxObjective* CollectProgressText = ObjectiveHandler->GetObjectiveToolTipWidget()->GetHorizontalBoxAtIndex(1);
	CollectProgressText->ShowSucceed();
	CollectProgressText->SetKeyAndValueOpacity(0.5);
	CollectProgressText->SetValue(FText::FromString("Completed!"));

	HideCollectables();
}

void ASGObjectiveCollectEmAll::Update(ASGGameObjectivesHandler* ObjectiveHandler)
{
	CurrentCollected++;
	FString StrCollected = FString::Printf(TEXT("%d/%d"), CurrentCollected, CollectGoal);
	//SetCurrentProgressText(StrCollected);
	
	if (USGHorizontalBoxObjective* CollectProgressText = ObjectiveHandler->GetObjectiveToolTipWidget()->GetHorizontalBoxAtIndex(1))
		CollectProgressText->SetValue(FText::FromString(StrCollected));
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
