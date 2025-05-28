#include "Enemies/AI/SGAIControllerEnemy.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


ASGAIControllerEnemy::ASGAIControllerEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASGAIControllerEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
		AttackTarget = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		AttackTargetLocation = AttackTarget->GetActorLocation();
		AttackTargetLocationInAir = FVector(AttackTarget->GetActorLocation().X, AttackTarget->GetActorLocation().Y, GetPawn()->GetActorLocation().Z);
		GetBlackboardComponent()->SetValueAsObject(TEXT("AttackTarget"), AttackTarget);
		GetBlackboardComponent()->SetValueAsVector(TEXT("AttackTargetLocation"), AttackTargetLocation);
		GetBlackboardComponent()->SetValueAsVector(TEXT("AttackTargetLocationInAir"), AttackTargetLocationInAir);
	}
}

void ASGAIControllerEnemy::SetInitialValues()
{
}

void ASGAIControllerEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BehaviorTree)
	{
		AttackTargetLocation = AttackTarget->GetActorLocation();
		AttackTargetLocationInAir = FVector(AttackTarget->GetActorLocation().X, AttackTarget->GetActorLocation().Y, GetPawn()->GetActorLocation().Z);
		GetBlackboardComponent()->SetValueAsObject(TEXT("AttackTarget"), AttackTarget);
		GetBlackboardComponent()->SetValueAsVector(TEXT("AttackTargetLocation"), AttackTargetLocation);
		GetBlackboardComponent()->SetValueAsVector(TEXT("AttackTargetLocationInAir"), AttackTargetLocationInAir);
	}
}

