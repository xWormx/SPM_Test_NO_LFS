#include "Enemies/Characters/SGEnemyCharacter.h"

#include "SPM_Test_NO_LFS.h"
#include "Components/SGHealthComponent.h"
#include "Core/SGUpgradeSubsystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utils/SGObjectPoolSubsystem.h"
#include "Utils/SGPickUpSubsystem.h"

ASGEnemyCharacter::ASGEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	HealthComponent = CreateDefaultSubobject<USGHealthComponent>(TEXT("HealthComponent"));

	Tags.Add("Enemy");
}

void ASGEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnNoHealth.AddDynamic(this, &ASGEnemyCharacter::HandleDeath);
	}
	
	if (USGUpgradeSubsystem* UpgradeSubsystem = GetOwner()->GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>())
	{
		FName Category = TEXT("Hidden");
		FName MaxHealth = TEXT("MaxHealth");
		UpgradeSubsystem->BindAttribute(HealthComponent, MaxHealth, TEXT("EnemyHealth"), Category);
		UpgradeSubsystem->BindDependentAttribute(HealthComponent, TEXT("CurrentHealth"), false, HealthComponent, MaxHealth);
	}
}

void ASGEnemyCharacter::HandleDeath(float NewHealth)
{
	GetGameInstance()->GetSubsystem<USGPickUpSubsystem>()->DropItem(this);

	OnEnemyDied.Broadcast(this);
	OnEnemyDiedObjective.Broadcast(this);

	GetGameInstance()->GetSubsystem<USGObjectPoolSubsystem>()->ReturnObjectToPool(this);
	HealthComponent->SetCurrentHealth(HealthComponent->GetMaxHealth());
}

void ASGEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

USGEnemyAttackComponentBase* ASGEnemyCharacter::GetAttackComponent() const
{
	return AttackComponent;
}

void ASGEnemyCharacter::JumpToLocation(const FVector Destination)
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	FVector CurrentLocation = GetActorLocation();
	FVector ToTarget = Destination - CurrentLocation;

	
	FVector JumpVelocity = ToTarget.GetSafeNormal() * 600.f; 
	JumpVelocity.Z = 420.f; 
	
	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, Destination);
	SetActorRotation(NewRotation);

	LaunchCharacter(JumpVelocity, true, true);

	GetWorld()->GetTimerManager().SetTimer(
		JumpTimerHandle,
		this,
		&ASGEnemyCharacter::AdjustJumpRotation,
		1.f,
		false
		);
}

void ASGEnemyCharacter::AdjustJumpRotation()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void ASGEnemyCharacter::ApplyPush(const FVector& PushDirection, float PushStrength)
{
	// Get current rotation before push
	FRotator CurrentRotation = GetActorRotation();

	// Apply push force
	LaunchCharacter(PushDirection.GetSafeNormal() * PushStrength, true, true);

	// Set up a timer to restore rotation after physics update
	GetWorldTimerManager().SetTimerForNextTick([this, CurrentRotation]()
	{
		SetActorRotation(CurrentRotation);
	});
}

