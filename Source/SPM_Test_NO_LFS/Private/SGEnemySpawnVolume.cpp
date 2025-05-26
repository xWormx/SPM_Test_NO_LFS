// Joel Larsson Wendt || jola6902

#include "SGEnemySpawnVolume.h"
#include "Components/BrushComponent.h"
#include "Enemies/Managers/SGEnemySpawnPoint.h"
#include "Kismet/GameplayStatics.h"

ASGEnemySpawnVolume::ASGEnemySpawnVolume()
{
	GetBrushComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASGEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> AllSpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASGEnemySpawnPoint::StaticClass(), AllSpawnPoints);
	
	UBrushComponent* MyBrushComponent = GetBrushComponent();
	if (!MyBrushComponent)
	{
		return;
	}

	FBox VolumeBounds = MyBrushComponent->Bounds.GetBox();

	for (AActor* SpawnPoint : AllSpawnPoints)
	{
		if (!SpawnPoint) continue;

		FVector PointLocation = SpawnPoint->GetActorLocation();

		if (VolumeBounds.IsInside(PointLocation))
		{
			ContainedSpawnPoints.Add(SpawnPoint);
		}
	}
}