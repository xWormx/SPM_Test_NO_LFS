// Joel Larsson Wendt || jola6902

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGVoiceLines.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API ASGVoiceLines : public AActor
{
	GENERATED_BODY()
	
public:	
	ASGVoiceLines();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
