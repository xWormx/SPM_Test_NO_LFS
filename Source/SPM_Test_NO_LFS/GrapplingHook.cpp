// Fill out your copyright notice in the Description page of Project Settings.


#include "GrapplingHook.h"

#include "CJPlayerController.h"
#include "LandscapeGizmoActiveActor.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AGrapplingHook::AGrapplingHook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CableComponent = CreateDefaultSubobject<UCableComponent>(TEXT("CableComponent"));
	CableComponent->SetupAttachment(GetRootComponent());
	
}

// Called when the game starts or when spawned
void AGrapplingHook::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrapplingHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

bool AGrapplingHook::TryGrapple(FVector& OutGrapplePoint)
{
	DrawDebugCamera(GetWorld(), GetActorLocation(), GetActorRotation(), 90, 2, FColor::Blue, false, 8);
	APawn* GrappleOwner = Cast<APawn>(GetOwner());
	if (GrappleOwner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner Not Found!"));
		return false;
	}

	ACJPlayerController* PlayerController = Cast<ACJPlayerController>(GrappleOwner->GetController());
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJPlayerController Not Found!"));
		return false;
	}	

	FVector ViewLocation;
	FRotator ViewRotation;
	PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);
	FHitResult HitResult;
	int MaxRange = 1000;
	FVector TraceEnd = ViewLocation + ViewRotation.Vector() * MaxRange;
	bool didHit = GetWorld()->LineTraceSingleByChannel(HitResult, ViewLocation, TraceEnd, ECC_GameTraceChannel1);
	
	DrawDebugPoint(GetWorld(), TraceEnd, 25, FColor::Red, false, 8);

	if (!didHit)
	{
		
		CableComponent->SetVisibility(false);
		CableComponent->EndLocation = FVector::Zero();
		return false;
	}

	DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 20, 12, FColor::Red, true);DrawDebugLine(GetWorld(), GetActorLocation(), HitResult.ImpactPoint, FColor::Blue, false, 8);
	// Gör om ImpactPoint till cablecomponents local space så att punkten blir korrekt.
	CableComponent->SetVisibility(true);
	FVector InverseImpactPoint = UKismetMathLibrary::InverseTransformLocation(GetTransform(), HitResult.ImpactPoint);
	CableComponent->EndLocation = InverseImpactPoint;
	OutGrapplePoint = HitResult.ImpactPoint;
	
	return true;
	
}

void AGrapplingHook::Deactivate()
{
	CableComponent->EndLocation = FVector::ZeroVector;
	CableComponent->SetVisibility(false);	
}


