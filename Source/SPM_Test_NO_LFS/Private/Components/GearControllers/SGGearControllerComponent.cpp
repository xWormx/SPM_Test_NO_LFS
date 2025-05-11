#include "Components/GearControllers/SGGearControllerComponent.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/Character.h"


USGGearControllerComponent::USGGearControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USGGearControllerComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	SetUpInputs();
	InitGear();
}

void USGGearControllerComponent::SetUpInputs()
{
	APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());

	ULocalPlayer* LocalPlayer = PlayerController ? PlayerController->GetLocalPlayer() : nullptr;
	if (!LocalPlayer)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!InputSubsystem)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("👋GEAR: Adding Input Mapping Context: %s"), *InputMapping->GetName());
	InputSubsystem->AddMappingContext(InputMapping, InputPriority);
}


