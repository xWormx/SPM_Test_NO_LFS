// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BTServices/SGBTServiceSetShouldPatrol.h"

#include "BehaviorTree/BlackboardComponent.h"

USGBTServiceSetShouldPatrol::USGBTServiceSetShouldPatrol()
{
	NodeName = TEXT("Set Should Patrol");
}

void USGBTServiceSetShouldPatrol::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (!GetWorld()->GetTimerManager().IsTimerActive(PatrolDelayTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(
				PatrolDelayTimerHandle,
				this,
				&USGBTServiceSetShouldPatrol::PatrolDelay,
				3.f,
				false
			);
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bShouldPatrol);
}

void USGBTServiceSetShouldPatrol::PatrolDelay()
{
	bShouldPatrol = true;
}
