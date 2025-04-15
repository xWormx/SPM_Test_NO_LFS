// Fill out your copyright notice in the Description page of Project Settings.


#include "SGBTService_CanHitPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


USGBTService_CanHitPlayer::USGBTService_CanHitPlayer()
{
	NodeName = TEXT("Set Can Hit Player");
}

void USGBTService_CanHitPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	APawn* EnemyPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());

	if (!PlayerPawn || !EnemyPawn)
	{
		return;
	}

	float Distance = FVector::Dist(EnemyPawn->GetActorLocation(), PlayerPawn->GetActorLocation());
	

	bool bCanHitPlayer = Distance <= HitRange;

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bCanHitPlayer);
	
}
