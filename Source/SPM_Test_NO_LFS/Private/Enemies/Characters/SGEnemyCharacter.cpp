#include "Enemies/Characters/SGEnemyCharacter.h"

#include "Enemies/Managers/SGEnemyDropManager.h"
#include "Components/SGHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utils/SGObjectPoolSubsystem.h"

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
}

void ASGEnemyCharacter::HandleDeath(float NewHealth)
{
	//TODO: Ska ändras - temporär lösning
	if (AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(), ASGEnemyDropManager::StaticClass()))
	{
		if (ASGEnemyDropManager* DropManager = Cast<ASGEnemyDropManager>(Actor))
		{
			DropManager->DropItem(this);
		}
	}
	
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

	FVector CurrentLocation = GetActorLocation();
	FVector ToTarget = Destination - CurrentLocation;

	
	FVector JumpVelocity = ToTarget.GetSafeNormal() * 600.f; 
	JumpVelocity.Z = 420.f; 
	
	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, Destination);
	SetActorRotation(NewRotation);

	LaunchCharacter(JumpVelocity, true, true);
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

