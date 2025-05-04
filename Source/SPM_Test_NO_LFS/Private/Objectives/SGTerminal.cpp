// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGTerminal.h"
#include "Player/SGPlayerController.h"
#include "Objectives/SGObjectiveToolTipWidget.h"
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
	InteractSphere->OnComponentEndOverlap.AddDynamic(this, &ASGTerminal::OnOverlapEnd);
	ASGPlayerController* PlayerController = Cast<ASGPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		HUDTerminal = Cast<USGTerminalWidget>(CreateWidget<UUserWidget>(PlayerController, HUDTerminalClass));
		PlayerController->OnInteract.AddDynamic(this, &ASGTerminal::OpenTerminal); 
	}
		
	
	if (HUDTerminal)
	{
		HUDTerminal->AddToViewport(10); // Should be higher then ToolTipWidget! highest render on top.
		HUDTerminal->SetVisibility(ESlateVisibility::Hidden);
		
		if (!HUDTerminal->OnStartMission.IsAlreadyBound(this, &ASGTerminal::OnStartMissionButtonClicked))
			HUDTerminal->OnStartMission.AddDynamic(this, &ASGTerminal::OnStartMissionButtonClicked);

		if (!HUDTerminal->OnCloseTerminal.IsAlreadyBound(this, &ASGTerminal::OnCloseTerminalClicked))
			HUDTerminal->OnCloseTerminal.AddDynamic(this, &ASGTerminal::OnCloseTerminalClicked);
	}
}

void ASGTerminal::OnStartMissionButtonClicked()
{
	CloseTerminal();
}

void ASGTerminal::OnCloseTerminalClicked()
{
	CloseTerminal();
}

void ASGTerminal::CloseTerminal()
{
	HUDTerminal->SetVisibility(ESlateVisibility::Hidden);
	LastInteractingPlayerController->EnableInput(LastInteractingPlayerController);
	LastInteractingPlayerController->SetInputMode(FInputModeGameOnly());
	LastInteractingPlayerController->SetShowMouseCursor(false);
	LastInteractingPlayerController->SetWantToInteractWithTerminal(false);
	ASGPlayerController* PlayerController = Cast<ASGPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
		PlayerController->SetPause(false);
}

void ASGTerminal::OpenTerminal()
{
	if (!LastInteractingPlayerController->GetCanInteractWithTerminal())
	{
		UE_LOG(LogTemp, Error, TEXT("Terminal: Player trying to interact with terminal but is not within range!"));
		return;
	}
		
	
	UE_LOG(LogTemp, Display, TEXT("Interacting with terminal"));
	HUDTerminal->SetVisibility(ESlateVisibility::Visible);
	LastInteractingPlayerController->DisableInput(LastInteractingPlayerController);
	LastInteractingPlayerController->SetInputMode(FInputModeUIOnly());
	LastInteractingPlayerController->SetShowMouseCursor(true);
	
	if (GameObjectivesHandler)
	{
		HUDTerminal->SetObjectiveHandler(GameObjectivesHandler);
		USGObjectiveToolTipWidget* ToolTipWidget = GameObjectivesHandler->GetObjectiveToolTipWidget();
		if (ToolTipWidget)
		{
			if (!ToolTipWidget->GetIsHidden())
				ToolTipWidget->InterruptAndHide();	
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Terminal needs a GameObjectivesHandler!"));
	}

	ASGPlayerController* PlayerController = Cast<ASGPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
		PlayerController->SetPause(true);
}

// Called every frame
void ASGTerminal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

void ASGTerminal::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
		PlayerController->SetCanInteractWithTerminal(false);
	}
}

void ASGTerminal::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("ASGTerminal::OnComponentHit"));

}


