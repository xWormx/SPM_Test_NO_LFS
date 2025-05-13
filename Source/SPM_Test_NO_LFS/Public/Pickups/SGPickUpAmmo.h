#pragma once

#include "CoreMinimal.h"
#include "Pickups/SGPickUp.h"
#include "SGPickUpAmmo.generated.h"

class ASGGun;

UCLASS()
class SPM_TEST_NO_LFS_API ASGPickUpAmmo : public ASGPickUp
{
	GENERATED_BODY()

public:
	ASGPickUpAmmo();

	bool IsCompatibleWithGun(const TSubclassOf<ASGGun>& Gun) const;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UPROPERTY - PickUp", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* AmmoMesh;

	//Gör till TArray alt TSet om flera vapen kan använda samma ammo
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UPROPERTY - PickUp",meta=(AllowPrivateAccess="true"))
	TSubclassOf<ASGGun> GunType;
};