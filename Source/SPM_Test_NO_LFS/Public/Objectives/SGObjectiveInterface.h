/*

Written by
----------------------
- Carl-Johan Larson Eliasson
- cael7567
	
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SGObjectiveInterface.generated.h"

UENUM(BlueprintType, meta = (BitFlags))
enum class EObjectiveType : uint8
{
	EOT_None           = 0 UMETA(DisplayName = "None"),
	EOT_KillAllEnemies = 1 << 0 UMETA(DisplayName = "Kill All Enemies"),
	EOT_Collect        = 1 << 1 UMETA(DisplayName = "Collect"),
	EOT_DefendThePod   = 1 << 2 UMETA(DisplayName = "Defend the Pod"),
	EOT_PodArrival     = 1 << 3 UMETA(DisplayName = "Pod Arrival"),
	EOT_FinalSweep     = 1 << 4 UMETA(DisplayName = "Final Sweep")
};
ENUM_CLASS_FLAGS(EObjectiveType)
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USGObjectiveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPM_TEST_NO_LFS_API ISGObjectiveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual EObjectiveType GetObjectiveType() = 0;
};
