
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "USGISaveGame.generated.h"

UINTERFACE(Blueprintable)
class SPM_TEST_NO_LFS_API USGISaveGame : public UInterface
{
	GENERATED_BODY()
};

/**
 * This interface should be implemented by actors or objects that support saving.
 */
class SPM_TEST_NO_LFS_API ISGISaveGame
{
	GENERATED_BODY()

public:

	
	// Declare a save function for Blueprints to implement or call
	/*UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SaveGame")
	void LoadGameData(bool Async);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SaveGame")
	void SaveGameData(bool Async);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SaveGame")
	class USGSaveGame* GetGameData() const;*/
};
