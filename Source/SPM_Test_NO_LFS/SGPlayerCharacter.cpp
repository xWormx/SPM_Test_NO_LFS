#include "SGPlayerCharacter.h"
#include "SGGrapplingHook.h"
#include "SGGun.h"
#include "MaterialHLSLTree.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASGPlayerCharacter::ASGPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GrapplingHookPosition = CreateDefaultSubobject<USceneComponent>(TEXT("GrapplingHookPosition"));
	GrapplingHookPosition->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ASGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GrapplingHook = GetWorld()->SpawnActor<ASGGrapplingHook>(GrapplingHookClass);
	GrapplingHook->SetActorLocation(GrapplingHookPosition->GetComponentLocation());
	// Grapplinghooken verkar vara omvänt roterad från början, debugkameran visar att den är riktad bakåt
	// Så vi roterar den 180 grader för att den ska vara riktad framåt från början.
	GrapplingHook->AddActorLocalRotation(FRotator(0, 180, 0));
	GrapplingHook->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
	GrapplingHook->SetOwner(this);

	Gun = GetWorld()->SpawnActor<ASGGun>(GunClass);
	if (Gun)
	{
		// WeaponSocket == bone-socket där vapnet ska sitta fast
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Gun->SetOwner(this);
	}
}

// Called every frame
void ASGPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GrapplingHook->HeadAttached())
	{
		FVector NewPosition = FMath::VInterpTo(GetActorLocation(),
			GrapplingHook->GetAttachmentPoint(),
			GetWorld()->GetDeltaSeconds(),
			GrapplingHook->GetDragSpeed());
		
		UE_LOG(LogTemp, Warning, TEXT("DELTASECONDS: %f"), DeltaTime);
		float DistanceToGrapplePoint = FVector::Distance(GrapplingHook->GetAttachmentPoint(), NewPosition); 
		UE_LOG(LogTemp, Warning, TEXT("Grapple Location: %f"), DistanceToGrapplePoint);

		if (DistanceToGrapplePoint < 150)
		{
			GrapplingHook->ResetGrapple();
			// Om hook riktningen är uppåt så lägg till lite extra kraft uppåt!
			if (GrapplingHook->GetGrappleDirectionNormalized().Z > 0)
			{
				FVector Impuls = GrapplingHook->GetGrappleDirectionNormalized() * 80000;
				float ExtraUpwardsImpuls = 50000;
				Impuls.Z += ExtraUpwardsImpuls;
				GetCharacterMovement()->AddImpulse(Impuls);	
			}
			else
			{
				FVector Impuls = GrapplingHook->GetGrappleDirectionNormalized() * 80000;
				GetCharacterMovement()->AddImpulse(Impuls);
			}
			
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, GrapplingHook->GetGrappleDirectionNormalized().ToString());
		}
		else
		{
			
			SetActorLocation(NewPosition);
		}
		
				
	}
}

// Called to bind functionality to input
void ASGPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASGPlayerCharacter::FireGrapple()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Grapple"));
	GrapplingHook->FireGrapple();
	//GetCharacterMovement()->GravityScale = 0.5;
}

void ASGPlayerCharacter::FireGun()
{
	if (Gun) Gun->Fire();
}

const ASGGun* ASGPlayerCharacter::GetGunRef() const
{
	return Gun;
}


