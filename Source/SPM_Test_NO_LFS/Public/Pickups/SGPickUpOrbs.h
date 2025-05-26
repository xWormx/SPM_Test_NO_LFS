#pragma once

#include "CoreMinimal.h"
#include "Pickups/SGPickUp.h"
#include "SGPickUpOrbs.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API ASGPickUpOrbs : public ASGPickUp
{
	GENERATED_BODY()
protected:

	virtual void BeginPlay() override;
public:
	ASGPickUpOrbs();

	virtual void Tick(float DeltaSeconds) override;
	
protected:	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UPROPERTY - PickUp")
	class ASGPlayerCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, Category = "UPROPERTY - PickUp")
	float InterpSpeed;
	
	UPROPERTY(EditAnywhere, Category = "UPROPERTY - PickUp")
	class UNiagaraComponent* OrbEffect;
};