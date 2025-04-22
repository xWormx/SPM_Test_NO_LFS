// Fill out your copyright notice in the Description page of Project Settings.


#include "SGTerminal.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASGTerminal::ASGTerminal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(Root);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Capsule);
}

// Called when the game starts or when spawned
void ASGTerminal::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentHit.AddDynamic(this, &ASGTerminal::OnComponentHit);
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController != nullptr)
		HUDTerminal = CreateWidget<UUserWidget>(PlayerController, HUDTerminalClass);
	
	if (HUDTerminal)
	{
		HUDTerminal->AddToViewport();
		HUDTerminal->SetVisibility(ESlateVisibility::Hidden);
	}

}

// Called every frame
void ASGTerminal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASGTerminal::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("ASGTerminal::OnComponentHit"));
	if (OtherActor != nullptr)
	{
		ASGPlayerCharacter* Player = Cast<ASGPlayerCharacter>(OtherActor);
		if (Player == nullptr)
			return;

		HUDTerminal->SetVisibility(ESlateVisibility::Visible);
		
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController != nullptr)
		{
			PlayerController->SetInputMode(FInputModeUIOnly());
			PlayerController->SetShowMouseCursor(true);
			
		}
			
	}
}


