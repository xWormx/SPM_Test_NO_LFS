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
	TSubclassOf<ASGGun> PlayerGun;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USGHealthComponent> PlayerHealth;
};

UCLASS(Blueprintable)
class SPM_TEST_NO_LFS_API USGSaveGame : public USaveGame
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere)
	FPlayerStats PlayerStats;

	/*UFUNCTION(BlueprintCallable)
	virtual void LoadGameFromSlot();*/

	/*virtual void LoadGameData_Implementation(bool Async) override;

	virtual void SaveGameData_Implementation(bool Async) override;

	virtual FPlayerStats GetGameData_Implementation() override;*/
};
