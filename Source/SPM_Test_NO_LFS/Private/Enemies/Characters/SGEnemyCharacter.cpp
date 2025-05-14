// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Managers/SGEnemyDropManager.h"
#include "Components/SGHealthComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASGEnemyCharacter::ASGEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthComponent = CreateDefaultSubobject<USGHealthComponent>(TEXT("HealthComponent"));

	Tags.Add("Enemy");
}

// Called when the game starts or when spawned
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
	UE_LOG(LogTemp, Warning, TEXT("Enemy [%s] died. Broadcasting OnEnemyDied"), *GetName());
	OnEnemyDiedObjective.Broadcast(this);

	HealthComponent->SetCurrentHealth(HealthComponent->GetMaxHealth()); // EnemySpawnManager hanterar poolning
}

// Called every frame
void ASGEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASGEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

USGEnemyAttackComponentBase* ASGEnemyCharacter::GetAttackComponent() const
{
	return AttackComponent;
}
