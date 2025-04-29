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

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	PhysicsConstraint->SetupAttachment(GrappleHeadMesh);

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

void ASGGrappleHeadBase::SetConstraintTo(AActor* OtherActor)
{
	if (OtherActor != nullptr)
	{
		PhysicsConstraint->ConstraintActor1 = this;
		PhysicsConstraint->ConstraintActor2 = OtherActor;
		PhysicsConstraint->SetConstrainedComponents(GrappleHeadMesh, NAME_None, OtherActor->GetComponentByClass<UCapsuleComponent>(), NAME_None);		
	}
}





