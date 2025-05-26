// Joel Larsson Wendt || jola6902

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "SGEnemySpawnVolume.generated.h"

UCLASS()
class ASGEnemySpawnVolume : public AVolume
{
	GENERATED_BODY()

public:
	ASGEnemySpawnVolume();
	
	UPROPERTY(EditAnywhere)
	int32 AreaIndex;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> ContainedSpawnPoints;

protected:
	virtual void BeginPlay() override;
};