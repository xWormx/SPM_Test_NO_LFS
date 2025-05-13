// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Components/SGEnemyMoveToPoint.h"

#include "Components/BillboardComponent.h"


ASGEnemyMoveToPoint::ASGEnemyMoveToPoint()
{
	
	PrimaryActorTick.bCanEverTick = false;
	Tags.Add(TEXT("EnemyMoveToPoint"));

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(Root);

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(Root);

	static ConstructorHelpers::FObjectFinder<UTexture2D> Icon(TEXT("/Game/Textures/T_EnemyMoveToPointEditorOnly"));
	if (Icon.Succeeded())
	{
		Billboard->SetSprite(Icon.Object);
	}

	bIsEditorOnlyActor = false;
	
}


void ASGEnemyMoveToPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASGEnemyMoveToPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

