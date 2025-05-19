// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "SGEnemyNavLink.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API ASGEnemyNavLink : public ANavLinkProxy
{
	GENERATED_BODY()

public:
	ASGEnemyNavLink();

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSmartLinkReachedHandler(AActor* MovingActor, const FVector& DestinationPoint);


};
