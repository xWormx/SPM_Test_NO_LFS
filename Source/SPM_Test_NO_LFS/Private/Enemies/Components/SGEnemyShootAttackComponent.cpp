

#include "Enemies/Components/SGEnemyShootAttackComponent.h"

#include "Enemies/Components/SGEnemyProjectile.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/SGObjectPoolSubsystem.h"

USGEnemyShootAttackComponent::USGEnemyShootAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USGEnemyShootAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void USGEnemyShootAttackComponent::StartAttack(AActor* Target)
{
	if (!Target || !bCanAttack || !OwnerCharacter)
	{
		return;
	}
	Super::StartAttack(Target);
	PerformAttack(Target);
}

void USGEnemyShootAttackComponent::PerformAttack(AActor* Target)
{
	if (!ProjectileClass)
	{
		return;
	}
	
	FVector SpawnLocation = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector();
	
    FVector Direction = (Target->GetActorLocation() - SpawnLocation).GetSafeNormal();
    FRotator SpawnRotation = Direction.Rotation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = OwnerCharacter;
    SpawnParams.Instigator = OwnerCharacter;

	//Sound
	if (ShootSound)
	{
		UGameplayStatics::SpawnSoundAttached(ShootSound, OwnerCharacter->GetMesh(), TEXT("ShootSound"));
	}
	
    GetWorld()->SpawnActor<ASGEnemyProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
}

