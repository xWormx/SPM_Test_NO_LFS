
#include "Enemies/Components/SGEnemyAttackComponentBase.h"

#include "Core/SGUpgradeSubsystem.h"
#include "Enemies/Characters/SGEnemyCharacter.h"


USGEnemyAttackComponentBase::USGEnemyAttackComponentBase()
{
	PrimaryComponentTick.bCanEverTick = false;
	bCanAttack = true;
	OwnerCharacter = Cast<ASGEnemyCharacter>(GetOwner());
}

void USGEnemyAttackComponentBase::BeginPlay()
{
	Super::BeginPlay();

	if (USGUpgradeSubsystem* UpgradeSubsystem = GetOwner()->GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>())
	{
		UpgradeSubsystem->BindAttribute(this, TEXT("DamageAmount"), TEXT("EnemyDamage"), TEXT("Hidden"));
	}	
}


void USGEnemyAttackComponentBase::StartAttack(AActor* Target)
{
	if (!Target || !bCanAttack || !OwnerCharacter)
	{
		return;
	}

	bCanAttack = false;

	GetWorld()->GetTimerManager().SetTimer(
		CooldownTimerHandle,
		this,
		&USGEnemyAttackComponentBase::ResetAttackCooldown,
		AttackCooldown,
		false
	);
}

void USGEnemyAttackComponentBase::PerformAttack(AActor* Target){}

void USGEnemyAttackComponentBase::ResetAttackCooldown()
{
	bCanAttack = true;
}

