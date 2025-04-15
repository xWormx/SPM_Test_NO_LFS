#pragma once

#include "CoreMinimal.h"
#include "SGPickUp.h"
#include "SGPickUpHealthPack.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API ASGPickUpHealthPack : public ASGPickUp
{
	GENERATED_BODY()

public:
	ASGPickUpHealthPack();
	
protected:
	UPROPERTY(EditAnywhere, Category = "PickUp Properties")
	class UStaticMeshComponent* HealthPacketMesh;
};