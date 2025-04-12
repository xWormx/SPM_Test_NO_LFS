// Fill out your copyright notice in the Description page of Project Settings.


#include "CJPlayerCharacter.h"

#include "GrapplingHook.h"
#include "MaterialHLSLTree.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACJPlayerCharacter::ACJPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GrapplingHookPosition = CreateDefaultSubobject<USceneComponent>(TEXT("GrapplingHookPosition"));
	GrapplingHookPosition->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ACJPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GrapplingHook = GetWorld()->SpawnActor<AGrapplingHook>(GrapplingHookClass);
	GrapplingHook->SetActorLocation(GrapplingHookPosition->GetComponentLocation());
	// Grapplinghooken verkar vara omvänt roterad från början, debugkameran visar att den är riktad bakåt
	// Så vi roterar den 180 grader för att den ska vara riktad framåt från början.
	GrapplingHook->AddActorLocalRotation(FRotator(0, 180, 0));
	GrapplingHook->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
	GrapplingHook->SetOwner(this);
}

// Called every frame
void ACJPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GrapplingHook->HeadAttached())
	{
		FVector NewPosition = FMath::VInterpTo(GetActorLocation(),
			GrapplingHook->GetAttachmentPoint(),
			GetWorld()->GetDeltaSeconds(),
			GrapplingHook->GetDragSpeed() * DeltaTime);
		
		float DistanceToGrapplePoint = FVector::Distance(GrapplingHook->GetAttachmentPoint(), NewPosition); 
		UE_LOG(LogTemp, Warning, TEXT("Grapple Location: %f"), DistanceToGrapplePoint);

		if (DistanceToGrapplePoint < 150)
		{
			GrapplingHook->ResetGrapple();
			GetCharacterMovement()->AddImpulse(GrapplingHook->GetGrappleDirectionNormalized() * 80000);
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, GrapplingHook->GetGrappleDirectionNormalized().ToString());
		}
		else
		{
			
			SetActorLocation(NewPosition);
		}
		
				
	}
}

// Called to bind functionality to input
void ACJPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACJPlayerCharacter::FireGrapple()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Grapple"));
	GrapplingHook->FireGrapple();
	//GetCharacterMovement()->GravityScale = 0.5;
}


