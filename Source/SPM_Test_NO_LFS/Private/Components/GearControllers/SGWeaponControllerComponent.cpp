#include "Components/GearControllers/SGWeaponControllerComponent.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "Gear/Weapons/SGGun.h"
#include "Camera/CameraComponent.h"


USGWeaponControllerComponent::USGWeaponControllerComponent()
{	
	PrimaryComponentTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
}

void USGWeaponControllerComponent::InitGear()
{
	UCameraComponent* OwnerCamera = Cast<UCameraComponent>(GetOwner()->GetComponentByClass(UCameraComponent::StaticClass()));

	if (!OwnerCamera)
	{
		return;
	}
	WeaponMesh->AttachToComponent(OwnerCamera, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	WeaponMesh->SetRelativeLocationAndRotation(WeaponMeshLocationOffset, WeaponMeshRotationOffset);	
	
	Guns.SetNum(GunClasses.Num());
	for (int32 i = 0; i < GunClasses.Num(); ++i)
	{
		if (ASGGun* Gun = Guns[i] = GetWorld()->SpawnActor<ASGGun>(GunClasses[i]))
		{
			Gun->SetOwner(GetOwner());
		}
		if (WeaponMesh)
		{
			Guns[i]->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
	if (Guns.Num() > 0 && Guns.IsValidIndex(CurrentGunIndex))
	{
		if (Guns[CurrentGunIndex])
		{
			// Använd senare för att attacha första vapnet vid start, ska sedan byta vapenobjekt vid weapon swap
			// WeaponSocket == bone-socket där vapnet ska sitta fast
			//Guns[CurrentGunIndex]->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		}
	}
	else
	{
		// Handle the case where the Guns array is not populated
		UE_LOG(LogTemp, Error, TEXT("Guns array is empty or index out of bounds!"));
	}
}

void USGWeaponControllerComponent::SetUpInputs()
{
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(OwnerCharacter->InputComponent);

	if (!Input)
	{
		return;
	}

	// Shooting
	Input->BindAction(FireGunInputAction, ETriggerEvent::Triggered, this, &USGWeaponControllerComponent::OnFireButtonPressed);
	Input->BindAction(StopFireGunInputAction, ETriggerEvent::Triggered, this, &USGWeaponControllerComponent::OnFireButtonReleased);
	Input->BindAction(SwapWeapon1InputAction, ETriggerEvent::Triggered, this, &USGWeaponControllerComponent::OnSwapWeaponKey1Pressed);
	Input->BindAction(SwapWeapon2InputAction, ETriggerEvent::Triggered, this, &USGWeaponControllerComponent::OnSwapWeaponKey2Pressed);
	Input->BindAction(SwapWeapon3InputAction, ETriggerEvent::Triggered, this, &USGWeaponControllerComponent::OnSwapWeaponKey3Pressed);
	Input->BindAction(SwapWeaponMouseWheelInputAction, ETriggerEvent::Triggered, this, &USGWeaponControllerComponent::OnSwapWeaponMouseWheel);
	
	Super::SetUpInputs();
}


void USGWeaponControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	HandleFiring(); // OK att ligga i Tick(), kollar bara 2st bools
}

void USGWeaponControllerComponent::FireGun()
{
	const ASGGun* Gun = GetGunRef();
	if (Gun)
	{
		bCanFire = false;
		if (Guns[CurrentGunIndex]) 
		{
			Guns[CurrentGunIndex]->Fire();
		}
		float FireRate = Gun->GetFireRate();	
		GetWorld()->GetTimerManager().SetTimer(CanFireAgainTimer, this, &USGWeaponControllerComponent::CanFireAgain, FireRate, false);
	}
}

const ASGGun* USGWeaponControllerComponent::GetGunRef() const
{
	return Guns[CurrentGunIndex];
}

void USGWeaponControllerComponent::SetCurrentGunIndex(int8 NewIndex)
{
	CurrentGunIndex = NewIndex;
}

int8 USGWeaponControllerComponent::GetCurrentGunIndex()
{
	return CurrentGunIndex;
}

const TArray<ASGGun*>& USGWeaponControllerComponent::GetGuns() const
{
	return Guns;
}

void USGWeaponControllerComponent::OnFireButtonPressed(const FInputActionValue& Value)
{
	bIsFiring = true;
}

void USGWeaponControllerComponent::OnFireButtonReleased(const FInputActionValue& Value)
{
	bIsFiring = false;
}

void USGWeaponControllerComponent::HandleFiring()
{
	if (!bIsFiring || !bCanFire)
	{
		return;
	}
	FireGun();
}

void USGWeaponControllerComponent::CanFireAgain()
{
	bCanFire = true;
}

void USGWeaponControllerComponent::OnSwapWeaponKey1Pressed(const FInputActionValue& Value)
{
	if (!OwnerCharacter.IsValid())
	{
		return;
	}
	SetCurrentGunIndex(0);
}

void USGWeaponControllerComponent::OnSwapWeaponKey2Pressed(const FInputActionValue& Value)
{
	if (!OwnerCharacter.IsValid())
	{
		return;
	}
	SetCurrentGunIndex(1);
}

void USGWeaponControllerComponent::OnSwapWeaponKey3Pressed(const FInputActionValue& Value)
{
	if (!OwnerCharacter.IsValid())
	{
		return;
	}
	SetCurrentGunIndex(2);
}

void USGWeaponControllerComponent::OnSwapWeaponMouseWheel(const FInputActionValue& Value)
{
	if (!OwnerCharacter.IsValid())
	{
		return;
	}
	int32 NumberOfGuns = Guns.Num();
	if (NumberOfGuns == 0) return;

	float ScrollValue = Value.Get<float>();

	if (FMath::IsNearlyZero(ScrollValue))
	{
		return;
	}

	if (ScrollValue > 0.f)
	{
		CurrentGunIndex = (CurrentGunIndex + 1) % NumberOfGuns;
	}
	else if (ScrollValue < 0.f)
	{
		CurrentGunIndex = (CurrentGunIndex - 1 + NumberOfGuns) % NumberOfGuns;
	}

	SetCurrentGunIndex(CurrentGunIndex);
}

