// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGTerminal.h"

#include "NiagaraComponent.h"
#include "Player/SGPlayerController.h"
#include "Objectives/SGObjectiveToolTipWidget.h"
#include "Components/SphereComponent.h"
#include "Blueprint/UserWidget.h"
#include "Core/SGGameInstance.h"
#include "PaperSpriteComponent.h"
#include "Core/SGObjectiveHandlerSubSystem.h"

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

	SpriteComponentAlert = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponentAlert->SetupAttachment(Root);

	ParticlesAlert = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParticlesAlert"));
	ParticlesAlert->SetupAttachment(Mesh);
	ParticlesAlert->SetActive(true);
	ParticlesAlert->Activate(true);

	
	
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
		PlayerController->OnInteract.AddDynamic(this, &ASGTerminal::OpenTerminal);	
	}

	ObjectiveHandlerSubSystem = GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>();
	if (ObjectiveHandlerSubSystem)
	{
		ObjectiveHandlerSubSystem->OnObjectiveCompleted.AddDynamic(this, &ASGTerminal::SetAlertActive);
	}
	
	if (SpriteComponentAlert)
	{
		SpriteComponentAlert->SetSprite(SpriteAlertAsset);
		float Scale = 0.15f;
		SpriteComponentAlert->SetWorldScale3D(FVector(Scale, Scale, Scale));

		if (ParticlesAlert)
		{
			ParticlesAlert->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			ParticlesAlert->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
			ParticlesAlert->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
			ParticlesAlert->SetWorldScale3D(FVector::OneVector);
			
		}
		SetAlertActive();
	}

	InitializeHUD();

}

void ASGTerminal::OnStartMissionButtonClicked()
{
	SetAlertInactive();
	CloseTerminal();
}

void ASGTerminal::OnCloseTerminalClicked()
{
	CloseTerminal();
}

void ASGTerminal::CloseTerminal()
{
	//USGObjectiveToolTipWidget* ObjectiveToolTip = GameObjectivesHandler->GetObjectiveToolTipWidget();
	USGObjectiveToolTipWidget* ObjectiveToolTip = ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget();
	if (ObjectiveToolTip)
		ObjectiveToolTip->ResumeAllOngoingAnimations();
	if (HUDTerminal == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ASGTerminal - CloseTerminal(): HUDTerminal was nullptr"))
		return;
	}
	if (LastInteractingPlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("LastInteractingPlayerController was nullptr from: %s"), *GetName());
		return;
	}
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
	
	//GameObjectivesHandler->GetObjectiveToolTipWidget()->PauseAllOngoingAnimations();
	ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->PauseAllOngoingAnimations();
	if (HUDTerminal == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ASGTerminal - OpenTerminal(): HUDTerminal was nullptr"))
		return;
	}
	
	if (LastInteractingPlayerController == nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("LastInteractingPlayerController was nullptr from: %s"), *GetName());
		return;
	}
		
	if (!LastInteractingPlayerController->CanInteractWithTerminal())
	{
		//UE_LOG(LogTemp, Error, TEXT("Terminal: Player trying to interact with terminal but is not within range!"));
		return;
	}
	
	UE_LOG(LogTemp, Display, TEXT("Interacting with terminal"));
	HUDTerminal->SetVisibility(ESlateVisibility::Visible);
	LastInteractingPlayerController->DisableInput(LastInteractingPlayerController);
	LastInteractingPlayerController->SetInputMode(FInputModeUIOnly());
	LastInteractingPlayerController->SetShowMouseCursor(true);
	
	//if (GameObjectivesHandler)
	if (ObjectiveHandlerSubSystem)
	{
		
		//HUDTerminal->SetObjectiveHandler(GameObjectivesHandler);
		OnTerminalOpen.Broadcast();
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("Terminal needs a ObjectiveHandlerSubSystem!"));
	}

	ASGPlayerController* PlayerController = Cast<ASGPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
		PlayerController->SetPause(true);
}

// Called every frame
void ASGTerminal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ASGPlayerController* PlayerController = Cast<ASGPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		FVector PlayerPosition = PlayerController->PlayerCameraManager->GetCameraLocation();
		FVector SpritePosition = SpriteComponentAlert->GetComponentLocation();

		FRotator SpriteRotation = (PlayerPosition - SpritePosition).Rotation();
		SpriteRotation.Yaw += 90.f;
		SpriteRotation.Pitch = 0.0f;
		SpriteRotation.Roll = 0.0f;
		SpriteComponentAlert->SetWorldRotation(SpriteRotation);
	}
	
}
void ASGTerminal::SetAlertActive()
{
	SpriteComponentAlert->SetSpriteColor(FLinearColor(ColorSpriteAlert));
	ParticlesAlert->Activate(true);
}

void ASGTerminal::SetAlertInactive()
{
	SpriteComponentAlert->SetSpriteColor(FLinearColor(1.0, 1.0, 1.0, 0.1f));
	ParticlesAlert->Deactivate();
}


void ASGTerminal::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("ASGTerminal::OnOverlapBegin"));
	if (OtherActor != nullptr)
	{
		ASGPlayerCharacter* Player = Cast<ASGPlayerCharacter>(OtherActor);
		if (Player == nullptr)
			return;
		//UE_LOG(LogTemp, Warning, TEXT("Player Overlapped!"));
		
		ASGPlayerController* PlayerController = Cast<ASGPlayerController>(Player->GetController());
		if (PlayerController == nullptr)
			return;

		LastInteractingPlayerController = PlayerController;
		PlayerController->SetCanInteractWithTerminal(true);
	}
}

void ASGTerminal::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("ASGTerminal::OnOverlapEnd"));
	if (OtherActor != nullptr)
	{
		ASGPlayerCharacter* Player = Cast<ASGPlayerCharacter>(OtherActor);
		if (Player == nullptr)
			return;
		//UE_LOG(LogTemp, Warning, TEXT("Player Overlapped Ended!"));
		
		ASGPlayerController* PlayerController = Cast<ASGPlayerController>(Player->GetController());
		if (PlayerController == nullptr)
			return;

		LastInteractingPlayerController = nullptr;
		PlayerController->SetCanInteractWithTerminal(false);
	}
}

void ASGTerminal::InitializeHUD()
{
	USGGameInstance* GameInstance = Cast<USGGameInstance>(GetWorld()->GetGameInstance());
	if (!GameInstance)
	{
		//UE_LOG(LogTemp, Error, TEXT("ASGTerminal: Couldn't find a GameInstance!"));
		return;
	}

	HUDTerminal = GameInstance->GetTerminalWidget();
	if (HUDTerminal)
	{
		
		if(!HUDTerminal->IsInViewport())
		{
			HUDTerminal->AddToViewport(10); // Should be higher then ToolTipWidget! highest render on top.
			HUDTerminal->SetVisibility(ESlateVisibility::Hidden);
		}
			
		if (!HUDTerminal->OnStartMission.IsAlreadyBound(this, &ASGTerminal::OnStartMissionButtonClicked))
			HUDTerminal->OnStartMission.AddDynamic(this, &ASGTerminal::OnStartMissionButtonClicked);

		if (!HUDTerminal->OnCloseTerminal.IsAlreadyBound(this, &ASGTerminal::OnCloseTerminalClicked))
			HUDTerminal->OnCloseTerminal.AddDynamic(this, &ASGTerminal::OnCloseTerminalClicked);
	}
}

void ASGTerminal::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Warning, TEXT("ASGTerminal::OnComponentHit"));

}


