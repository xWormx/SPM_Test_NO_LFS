
#include "Enemies/Navigation/SGEnemyPatrolPoint.h"

#include "Components/BillboardComponent.h"


ASGEnemyPatrolPoint::ASGEnemyPatrolPoint()
{
	PrimaryActorTick.bCanEverTick = false;
	Tags.Add(TEXT("PatrolPoint"));

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(Root);

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(Root);

	static ConstructorHelpers::FObjectFinder<UTexture2D> Icon(TEXT("/Game/Textures/T_EnemyPatrolPointEditorOnly"));

	if (Icon.Succeeded())
	{
		Billboard->SetSprite(Icon.Object);
	}
	
	bIsEditorOnlyActor = false;
}

void ASGEnemyPatrolPoint::BeginPlay()
{
	Super::BeginPlay();
	
}



