#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SGPlayerCharacter.generated.h"

// Forward declarations
class ASGGun;

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
	void FireGun();
	const ASGGun* GetGunRef() const;
	void SetCurrentGunIndex(int8 NewIndex);
	int8 GetCurrentGunIndex();
	const TArray<ASGGun*>& GetGuns() const;
	
private:
	float Rage = 66.f;
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	USceneComponent* GrapplingHookPosition;
	
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	TSubclassOf<class ASGGrapplingHook> GrapplingHookClass;

	UPROPERTY()
	ASGGrapplingHook* GrapplingHook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<ASGGun>> GunClasses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	TArray<ASGGun*> Guns;
	int8 CurrentGunIndex = 0;
	UPROPERTY(VisibleAnywhere, Category="design")
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, Category="design")
	USkeletalMeshComponent* WeaponMesh;
};
