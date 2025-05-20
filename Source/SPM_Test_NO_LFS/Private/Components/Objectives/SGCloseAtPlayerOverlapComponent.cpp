// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Objectives/SGCloseAtPlayerOverlapComponent.h"

#include "Core/SGObjectiveHandlerSubSystem.h"
#include "Player/SGPlayerCharacter.h"

// Sets default values for this component's properties
USGCloseAtPlayerOverlapComponent::USGCloseAtPlayerOverlapComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	// ...
}


// Called when the game starts
void USGCloseAtPlayerOverlapComponent::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &USGCloseAtPlayerOverlapComponent::Close);
	// ...
	
}


// Called every frame
void USGCloseAtPlayerOverlapComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USGCloseAtPlayerOverlapComponent::Close(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ASGPlayerCharacter* PlayerCharacter = Cast<ASGPlayerCharacter>(OtherActor);
	if (PlayerCharacter == nullptr)
	{
		CALLE_LOG(Error, TEXT("USGCloseAtPlayerOverlapComponent::Close did NOT recognise Player"));
		return;
	}
	
	UStaticMeshComponent* CubeMesh = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if (CubeMesh)
	{
		CubeMesh->SetVisibility(true);
		CubeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

