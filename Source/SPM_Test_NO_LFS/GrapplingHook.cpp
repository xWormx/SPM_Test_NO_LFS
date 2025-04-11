// Fill out your copyright notice in the Description page of Project Settings.


#include "GrapplingHook.h"

#include "CJPlayerController.h"
#include "LandscapeGizmoActiveActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AGrapplingHook::AGrapplingHook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	CableComponent = CreateDefaultSubobject<UCableComponent>(TEXT("CableComponent"));
	CableComponent->SetupAttachment(GetRootComponent());
	CableComponent->NumSegments = 3;

	GrappleHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrappleHead"));
	GrappleHead->SetupAttachment(GetRootComponent());
	
}

// Called when the game starts or when spawned
void AGrapplingHook::BeginPlay()
{
	Super::BeginPlay();
	CableComponent->SetVisibility(true);
//	CableComponent->EndLocation = GrappleHead->GetComponentLocation();
	FVector End = UKismetMathLibrary::InverseTransformLocation(GetTransform(), GrappleHead->GetComponentLocation());
	CableComponent->EndLocation = End;
	CableComponent->SetAttachEndToComponent(GrappleHead);
}

// Called every frame
void AGrapplingHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDidGrapple)
	{
		FVector NewPosition = FMath::VInterpTo(GrappleHead->GetComponentLocation(), AttachmentPoint,
													GetWorld()->GetDeltaSeconds(), 5);
		if (FVector::Dist(NewPosition, AttachmentPoint) < 50)
		{
			bIsActivated = true;
			bDidGrapple = false;
			GrappleHead->SetWorldLocation(GetActorLocation());
			GrappleHead->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			
		}
		else
		{
			GrappleHead->SetWorldLocation(NewPosition);
		}
		
		//GrappleHead->SetWorldLocation(AttachmentPoint);
	}
}

void AGrapplingHook::FireGrapple()
{
	DrawDebugCamera(GetWorld(), GetActorLocation(), GetActorRotation(), 90, 2, FColor::Blue, false, 8);
	APawn* GrappleOwner = Cast<APawn>(GetOwner());
	if (GrappleOwner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner Not Found!"));
		return;
	}

	ACJPlayerController* PlayerController = Cast<ACJPlayerController>(GrappleOwner->GetController());
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJPlayerController Not Found!"));
		return;
	}	

	FVector ViewLocation;
	FRotator ViewRotation;
	PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);
	FHitResult HitResult;
	FVector TraceEnd = ViewLocation + ViewRotation.Vector() * MaxHookRange;
	bool didHit = GetWorld()->LineTraceSingleByChannel(HitResult, ViewLocation, TraceEnd, ECC_GameTraceChannel1);
	
	DrawDebugPoint(GetWorld(), TraceEnd, 25, FColor::Red, false, 8);

	if (!didHit)
	{
		Deactivate();
		return;
	}
	PlayerController->GetCharacter()->LaunchCharacter(FVector(0, 0, 300), true, true);
	PlayerController->GetCharacter()->GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
	bDidGrapple = didHit;
	AttachmentPoint = HitResult.ImpactPoint;
	PointOfDeparture = GetActorLocation();
	// Gör om ImpactPoint till cablecomponents local space så att punkten blir korrekt.
	FVector InverseImpactPoint = UKismetMathLibrary::InverseTransformLocation(GetTransform(), HitResult.ImpactPoint);
	//CableComponent->EndLocation = InverseImpactPoint;
	CableComponent->SetVisibility(true);
	GrappleHead->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	
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

void AGrapplingHook::Deactivate()
{
	bIsActivated = false;
	//CableComponent->EndLocation = FVector::ZeroVector;
	//CableComponent->SetVisibility(false);
	//GrappleHead->SetRelativeLocation(FVector(0, 0, 0));
}


