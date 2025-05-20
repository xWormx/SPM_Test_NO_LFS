#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SGPlayerCharacter.generated.h"

// Forward declarations
class ASGGun;
class USGWeaponsHUD;

UCLASS()
class SPM_TEST_NO_LFS_API ASGPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASGPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	float GetRage() const { return Rage; };
	void FireGrapple();

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
	TSubclassOf<class ASGGrapplingHook> GrapplingHookClass;

	UPROPERTY()
	ASGGrapplingHook* GrapplingHook;

	UPROPERTY(VisibleAnywhere, Category="design")
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, Category="design")
	USkeletalMeshComponent* WeaponMesh;
};
