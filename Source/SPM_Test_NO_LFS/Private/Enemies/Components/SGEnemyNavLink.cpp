
#include "Enemies/Components/SGEnemyNavLink.h"

#include "SPM_Test_NO_LFS.h"
#include "Enemies/AI/SGAIControllerEnemyBase.h"
#include "Enemies/AI/SGAIControllerEnemyBig.h"
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
		BASIR_DEBUG(TEXT("Should Jump"), FColor::Green, 3.f);
	}
	
}



