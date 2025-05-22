#include "Player/SGPlayerCharacter.h"

#include "jola6902_GunsComponent.h"

#include "SPM_Test_NO_LFS.h"

#include "Gear/Grapple/SGGrapplingHook.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/SGGameInstance.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Player/SGPlayerController.h"
#include "SaveGame/SGSaveGame.h"

// Sets default values
ASGPlayerCharacter::ASGPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerController = Cast<ASGPlayerController>(GetController());

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

	// jola6902_GunsComponent coupling
	GunsComponent = CreateDefaultSubobject<Ujola6902_GunsComponent>(TEXT("GunsComponent"));
}

// Called when the game starts or when spawned
void ASGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	USGGameInstance* GameIns = Cast<USGGameInstance>(GetGameInstance());

	if (GameIns)
	{
		UseSavedGame(GameIns->GetSaveGame()->PlayerStats);
	}
	else
	{
		BASIR_LOG(Warning, TEXT("GameInstance was not found!"));
	}
	
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
}

// Called every frame
void ASGPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
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

FPlayerStats ASGPlayerCharacter::GetPlayerStats()
{
	FPlayerStats PlayerStats;
	PlayerStats.PlayerTransform = GetActorTransform();

	if (PlayerController)
	{
		PlayerStats.ScorePoints = PlayerController->GetScorePoint();
	}
	else
	{
		PlayerStats.ScorePoints = 0;
	}
	return PlayerStats;
}

void ASGPlayerCharacter::UseSavedGame(FPlayerStats SavedStats)
{
	SetActorTransform(SavedStats.PlayerTransform);
	if (PlayerController)
	{
		PlayerController->SetScorePoint(SavedStats.ScorePoints);
	}
}


