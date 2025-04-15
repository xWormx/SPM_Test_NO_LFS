// Fill out your copyright notice in the Description page of Project Settings.


#include "SGBTService_PlayerLocationIfSeen.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

USGBTService_PlayerLocationIfSeen::USGBTService_PlayerLocationIfSeen()
{
	NodeName = TEXT("Set Player Location If Seen");
}

void USGBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation());
	}
}
