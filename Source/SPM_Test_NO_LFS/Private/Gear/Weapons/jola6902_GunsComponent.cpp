// Joel Larsson Wendt || jola6902

#include "Gear/Weapons/jola6902_GunsComponent.h"
#include "Gear/Weapons/SGGun.h"
//#include "UI/Widgets/SGWeaponsHUD.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "SPM_Test_NO_LFS.h"
#include "Player/SGPlayerCharacter.h"
//#include "Blueprint/UserWidget.h"

// Public
Ujola6902_GunsComponent::Ujola6902_GunsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void Ujola6902_GunsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FireGun();
	//UpdateGunsHUD();
}

const TArray<ASGGun*> Ujola6902_GunsComponent::GetGuns() const
{
	return Guns;
}

const ASGGun* Ujola6902_GunsComponent::GetCurrentGun() const
{
	return Guns[CurrentGunIndex];
}

// Protected
void Ujola6902_GunsComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	Guns.SetNum(GunClasses.Num());

	ValidateKeyBindings();
	BindActions();
	SetUpGuns();

	//CreateGunsHUD();

	bCanFire = true;
}

// Fick ibland krasch-problem med garbage collectionen av innehållet i Guns när spelet stängs ner, därav denna funktion.
void Ujola6902_GunsComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (ASGGun* Gun : Guns)
	{
		if (Gun && IsValid(Gun))
		{
			Gun->Destroy();
		}
	}
	Guns.Empty();
}

// Private
// Om karaktären som äger denna komponent har en Gun vald, avfyra sagda Gun (ifall ammo finns, annars ladda om).
void Ujola6902_GunsComponent::FireGun()
{
	if (!bCanFire || !bFireButtonHeld) return;
	
	ASGGun* Gun = Guns[CurrentGunIndex];
	
	if (Gun)
	{
		bCanFire = false;
		
		if (Gun->GetAmmoClip() <= 0)
		{
			ReloadGun();
		}
		
		Gun->Fire();
		float FireRate = Gun->GetFireRate();
		GetWorld()->GetTimerManager().SetTimer(CanFireAgainTimer, this, &Ujola6902_GunsComponent::CanFireAgain, FireRate, false);

		OnFireGun.Broadcast(CurrentGunIndex, Gun);
	}
}

// Existerar för att hämma skjuthastigheten i enlighet med Gun->FireRate
void Ujola6902_GunsComponent::CanFireAgain()
{
	bCanFire = true;

	OnCanFireGun.Broadcast(bCanFire, CurrentGunIndex, Guns[CurrentGunIndex]);
}

void Ujola6902_GunsComponent::ReloadGun()
{
	if (ASGGun* Gun = Guns[CurrentGunIndex])
	{
		Gun->Reload();

		OnReload.Broadcast(CurrentGunIndex, Gun);
	}
}

void Ujola6902_GunsComponent::OnFireButtonPressed([[maybe_unused]] const FInputActionValue& Value)
{
	bFireButtonHeld = true;
}

void Ujola6902_GunsComponent::OnFireButtonReleased([[maybe_unused]] const FInputActionValue& Value)
{
	bFireButtonHeld = false;
}

void Ujola6902_GunsComponent::OnReloadButtonPressed([[maybe_unused]] const FInputActionValue& Value)
{
	ReloadGun();
}

// Hanterar selektionen av vapen genom de ställbara InputActions som läggs i medlemmen Map<UInputAction*, int32> GunIndexKeyBindings i editorn.
void Ujola6902_GunsComponent::OnGunIndexKeyPressed(const FInputActionInstance& Instance)
{
	const UInputAction* TriggeredAction = Instance.GetSourceAction();
	const int32* Index = GunIndexKeyBindings.Find(TriggeredAction);
	
	if (Index && *Index >= 0 && *Index < Guns.Num())
	{
		CurrentGunIndex = *Index;
		JOEL_LOG(Log, TEXT("jola6902_GunsComponent::OnKeyPressed() | Swapped to weapon index %d via input action %s"), *Index, *TriggeredAction->GetName());

		OnSwitchedGun.Broadcast(CurrentGunIndex, Guns[CurrentGunIndex]);
		
		if (ASGPlayerCharacter* PlayerRef = Cast<ASGPlayerCharacter>(Owner))
		{
			switch (CurrentGunIndex)
			{
				case 0:
					if (PlayerRef) PlayerRef->BPWeaponSwapAR();
					break;
				case 1:
					if (PlayerRef) PlayerRef->BPWeaponSwapSG();
					break;
				case 2:
					if (PlayerRef) PlayerRef->BPWeaponSwapGL();
					break;
				default:
					break;
			}
		}
	}
	else
	{
		JOEL_LOG(Error, TEXT("jola6902_GunsComponent::OnKeyPressed() | Invalid gun index %d from input action %s"), *Index, *TriggeredAction->GetName());
	}
}

// Hanterar bytande av vapen med mouse scroll wheel
void Ujola6902_GunsComponent::OnMouseWheelScroll(const FInputActionValue& Value)
{
	float ScrollValue = Value.Get<float>();
	
	if (FMath::IsNearlyZero(ScrollValue)) return;

	if (ScrollValue < 0.f)
	{
		CurrentGunIndex = (CurrentGunIndex + 1) % Guns.Num();
	}
	else if (ScrollValue > 0.f)
	{
		CurrentGunIndex = (CurrentGunIndex - 1 + Guns.Num()) % Guns.Num();
	}

	OnSwitchedGun.Broadcast(CurrentGunIndex, Guns[CurrentGunIndex]);

	if (ASGPlayerCharacter* PlayerRef = Cast<ASGPlayerCharacter>(Owner))
	{
		switch (CurrentGunIndex)
		{
			case 0:
				if (PlayerRef) PlayerRef->BPWeaponSwapAR();
				break;
			case 1:
				if (PlayerRef) PlayerRef->BPWeaponSwapSG();
				break;
			case 2:
				if (PlayerRef) PlayerRef->BPWeaponSwapGL();
				break;
			default:
				break;
		}
	}
}

// Kontrollerar att Map<UInputAction*, int32> GunIndexKeyBindings inte har några index som ligger utanför bounds för Guns och tar bort potentiella problematiska bindings.
void Ujola6902_GunsComponent::ValidateKeyBindings()
{
	TArray<UInputAction*> InvalidKeyBindings;

	for (const auto& Pair : GunIndexKeyBindings)
	{
		if (Pair.Value < 0 || Pair.Value >= Guns.Num())
		{
			JOEL_LOG(Error, TEXT("jola6902_GunsComponent::ValidateKeyBindings() | Removed invalid binding index %d from input action %s"), Pair.Value, *Pair.Key->GetName());
			InvalidKeyBindings.Add(Pair.Key);
		}
	}

	for (const UInputAction* Key : InvalidKeyBindings)
	{
		GunIndexKeyBindings.Remove(Key);
	}
}

// Binder dynamiskt alla ActionBindings som designern valt att använda under runtime. (Både custom GunIndexKeyBindings och de bindings som jag valt). Inget är required.
void Ujola6902_GunsComponent::BindActions()
{
	if (APlayerController* PC = Cast<APlayerController>(Owner->GetInstigatorController()))
	{
		if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PC->InputComponent))
		{
			for (const TPair<UInputAction*, int32>& Pair : GunIndexKeyBindings)
			{
				Input->BindAction(Pair.Key, ETriggerEvent::Triggered, this, &Ujola6902_GunsComponent::OnGunIndexKeyPressed);
			}

			if (MouseWheelScrollInputAction)
			{
				Input->BindAction(MouseWheelScrollInputAction, ETriggerEvent::Triggered, this, &Ujola6902_GunsComponent::OnMouseWheelScroll);
			}

			if (FireGunInputAction)
			{
				Input->BindAction(FireGunInputAction, ETriggerEvent::Started, this, &Ujola6902_GunsComponent::OnFireButtonPressed);
				Input->BindAction(FireGunInputAction, ETriggerEvent::Completed, this, &Ujola6902_GunsComponent::OnFireButtonReleased);
			}

			if (ReloadGunInputAction)
			{
				Input->BindAction(ReloadGunInputAction, ETriggerEvent::Triggered, this, &Ujola6902_GunsComponent::OnReloadButtonPressed);
			}
		}
	}
}

// Skapar alla Guns i GunClasses och sätter komponentens ägare som dess ägare.
void Ujola6902_GunsComponent::SetUpGuns()
{
	USceneComponent* AttachmentComponent = Cast<USceneComponent>(GunsAttachment.GetComponent(Owner));
	
	for (int32 i = 0; i < Guns.Num(); ++i)
	{
		Guns[i] = GetWorld()->SpawnActor<ASGGun>(GunClasses[i]);
		
		if (Guns[i])
		{
			Guns[i]->SetOwner(Owner);
		}
		
		if (AttachmentComponent)
		{
			Guns[i]->AttachToComponent(AttachmentComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
}

// Sätter upp och uppdaterar min widget GunsHUD (som brutits ut och flyttats till Emmas UI overhaul)
/*void Ujola6902_GunsComponent::CreateGunsHUD()
{
	if (GunsHUDWidgetClass)
	{
		GunsHUD = CreateWidget<USGWeaponsHUD>(GetWorld(), GunsHUDWidgetClass);
		
		if (GunsHUD)
		{
			GunsHUD->AddToViewport();
		}
	}
}*/

/*void Ujola6902_GunsComponent::UpdateGunsHUD()
{
	if (GunsHUD && Guns[CurrentGunIndex])
	{
		GunsHUD->UpdWeaponName(Guns[CurrentGunIndex]->GetWeaponDisplayName());
		GunsHUD->UpdAmmoClip(Guns[CurrentGunIndex]->GetAmmoClip());
		GunsHUD->UpdAmmoStock(Guns[CurrentGunIndex]->GetAmmoStock());
	}
}*/