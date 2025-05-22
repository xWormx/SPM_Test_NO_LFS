// Fill out your copyright notice in the Description page of Project Settings.

// comment
#include "Components/Objectives/SGOpenAtObjectiveCompleteComponent.h"

#include "SPM_Test_NO_LFS.h"
#include "Core/SGGameInstance.h"
#include "Core/SGObjectiveHandlerSubSystem.h"
#include "Objectives/SGGameObjectivesHandler.h"
#include "Objectives/SGObjectiveBase.h"


// Sets default values for this component's properties
USGOpenAtObjectiveCompleteComponent::USGOpenAtObjectiveCompleteComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USGOpenAtObjectiveCompleteComponent::BeginPlay()
{
	Super::BeginPlay();
	
	USGObjectiveHandlerSubSystem* ObjectiveHandlerSubSystem = GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>();
	if (ObjectiveHandlerSubSystem)
	{
		ObjectiveHandlerSubSystem->OnObjectiveCompletedWithType.AddDynamic(this, &USGOpenAtObjectiveCompleteComponent::Open);
	}
	
}


// Called every frame
void USGOpenAtObjectiveCompleteComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USGOpenAtObjectiveCompleteComponent::Open(EObjectiveType ObjectiveType)
{
	
	if (ObjectiveType == ObjectiveTypeToWatch)
	{
		UStaticMeshComponent* CubeMesh = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		if (CubeMesh)
		{
			CubeMesh->SetVisibility(false);
			CubeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}
