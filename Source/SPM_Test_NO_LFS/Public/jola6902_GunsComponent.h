// Joel Larsson Wendt | jola6902

#pragma once

class ASGGun;
class USGWeaponsHUD;

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "jola6902_GunsComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_TEST_NO_LFS_API Ujola6902_GunsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	Ujola6902_GunsComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	void FireGun();
	void ReloadGun();
	const ASGGun* GetGunRef() const;
	void SetCurrentGunIndex(int8 NewIndex);
	int8 GetCurrentGunIndex();
	const TArray<ASGGun*>& GetGuns() const;

private:
	void CreateGunsHUD();
	void UpdateGunsHUD();
	void SetUpGuns();

	AActor* Owner;
	USGWeaponsHUD* GunsHUD;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<ASGGun>> GunClasses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	TArray<ASGGun*> Guns;
	int8 CurrentGunIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UUserWidget> GunsHUDWidgetClass;
};
