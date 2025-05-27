// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Components/SGEnemyShootAttackComponent.h"

#include "Enemies/Components/SGEnemyProjectile.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/SGObjectPoolSubsystem.h"


// Sets default values for this component's properties
USGEnemyShootAttackComponent::USGEnemyShootAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
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

    //FVector SpawnLocation = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * 60.f;
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

	/*TSubclassOf<AActor> DropClass = TSubclassOf<ASGEnemyProjectile>();

	Projectile = Cast<ASGEnemyProjectile>(
		GetWorld()->GetGameInstance()->GetSubsystem<USGObjectPoolSubsystem>()->GetPooledObject(DropClass)
		);*/

	//Projectile = GetWorld()->GetGameInstance()->GetSubsystem<USGObjectPoolSubsystem>()->GetPooledObject(EnemyProjectileClass);
    GetWorld()->SpawnActor<ASGEnemyProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	
}

