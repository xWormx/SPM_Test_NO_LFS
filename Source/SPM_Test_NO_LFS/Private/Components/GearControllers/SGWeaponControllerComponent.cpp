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
	if (UCameraComponent* OwnerCamera = Cast<UCameraComponent>(GetOwner()->GetComponentByClass(UCameraComponent::StaticClass())))
	{
		WeaponMesh->AttachToComponent(OwnerCamera, FAttachmentTransformRules::KeepRelativeTransform);
		WeaponMesh->SetRelativeLocationAndRotation(WeaponMeshLocationOffset, WeaponMeshRotationOffset);

		if (!ensureMsgf(WeaponMesh, TEXT("WeaponMesh is not set!")))
		{
			return;
		}

	}

	Guns.SetNum(GunClasses.Num());
	
	for (int32 i = 0; i < GunClasses.Num(); ++i)
	{
		ASGGun* Gun = Guns[i] = GetWorld()->SpawnActor<ASGGun>(GunClasses[i]);
		if (!Gun)
		{
			continue;
		}
		Gun->SetOwner(GetOwner());
		Gun->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);		
	}
	
	/*if (Guns.Num() > 0 && Guns.IsValidIndex(CurrentGunIndex))
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
	}*/
}

void USGWeaponControllerComponent::SetUpInputs()
{
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(OwnerCharacter->InputComponent);

	if (!Input)
	{
		return;
	}	

	Input->BindAction(FireGunInputAction, ETriggerEvent::Triggered, this, &USGWeaponControllerComponent::OnFireButtonPressed);
	Input->BindAction(StopFireGunInputAction, ETriggerEvent::Triggered, this, &USGWeaponControllerComponent::OnFireButtonReleased);

	Input->BindAction(SwapWeapon1InputAction, ETriggerEvent::Triggered, this, &USGWeaponControllerComponent::OnSwapWeaponKey1Pressed);
	Input->BindAction(SwapWeapon2InputAction, ETriggerEvent::Triggered, this, &USGWeaponControllerComponent::OnSwapWeaponKey2Pressed);
	Input->BindAction(SwapWeapon3InputAction, ETriggerEvent::Triggered, this, &USGWeaponControllerComponent::OnSwapWeaponKey3Pressed);

	Input->BindAction(SwapWeaponMouseWheelInputAction, ETriggerEvent::Triggered, this, &USGWeaponControllerComponent::OnSwapWeaponMouseWheel);

	InputPriority = 2;
	
	Super::SetUpInputs();
}

void USGWeaponControllerComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	HandleFiring(); // OK att ligga i Tick(), kollar bara 2st bools
}

void USGWeaponControllerComponent::FireGun()
{
	if (ASGGun* Gun = Guns[CurrentGunIndex])
	{
		bCanFire = false;
		Gun->Fire();
		const float FireRate = Gun->GetFireRate();
		GetWorld()->GetTimerManager().SetTimer(CanFireAgainTimer, this, &USGWeaponControllerComponent::CanFireAgain,  FireRate, false);
	}
}

void USGWeaponControllerComponent::SetCurrentGunIndex(int8 NewIndex)
{
	CurrentGunIndex = NewIndex;
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
	CurrentGunIndex = 0;
}

void USGWeaponControllerComponent::OnSwapWeaponKey2Pressed(const FInputActionValue& Value)
{
	CurrentGunIndex = 1;
}

void USGWeaponControllerComponent::OnSwapWeaponKey3Pressed(const FInputActionValue& Value)
{
	CurrentGunIndex = 2;
}

void USGWeaponControllerComponent::OnSwapWeaponMouseWheel(const FInputActionValue& Value)
{	
	const int32 NumberOfGuns = Guns.Num();
	if (NumberOfGuns == 0)
	{
		return;
	}

	const float ScrollValue = Value.Get<float>();

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
}
