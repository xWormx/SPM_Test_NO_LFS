#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SGPlayerCharacter.generated.h"

class ASGPlayerCharacter;
class USGCounterComponentAmmo;
class USGCounterComponentOrbs;
class USGCounterComponentHealth;
class USGHealthComponent;
class UCameraComponent;
class Ujola6902_GunsComponent;
class ASGGrapplingHook;
class ASGGun;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerIsReady, ASGPlayerCharacter*);

class ASGPlayerController;
class USGSaveGame;

UCLASS()
class SPM_TEST_NO_LFS_API ASGPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASGPlayerCharacter();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	float GetRage() const;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Uproperty - Player | Movement", meta=(AllowPrivateAccess="true", ToolTip = "When the player collides with enemies"))
	float PushStrength = 800.0f;

	UPROPERTY(EditAnywhere, Category = "UProperty - Player | Movement", meta=(AllowPrivateAccess="true"))
	float MoveSpeed = 5;

	UPROPERTY(EditAnywhere, Category = "UProperty - Player | Movement", meta=(AllowPrivateAccess="true"))
	float AirControl = 1.f;

	friend class ASGPlayerController;
private:
	float Rage = 66.f; // lol

	UPROPERTY(EditDefaultsOnly, Category = "Uproperty - Player | GrapplingHook", meta=(AllowPrivateAccess="true"))
	USceneComponent* GrapplingHookPosition;
	
	UPROPERTY(EditAnywhere, Category = "Uproperty - Player | GrapplingHook", meta=(AllowPrivateAccess="true"))
	TSubclassOf<ASGGrapplingHook> GrapplingHookClass;

	UPROPERTY(VisibleAnywhere, Category="Uproperty - Player", meta=(AllowPrivateAccess="true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Uproperty - Player | Weapons", meta=(AllowPrivateAccess="true"))
	USkeletalMeshComponent* WeaponMesh;

public:
	FOnPlayerIsReady OnPlayerIsReady;

	UPROPERTY(VisibleAnywhere, Category="Uproperty - Player | Weapons")
	Ujola6902_GunsComponent* GunsComponent;

	UPROPERTY(VisibleAnywhere, Category="Uproperty - Player | Weapons")
	USGCounterComponentAmmo* AmmoComponent;

	UPROPERTY(VisibleAnywhere, Category="Uproperty - Player | GrapplingHook", meta = (ToolTip = "Created during BeginPlay"))
	ASGGrapplingHook* GrapplingHook;

	UPROPERTY(VisibleAnywhere, Category="Uproperty - Player | Health")
	USGHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, Category="Uproperty - Player | Health")
	USGCounterComponentHealth* HealthShieldComponent;

	UPROPERTY(VisibleAnywhere, Category="Uproperty - Player | Orbs")
	USGCounterComponentOrbs* OrbsComponent;

public:
//----Start: Added by Basir
	//Saving functionality
	UFUNCTION(BlueprintCallable)
	struct FPlayerStats GetPlayerStats();

	UFUNCTION(BlueprintCallable)
	void UseSavedGame(FPlayerStats SavedStats);

	ASGPlayerController* PlayerController;
//----End: Added by Basir
};
