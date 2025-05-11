#include "Components/GearControllers/SGGrapplingHookControllerComponent.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "Gear/Grapple/SGGrapplingHook.h"

USGGrapplingHookControllerComponent::USGGrapplingHookControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	GrapplingHookPosition = CreateDefaultSubobject<USceneComponent>(TEXT("GrapplingHookPosition"));
}

void USGGrapplingHookControllerComponent::SetUpInputs()
{
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(OwnerCharacter->InputComponent);

	if (!Input)
	{
		return;
	}

	Input->BindAction(GrappleInputAction, ETriggerEvent::Triggered, this, &USGGrapplingHookControllerComponent::Fire);

	Super::SetUpInputs();
}

void USGGrapplingHookControllerComponent::InitGear()
{
	if (GrapplingHookPosition && OwnerCharacter.IsValid())
	{
		GrapplingHookPosition->UpdateComponentToWorld();
	}

	GrapplingHook = GetWorld()->SpawnActor<ASGGrapplingHook>(GrapplingHookClass);
	if (GrapplingHook)
	{
		FVector SpawnLocation = OwnerCharacter->GetActorLocation() + GrapplingHookOffset;

		GrapplingHook->SetActorLocation(SpawnLocation);
		GrapplingHook->AddActorLocalRotation(FRotator(0, 180, 0));

		GrapplingHook->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
		GrapplingHook->SetOwner(GetOwner());
	}
}
void USGGrapplingHookControllerComponent::Fire()
{
	GrapplingHook->FireGrapple();
}


