/*

Written by
----------------------
- Carl-Johan Larson Eliasson
- cael7567
	
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SGObjectiveSequenceComponent.generated.h"


enum class EObjectiveType : uint8;
class ULevelSequence;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_TEST_NO_LFS_API USGObjectiveSequenceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USGObjectiveSequenceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	
	UPROPERTY(EditAnywhere, Category=UPROPERTY)
	EObjectiveType ObjectiveTypeToWatch;

	UPROPERTY(EditAnywhere, Category=UPROPERTY)
	USoundBase* SoundDuringSequence;

	UPROPERTY(EditAnywhere, Category=UPROPERTY)
	ULevelSequence* SequenceToPlay;
	
	UFUNCTION()
	void PlaySequence(EObjectiveType ObjectiveType);
		
};
