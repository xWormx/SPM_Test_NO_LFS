#pragma once

#include "CoreMinimal.h"
#include "SGGearControllerComponent.h"
#include "SGGrapplingHookControllerComponent.generated.h"

class UInputAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPM_TEST_NO_LFS_API USGGrapplingHookControllerComponent : public USGGearControllerComponent
{
	GENERATED_BODY()

public:
	USGGrapplingHookControllerComponent();

protected:
	virtual void InitGear() override;
	virtual void Fire() override;
	virtual void SetUpInputs() override;

private:
	UPROPERTY(EditAnywhere, Category = "UProperty - Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* GrappleInputAction;

	UPROPERTY(EditAnywhere, Category = "UProperty - Input", meta = (AllowPrivateAccess = "true"))
	FVector GrapplingHookOffset = FVector(40.f,-50.f,40.f);

	UPROPERTY(EditAnywhere, Category = "UProperty - Input", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ASGGrapplingHook> GrapplingHookClass;

	UPROPERTY()
	ASGGrapplingHook* GrapplingHook;

	UPROPERTY()
	USceneComponent* GrapplingHookPosition;
};
