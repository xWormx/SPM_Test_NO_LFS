// Fill out your copyright notice in the Description page of Project Settings.


#include "Gear/Grapple/SGGrappleHeadBase.h"

#include "Components/CapsuleComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
// Sets default values
ASGGrappleHeadBase::ASGGrappleHeadBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	GrappleHeadMesh	= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrappleHeadMesh"));
	GrappleHeadMesh->SetupAttachment(Root);


}

// Called when the game starts or when spawned
void ASGGrappleHeadBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASGGrappleHeadBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}






