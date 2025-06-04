
#include "Enemies/AI/SGAIControllerEnemyFlying_2.h"
#include "Enemies/Characters/SGEnemyCharacter.h"


ASGAIControllerEnemyFlying_2::ASGAIControllerEnemyFlying_2()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void ASGAIControllerEnemyFlying_2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetFlyingMode(true);
}

void ASGAIControllerEnemyFlying_2::SetFlyingMode(bool bShouldFly)
{
	if (!ControlledEnemy)
	{
		return;
	}
	
	ControlledEnemyLocation = ControlledEnemy->GetActorLocation();

	const float ZValue = ControlledEnemyLocation.Z;
	
	const float HoverZ = ZValue + FMath::Sin(GetWorld()->TimeSeconds * HoverSpeed) * HoverAmplitude;
	
	
	ControlledEnemyLocation.Z = FMath::FInterpTo(ZValue, HoverZ, GetWorld()->GetDeltaSeconds(), HoverInterpSpeed);
	
	ControlledEnemy->SetActorLocation(ControlledEnemyLocation, true);
}


void ASGAIControllerEnemyFlying_2::BeginPlay()
{
	Super::BeginPlay();

	bShouldBeFlying = true;
	
}



