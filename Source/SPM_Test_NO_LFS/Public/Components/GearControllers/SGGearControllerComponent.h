#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SGGearControllerComponent.generated.h"

UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, RestrictedToClasses = "Character"))
class SPM_TEST_NO_LFS_API USGGearControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USGGearControllerComponent();

protected:
	virtual void BeginPlay() override;

	virtual void InitGear(){};

	virtual void SetUpInputs();

	UFUNCTION()
	virtual void Fire(){}

protected:
	UPROPERTY(EditAnywhere, Category = "UProperty - Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* InputMapping;

	uint32 InputPriority = 1;
	
	UPROPERTY()
	TWeakObjectPtr<ACharacter> OwnerCharacter;

};

