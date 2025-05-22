#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SGPlayerCharacter.generated.h"


class ASGGrapplingHook;
class ASGGun;
class USGWeaponsHUD;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGrapplingHookReady, ASGGrapplingHook*);

UCLASS()
class SPM_TEST_NO_LFS_API ASGPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASGPlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	float GetRage() const { return Rage; };
	void FireGrapple();

	// jola6902_GunsComponent
	UPROPERTY(VisibleAnywhere)
	class Ujola6902_GunsComponent* GunsComponent;

protected:

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Uproperty - Player", meta=(AllowPrivateAccess="true", ToolTip = "När spelaren kolliderar med fiender"))
	float PushStrength = 800.0f;

private:
	float Rage = 66.f; // lol
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	USceneComponent* GrapplingHookPosition;
	
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	TSubclassOf<ASGGrapplingHook> GrapplingHookClass;

	UPROPERTY()
	ASGGrapplingHook* GrapplingHook;

	UPROPERTY(VisibleAnywhere, Category="design")
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, Category="design")
	USkeletalMeshComponent* WeaponMesh;

public:
	FOnGrapplingHookReady OnGrapplingHookReady;
};
