// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Player/SGPlayerCharacter.h"
#include "SGTerminalWidget.h"
#include "SGTerminal.generated.h"
class UNiagaraComponent;
class UPaperSpriteComponent;
class UPaperSprite;
DECLARE_MULTICAST_DELEGATE(FOnTerminalOpen);
UCLASS()
class SPM_TEST_NO_LFS_API ASGTerminal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASGTerminal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void SetAlertActive();
	UFUNCTION()
	void SetAlertInactive();
	
	FOnTerminalOpen OnTerminalOpen;
	
private:

	bool bHUDInitialized = false;
	
	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* Capsule;

	UPROPERTY(EditAnywhere)
	class USphereComponent* InteractSphere;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshFloor;


	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	class ASGGameObjectivesHandler* GameObjectivesHandler;
/*
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	class USGObjectiveToolTipWidget* ObjectiveToolTipWidget;
*/
	// Sprite Above Terminal
	UPROPERTY(EditAnywhere)
	UPaperSpriteComponent* SpriteComponentAlert;
	UPROPERTY(EditAnywhere)
	UPaperSprite* SpriteAlertAsset;
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* ParticlesAlert;
	UPROPERTY(EditAnywhere)
	FLinearColor ColorSpriteAlert;
	
	// Terminal Widget
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	TSubclassOf<USGTerminalWidget> HUDTerminalClass;
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	USGTerminalWidget* HUDTerminal;

	
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	class ASGPlayerController* LastInteractingPlayerController;

	
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void InitializeHUD();
	
	UFUNCTION()
	void OnStartMissionButtonClicked();
	UFUNCTION()
	void OnCloseTerminalClicked();
	UFUNCTION()
	void OpenTerminal();
	
	void CloseTerminal();
	
};

