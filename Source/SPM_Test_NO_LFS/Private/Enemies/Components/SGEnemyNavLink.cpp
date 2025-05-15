
#include "Enemies/Components/SGEnemyNavLink.h"

#include "Enemies/AI/SGAIControllerEnemyBase.h"
#include "GameFramework/Character.h"
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
	ACharacter* Character = Cast<ACharacter>(MovingActor);

	if (Character)
	{
		ASGAIControllerEnemyBase* AIController = Cast<ASGAIControllerEnemyBase>(Character->GetController());

		if (AIController)
		{
			//AIController->JumpTowardsTarget(DestinationPoint);
		}
	}
	
}



