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

private:
	UFUNCTION(BlueprintCallable)
	void PlaySound(USoundBase* Sound) const;
	
	UPROPERTY(VisibleAnywhere,Category="sound")
	UAudioComponent* AudioComponent;

	UPROPERTY(VisibleAnywhere,Category="sound")
	TMap<USoundBase*, float> CooldownMap;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="sound",meta=(AllowPrivateAccess = "true"))
	TArray<USoundBase*> Sounds;
};
