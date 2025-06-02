
#include "Enemies/AI/SGAIControllerEnemyFlying_2.h"


ASGAIControllerEnemyFlying_2::ASGAIControllerEnemyFlying_2()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void ASGAIControllerEnemyFlying_2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASGAIControllerEnemyFlying_2::SetFlyingMode(bool bShouldFly)
{
	/*float HoverAmplitude = 50.f;
	 float HoverSpeed = 1.5;
	 float HoverInterpSpeed = 0.5;

	float TargetZ = AttackTarget->GetActorLocation().Z;
	float HoverZ = TargetZ + FMath::Sin(GetWorld()->TimeSeconds * HoverSpeed) * HoverAmplitude;

	CurrentLocation = GetPawn()->GetActorLocation();

	CurrentLocation.Z = FMath::FInterpTo(CurrentLocation.Z, HoverZ, GetWorld()->GetDeltaSeconds(), HoverInterpSpeed);
	ControlledEnemy->SetActorLocation(CurrentLocation, true);*/
}


void ASGAIControllerEnemyFlying_2::BeginPlay()
{
	Super::BeginPlay();
	
}



