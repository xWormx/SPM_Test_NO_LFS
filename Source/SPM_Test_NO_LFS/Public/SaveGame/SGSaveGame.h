#pragma once

#include "CoreMinimal.h"
#include "Core/SGUpgradeSubsystem.h"
#include "GameFramework/SaveGame.h"
#include "SGSaveGame.generated.h"


USTRUCT(Blueprintable)
struct FPlayerStats
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FTransform PlayerTransform;

	UPROPERTY(EditAnywhere)
	int32 ScorePoints;

	UPROPERTY(EditAnywhere)
	float Health;
	
};

UCLASS(Blueprintable)
class SPM_TEST_NO_LFS_API USGSaveGame : public USaveGame
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere)
	FPlayerStats PlayerStats;

	UPROPERTY(EditAnywhere)
	struct FSGSavedAttributes UpgradeSystemSavedAttributes;
};
