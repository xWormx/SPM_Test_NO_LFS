// Fill out your copyright notice in the Description page of Project Settings.


#include "SGTerminal.h"

#include "SGPlayerController.h"
#include "Components/SphereComponent.h"
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

	MeshFloor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshFloor"));
	MeshFloor->SetupAttachment(Mesh);
	
	InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Interact Sphere"));
	InteractSphere->SetupAttachment(Mesh);
	
}

// Called when the game starts or when spawned
void ASGTerminal::BeginPlay()
{
	Super::BeginPlay();
	InteractSphere->OnComponentHit.AddDynamic(this, &ASGTerminal::OnComponentHit);
	InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &ASGTerminal::OnOverlapBegin);
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
	if (LastInteractingPlayerController != nullptr && LastInteractingPlayerController->GetWantToInteractWithTerminal())
	{
		if (HUDTerminal->GetVisibility() == ESlateVisibility::Hidden)
		{
			UE_LOG(LogTemp, Display, TEXT("Interacting with terminal"));
			LastInteractingPlayerController->SetInputMode(FInputModeUIOnly());
			LastInteractingPlayerController->SetShowMouseCursor(true);
			HUDTerminal->SetVisibility(ESlateVisibility::Visible);	
		}
	}
	
}

void ASGTerminal::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("ASGTerminal::OnComponentHit"));
	if (OtherActor != nullptr)
	{
		ASGPlayerCharacter* Player = Cast<ASGPlayerCharacter>(OtherActor);
		if (Player == nullptr)
			return;
		UE_LOG(LogTemp, Warning, TEXT("Player Overlapped!"));
		
		ASGPlayerController* PlayerController = Cast<ASGPlayerController>(Player->GetController());
		if (PlayerController == nullptr)
			return;

		LastInteractingPlayerController = PlayerController;
		PlayerController->SetCanInteractWithTerminal(true);
	}
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
		
		ASGPlayerController* PlayerController = Cast<ASGPlayerController>(Player->GetController());
		if (PlayerController != nullptr)
		{
			if (PlayerController->GetWantToInteractWithTerminal())
			{
				PlayerController->SetInputMode(FInputModeUIOnly());
				PlayerController->SetShowMouseCursor(true);	
			}
		}
			
	}
}


