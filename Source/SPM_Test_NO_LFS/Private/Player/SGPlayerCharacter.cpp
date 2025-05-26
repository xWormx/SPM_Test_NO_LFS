#include "Player/SGPlayerCharacter.h"
#include "SGWeaponsHUD.h"
#include "Blueprint/UserWidget.h"
#include "Gear/Grapple/SGGrapplingHook.h"
#include "Gear/Weapons/SGGun.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enemies/Characters/SGEnemyCharacter.h"

// Sets default values
ASGPlayerCharacter::ASGPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	
	if (WeaponMesh && CameraComponent)
	{
		CameraComponent->SetupAttachment(RootComponent);
		WeaponMesh->SetupAttachment(CameraComponent);
	}
	
	GrapplingHookPosition = CreateDefaultSubobject<USceneComponent>(TEXT("GrapplingHookPosition"));
	GrapplingHookPosition->SetupAttachment(GetRootComponent());

    if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
	{
		CapsuleComp->SetNotifyRigidBodyCollision(true); // Enable hit events
		CapsuleComp->OnComponentHit.AddDynamic(this, &ASGPlayerCharacter::OnComponentHit);
	}
}

// Called when the game starts or when spawned
void ASGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GrapplingHook = GetWorld()->SpawnActor<ASGGrapplingHook>(GrapplingHookClass);
	if (GrapplingHook)
	{
		GrapplingHook->SetActorLocation(GrapplingHookPosition->GetComponentLocation());
		// Grapplinghooken verkar vara omvänt roterad från början, debugkameran visar att den är riktad bakåt
		// Så vi roterar den 180 grader för att den ska vara riktad framåt från början.
		GrapplingHook->AddActorLocalRotation(FRotator(0, 180, 0));
		GrapplingHook->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
		GrapplingHook->SetOwner(this);	
	}

	Guns.SetNum(GunClasses.Num());
	for (int32 i = 0; i < GunClasses.Num(); ++i)
	{
		Guns[i] = GetWorld()->SpawnActor<ASGGun>(GunClasses[i]);
		if (Guns[i]) Guns[i]->SetOwner(this);
		if (WeaponMesh) Guns[i]->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		if (WeaponHUD)
		{
			WeaponHUD->UpdWeaponName(Guns[i]->GetWeaponDisplayName());
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
		UE_LOG(LogTemp, Error, TEXT("Guns array is empty or index out of bounds!"));
	}

	if (WeaponHUDWidgetClass)
	{
		WeaponHUD = CreateWidget<USGWeaponsHUD>(GetWorld(), WeaponHUDWidgetClass);
		if (WeaponHUD)
		{
			WeaponHUD->AddToViewport();
		}
	}
}

// Called every frame
void ASGPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WeaponHUD && Guns[CurrentGunIndex])
	{
		WeaponHUD->UpdWeaponName(Guns[CurrentGunIndex]->GetWeaponDisplayName());
		WeaponHUD->UpdAmmoClip(Guns[CurrentGunIndex]->GetAmmoClip());
		WeaponHUD->UpdAmmoStock(Guns[CurrentGunIndex]->GetAmmoStock());
	}
	
/*
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
	*/
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
	//UE_LOG(LogTemp, Warning, TEXT("Trying to fire gun at index: %d"), CurrentGunIndex);
	ASGGun* Gun = Guns[CurrentGunIndex];
	if (Gun)
	{
		if (Gun->GetAmmoClip() <= 0)
		{
			ReloadGun();
		}
		
		Gun->Fire();
	}
}

void ASGPlayerCharacter::ReloadGun()
{
	UE_LOG(LogTemp, Warning, TEXT("SGPlayerCharacter::ReloadGun()"));
	
	if (Guns[CurrentGunIndex]) Guns[CurrentGunIndex]->Reload();
}

const ASGGun* ASGPlayerCharacter::GetGunRef() const
{
	return Guns[CurrentGunIndex];
}

void ASGPlayerCharacter::SetCurrentGunIndex(int8 NewIndex)
{
	CurrentGunIndex = NewIndex;
}

int8 ASGPlayerCharacter::GetCurrentGunIndex()
{
	return CurrentGunIndex;
}

const TArray<ASGGun*>& ASGPlayerCharacter::GetGuns() const
{
	return Guns;
}

void ASGPlayerCharacter::OnComponentHit([[maybe_unused]] UPrimitiveComponent* HitComponent, AActor* OtherActor, [[maybe_unused]] UPrimitiveComponent* OtherComp, [[maybe_unused]] FVector NormalImpulse, [[maybe_unused]] const FHitResult& Hit)
{
	ASGEnemyCharacter* Enemy = Cast<ASGEnemyCharacter>(OtherActor);
	if (!Enemy)
	{
		return;
	}

	FVector PushDirection = Enemy->GetActorLocation() - GetActorLocation();
	PushDirection.Z = 0;
	Enemy->ApplyPush(PushDirection, PushStrength);
}
