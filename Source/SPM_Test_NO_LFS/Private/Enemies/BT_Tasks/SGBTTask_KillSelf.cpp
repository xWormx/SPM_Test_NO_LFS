// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BT_Tasks/SGBTTask_KillSelf.h"

#include "SPM_Test_NO_LFS.h"
#include "Enemies/AI/SGAIControllerEnemy.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

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
	
	UGameplayStatics::ApplyDamage(ControlledEnemy, 2000.f, nullptr, ControlledEnemy, UDamageType::StaticClass());
	return EBTNodeResult::Succeeded;
}


