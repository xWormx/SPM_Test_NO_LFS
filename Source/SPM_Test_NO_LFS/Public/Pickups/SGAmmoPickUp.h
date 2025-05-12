#pragma once

#include "CoreMinimal.h"
#include "Pickups/SGPickUp.h"
#include "SGAmmoPickUp.generated.h"

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	AssaultRifle, Shotgun, GrenadeLauncher
};

UCLASS()
class SPM_TEST_NO_LFS_API ASGAmmoPickUp : public ASGPickUp
{
	GENERATED_BODY()

public:
	ASGAmmoPickUp();
	virtual void OnPickup_Implementation() override;

protected:
	virtual void BeginPlay() override;

private:
	class ASGPlayerCharacter* PlayerCharacter;

	UPROPERTY(VisibleAnywhere, Category="design")
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	class UNiagaraSystem* PickUpEffect;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design",meta=(AllowPrivateAccess="true"))
	EAmmoType AmmoType = EAmmoType::AssaultRifle;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design",meta=(AllowPrivateAccess="true"))
	int32 AmmoAmount = 1;
};