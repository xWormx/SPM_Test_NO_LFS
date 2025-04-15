// Fill out your copyright notice in the Description page of Project Settings.


#include "SGGrapplingHook.h"
#include "SGGrappleHeadBase.h"
#include "AssetTypeCategories.h"
#include "SGPlayerController.h"
#include "LandscapeGizmoActiveActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASGGrapplingHook::ASGGrapplingHook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	CableComponent = CreateDefaultSubobject<UCableComponent>(TEXT("CableComponent"));
	CableComponent->SetupAttachment(GetRootComponent());
	CableComponent->NumSegments = 3;
	
	GrappleHeadPosition = CreateDefaultSubobject<USceneComponent>(TEXT("GrappleHeadPosition"));
	GrappleHeadPosition->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ASGGrapplingHook::BeginPlay()
{
	Super::BeginPlay();
	
	Head = GetWorld()->SpawnActor<ASGGrappleHeadBase>(GrappleHeadClass);
	Head->SetActorLocation(GrappleHeadPosition->GetComponentLocation());
	Head->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

	FVector End = UKismetMathLibrary::InverseTransformLocation(GetTransform(), Head->GetActorLocation());
	CableComponent->SetAttachEndToComponent(Head->GetComponentByClass<UStaticMeshComponent>());
	CableComponent->EndLocation = End;
	SetGrappleVisibility(false);

}

// Called every frame
void ASGGrapplingHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDidGrapple)
	{
		// Detta skjuter iväg Grapplehead och uppdateras dess position tills att den är inom 50 enheter
		// av AttachmentPoint
		FVector NewHeadPosition = Head->GetActorLocation();
		
		float DistanceBetweenHeadAndAttachment = FVector::Dist(NewHeadPosition, AttachmentPoint); 
		if (DistanceBetweenHeadAndAttachment < 10)
		{
			bHeadAttached = true;
			bDidGrapple = false;
		}
		else
		{
			// Sålänge avståndet är större än 150 så interpolar vi långsammare mot slutet
			// Annars (när vi är nära målet gör vi det med konstant hastighet)
			if (DistanceBetweenHeadAndAttachment > 150)
			{
				NewHeadPosition = FMath::VInterpTo(Head->GetActorLocation(), AttachmentPoint,
														DeltaTime, 5d);	
			}
			else
			{
				NewHeadPosition = FMath::VInterpConstantTo(Head->GetActorLocation(), AttachmentPoint,
															DeltaTime, 1000);		
			}
				
			Head->SetActorLocation(NewHeadPosition);
		}
	}

}

void ASGGrapplingHook::FireGrapple()
{
	AController* Controller = GetValidController();
	if (Controller == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJPlayerController Not Found!"));
		return;
	}

	FVector ViewLocation;
	FRotator ViewRotation;
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	FHitResult HitResult;
	FVector TraceEnd = ViewLocation + ViewRotation.Vector() * MaxHookRange;

	// Detta är basically SphereTraceByChannel
	bool didHit = GetWorld()->SweepSingleByChannel(HitResult, ViewLocation, TraceEnd,
												FQuat::Identity, ECC_GameTraceChannel1,
													FCollisionShape::MakeSphere(30));
	
	DrawDebugPoint(GetWorld(), TraceEnd, 25, FColor::Red, false, 8);

	if (!didHit)
	{
		ResetGrapple();
		return;
	}
	
	GrappleDirection = TraceEnd - ViewLocation;
	GrappleDirection.Normalize();
	CableLengthWhenAttached = CableComponent->CableLength;
	Head->SetActorRotation(ViewRotation);
	// Skjuter upp karaktären när hooken avlossas för att få ett bra momentum
	Controller->GetCharacter()->LaunchCharacter(FVector(0, 0, 300), true, true);
	// För att karaktären inte ska bromsas när den dras mot väggar
	Controller->GetCharacter()->GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
	Controller->GetCharacter()->GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	Controller->GetCharacter()->GetCharacterMovement()->GravityScale = 0.5f;
	bDidGrapple = didHit;
	AttachmentPoint = HitResult.ImpactPoint;
	PointOfDeparture = GetActorLocation();
	SetGrappleVisibility(true);
	Head->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	DrawDebugSphere(GetWorld(),
					HitResult.ImpactPoint,
					20, 12,
					FColor::Red,
					true);
	DrawDebugLine(GetWorld(),
		GetActorLocation(),
		HitResult.ImpactPoint,
		FColor::Blue,
		false, 8);
}

void ASGGrapplingHook::ResetGrapple()
{
	AController* Controller = GetValidController();
	if (Controller == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJPlayerController Not Found!"));
		return;
	}
	Controller->GetCharacter()->GetCharacterMovement()->GravityScale = 1.5f;
	Controller->GetCharacter()->GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
	
	Head->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	Head->SetActorLocation(GrappleHeadPosition->GetComponentLocation());
	SetGrappleVisibility(false);
	bHeadAttached = false;
}

void ASGGrapplingHook::SetGrappleVisibility(bool bVisibility)
{
	Head->GetComponentByClass<UStaticMeshComponent>()->SetVisibility(bVisibility);
	CableComponent->SetVisibility(bVisibility);
}

void ASGGrapplingHook::SetHeadConstraint(AActor* OtherActor)
{
	Head->SetConstraintTo(OtherActor);
}


AController* ASGGrapplingHook::GetValidController() const
{
	DrawDebugCamera(GetWorld(), GetActorLocation(), GetActorRotation(), 90, 2, FColor::Blue, false, 8);
	APawn* GrappleOwner = Cast<APawn>(GetOwner());
	if (GrappleOwner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner Not Found!"));
		return nullptr;
	}

	return GrappleOwner->GetController();
}


