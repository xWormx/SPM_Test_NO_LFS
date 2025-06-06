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
	void PlaySound(USoundBase* Sound, float Cooldown);
	
	UPROPERTY(VisibleAnywhere,Category="editor")
	UAudioComponent* AudioComponent;

	UPROPERTY(VisibleAnywhere,Category="editor")
	TMap<USoundBase*, float> CooldownMap;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="editor",meta=(AllowPrivateAccess = "true"))
	TArray<USoundBase*> Sounds;

	UPROPERTY(EditAnywhere)
	USoundBase* JumpSound;

	void CheckIfJumping();
	void BindDelegateHandlers() const;

	class ASGPlayerCharacter* PlayerRef = nullptr;
	class USGObjectiveHandlerSubSystem* ObjectiveHandlerRef = nullptr;

public:
	UFUNCTION()
	void Voice_Fluff(class ASGEnemyCharacter* Enemy);

	UFUNCTION()
	void Voice_Reload(int32 GunIndex, class ASGGun* Gun);

	UFUNCTION()
	void Voice_ObjectiveStarted(EObjectiveType ObjectiveType);
	
	UFUNCTION()
	void Voice_ObjectiveCompleted(EObjectiveType ObjectiveType);

	UFUNCTION()
	void Voice_Hurt(float NewHealth);

	UFUNCTION()
	void Voice_Death(float NewHealth);
};
