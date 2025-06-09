
#include "Enemies/Navigation/SGEnemyNavLink.h"

#include "SPM_Test_NO_LFS.h"
#include "Enemies/AI/Old_AI-ControllersWithoutBehaviorTrees/SGAIControllerEnemyBase.h"
#include "Enemies/AI/Old_AI-ControllersWithoutBehaviorTrees/SGAIControllerEnemyBig.h"
#include "Enemies/Characters/SGEnemyCharacter.h"

#include "Navigation/NavLinkProxy.h"

ASGEnemyNavLink::ASGEnemyNavLink()
{
	
	PrimaryActorTick.bCanEverTick = true;
}


void ASGEnemyNavLink::BeginPlay()
{
	Super::BeginPlay();

	OnSmartLinkReached.AddDynamic(this, &ASGEnemyNavLink::OnSmartLinkReachedHandler);
	
}

void ASGEnemyNavLink::OnSmartLinkReachedHandler(AActor* MovingActor, const FVector& DestinationPoint)
{
	if (ASGEnemyCharacter* Character = Cast<ASGEnemyCharacter>(MovingActor))
	{
		Character->JumpToLocation(DestinationPoint);
	}
}



