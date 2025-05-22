#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "USGISaveGame.h"
#include "SGSaveGame.generated.h"


class USGHealthComponent;
class ASGGun;

USTRUCT(Blueprintable)
struct FPlayerStats
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FTransform PlayerTransform;

	UPROPERTY(EditAnywhere)
	int32 ScorePoints;
	
};

UCLASS(Blueprintable)
class SPM_TEST_NO_LFS_API USGSaveGame : public USaveGame
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere)
	FPlayerStats PlayerStats;
};
