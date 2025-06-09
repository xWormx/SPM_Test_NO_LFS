// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BT_Tasks/SGBTTask_KillSelf.h"

#include "SPM_Test_NO_LFS.h"
#include "Enemies/AI/SGAIControllerEnemy.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Managers/SGEnemySpawnManager.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/SGObjectPoolSubsystem.h"

USGBTTask_KillSelf::USGBTTask_KillSelf()
{
	NodeName = TEXT("Kill Self");
}

EBTNodeResult::Type USGBTTask_KillSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ASGAIControllerEnemy* AIController = Cast<ASGAIControllerEnemy>(OwnerComp.GetAIOwner());

	if (!AIController)
	{
		BASIR_LOG(Error, TEXT("Kill Self, AIController is null"));
		return EBTNodeResult::Failed;
	}

	ASGEnemyCharacter* ControlledEnemy = Cast<ASGEnemyCharacter>(AIController->GetPawn());

	if (!ControlledEnemy)
	{
		BASIR_LOG(Error, TEXT("Kill Self, Controlled Enemy is null"));
		return EBTNodeResult::Failed;
	}
	
	USGObjectPoolSubsystem* ObjectPoolingSystem = Cast<USGObjectPoolSubsystem>(GetWorld()->GetGameInstance()->GetSubsystem<USGObjectPoolSubsystem>());
	if (!ObjectPoolingSystem)
	{
		BASIR_LOG(Error, TEXT("Kill Self, Object Pooling System is null"));
		return EBTNodeResult::Failed;
	}
	
	ASGEnemySpawnManager* SpawnManager = Cast<ASGEnemySpawnManager>(AIController->SpawnManagers[0]);

	if (!SpawnManager)
	{
		BASIR_LOG(Error, TEXT("Kill Self, Spawn Manager is null"));
		return EBTNodeResult::Failed;
	}
	
	ControlledEnemy->PlayDeathEffect();
	ObjectPoolingSystem->ReturnObjectToPool(ControlledEnemy);
	SpawnManager->HandleEnemyDeath(ControlledEnemy);
	return EBTNodeResult::Succeeded;
}


