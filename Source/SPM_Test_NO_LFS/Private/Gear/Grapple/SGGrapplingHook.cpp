#include "Gear/Grapple/SGGrapplingHook.h"

#include "Gear/Grapple/SGGrappleHeadBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ASGGrapplingHook::ASGGrapplingHook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	CableComponent = CreateDefaultSubobject<UCableComponent>(TEXT("CableComponent"));
	CableComponent->SetupAttachment(GetRootComponent());
	CableComponent->NumSegments = 3;
	
	GrappleHeadPosition = CreateDefaultSubobject<USceneComponent>(TEXT("GrappleHeadPosition"));
	GrappleHeadPosition->SetupAttachment(GetRootComponent());
}

void ASGGrapplingHook::BeginPlay()
{
	Super::BeginPlay();

	Head = GetWorld()->SpawnActor<ASGGrappleHeadBase>(GrappleHeadClass);
	Head->SetActorLocation(GrappleHeadPosition->GetComponentLocation());
	Head->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

	FVector End = UKismetMathLibrary::InverseTransformLocation(GetTransform(), Head->GetActorLocation());
	CableComponent->SetAttachEndToComponent(Head->GetComponentByClass<UStaticMeshComponent>());
	CableComponent->EndLocation = End;
	SetGrappleVisibility(false);
}

void ASGGrapplingHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FHitResult HitResult;
	AController* Controller = GetValidController();
	
	if (const bool bDidHit = GrappleTrace(HitResult, Controller); !bDidHit)
	{
		OnCanGrapple.Broadcast(bDidHit);
	}

	if (!CanGrapple())
	{
		int TimeLeft = GetWorldTimerManager().GetTimerRemaining(GrappleTimerHandle);
		//FString str = FString::Printf(TEXT("Grapple Time: %d"), TimeLeft);
		//GEngine->AddOnScreenDebugMessage(-1, 0.1, FColor::Red, str);
	}
		
	
	if (bDidGrapple)
	{
		// Detta skjuter iväg Grapplehead och uppdateras dess position tills att den är inom 50 enheter
		// av AttachmentPoint
		FVector NewHeadPosition = Head->GetActorLocation();
		
		float DistanceBetweenHeadAndAttachment = FVector::Dist(NewHeadPosition, AttachmentPoint); 
		if (DistanceBetweenHeadAndAttachment < 10)
		{
			bHeadAttached = true;
			bDidGrapple = false;
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), GrappleAttachmentSound, Head->GetActorLocation(), 0.6);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GrappleAttachmentParticles, Head->GetActorLocation());
		}
		else
		{
			// Sålänge avståndet är större än 150 så interpolar vi långsammare mot slutet
			// Annars (när vi är nära målet gör vi det med konstant hastighet)
			if (DistanceBetweenHeadAndAttachment > 350)
			{
				NewHeadPosition = FMath::VInterpTo(Head->GetActorLocation(), AttachmentPoint, DeltaTime, HeadTravelSpeed);
			}
			else
			{
				NewHeadPosition = FMath::VInterpConstantTo(Head->GetActorLocation(), AttachmentPoint, DeltaTime, 1500);
			}
				
			Head->SetActorLocation(NewHeadPosition);
		}
	}

	if (HeadAttached() && PlayerWantToTravel())
	{
		UpdatePlayerPosition(GetValidController()->GetCharacter(), DeltaTime);
	}

}

void ASGGrapplingHook::FireGrapple()
{
	FHitResult HitResult;
	AController* Controller = GetValidController();
	
	bool bDidAttach = AttachGrapple(Controller, HitResult);
	// What happends if the hook attached to a point?
	/*
		1 - Could Disable and go towards point directly
		2 - Could let the hook be attached and then go towards on press.
		3 - Could let it be attached will button held down and go towards on release.
	*/
	if (!bDidAttach)
		return;
	
	// Skulle kunna ha andra beteenden än att färdas direkt mot punkten.
	TravelDirectly(Controller->GetCharacter(), HitResult);

	OnFireGrapple.Broadcast();
	OnBeginCooldown.Broadcast(GrappleTimerHandle);
}

void ASGGrapplingHook::ResetGrapple()
{
	AController* Controller = GetValidController();
	if (Controller == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJPlayerController Not Found!"));
		return;
	}
	
	Controller->GetCharacter()->GetCharacterMovement()->GravityScale = 2.0f;
	// TODO (Calle): Friction borde resettas när man INTE ÄR I LUFTEN LÄNGRE, så att man kan glida på väggar
	// annars fastnar man i dem.
	//Controller->GetCharacter()->GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
	
	Head->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	Head->SetActorLocation(GrappleHeadPosition->GetComponentLocation());
	SetGrappleVisibility(false);
	bHeadAttached = false;
	bStartTravel = false;
}

void ASGGrapplingHook::SetGrappleVisibility(bool bVisibility)
{
	Head->GetComponentByClass<UStaticMeshComponent>()->SetVisibility(bVisibility);
	CableComponent->SetVisibility(bVisibility);
}

void ASGGrapplingHook::SetHeadConstraint(AActor* OtherActor)
{
	Head->SetConstraintTo(OtherActor);
}

void ASGGrapplingHook::EnableGrappling()
{
	bCanGrapple = true;
}

void ASGGrapplingHook::DisableGrappling()
{
	bCanGrapple = false;
}

AController* ASGGrapplingHook::GetValidController() const
{
	APawn* GrappleOwner = Cast<APawn>(GetOwner());
	if (GrappleOwner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner Not Found!"));
		return nullptr;
	}

	return GrappleOwner->GetController();
}

bool ASGGrapplingHook::GrappleTrace(FHitResult& OutHitResult, AController* Controller)
{
	if (Controller == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJPlayerController Not Found!"));
		return false;
	}

	FVector ViewLocation;
	FRotator ViewRotation;
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	FVector TraceEnd = ViewLocation + ViewRotation.Vector() * MaxHookRange;
	GrappleDirection = TraceEnd - ViewLocation;
	GrappleDirection.Normalize();
	Head->SetActorRotation(ViewRotation);
	
	// LineTrace för att kunna hooka när man är nära väggar men också få den mest precisa hookpunkten
	FHitResult LineHitResult;
	bool LineHit = GetWorld()->LineTraceSingleByChannel(LineHitResult,ViewLocation, TraceEnd,
														ECC_GameTraceChannel1);

	// SphereTrace för att det ska vara lättare att träffa med hooken
	bool SphereHitResult = GetWorld()->SweepSingleByChannel(OutHitResult, ViewLocation, TraceEnd,
												FQuat::Identity, ECC_GameTraceChannel1,
													FCollisionShape::MakeSphere(30));

	// Om LineHit träffar vill vi alltid använda den för mer preciserad hook
	if (LineHit)
		OutHitResult = LineHitResult;
	
	return (LineHit || SphereHitResult);
}

void ASGGrapplingHook::StartCharacterLaunch(ACharacter* Character)
{
	if (Character == nullptr)
		return;
	// Skjuter upp karaktären när hooken avlossas för att få ett bra momentum och att karaktären inte dras mot marken
	Character->LaunchCharacter(FVector(0, 0, 300), true, true);
	
	// För att karaktären inte ska bromsas när den dras mot väggar
	Character->GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
	Character->GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	Character->GetCharacterMovement()->GravityScale = 0.6f;
}

void ASGGrapplingHook::UpdatePlayerPosition(ACharacter* Character, float DeltaTime)
{
	UpdatePlayerPositionVelocityBased(Character, DeltaTime);

	// Denna variant kämpar emot Unreals inbyggda Movement/Physics system vilket kan leda till en del "buggar" och specialfall som måste hanteras.
	//UpdatePlayerPositionLocationBased(Character, DeltaTime);
}

void ASGGrapplingHook::UpdatePlayerPositionVelocityBased(ACharacter* Character, float DeltaTime)
{
	if (Character == nullptr || !HeadAttached())
		return;

	float DistanceToGrapplePoint = FVector::Distance(GetAttachmentPoint(), Character->GetActorLocation());

	// Riktning till grapplepoint
	FVector ToGrapplePoint = (AttachmentPoint - Character->GetActorLocation()).GetSafeNormal();

	// Override a velocity
	Character->GetCharacterMovement()->Velocity = ToGrapplePoint * DragSpeed * 1000.0f;

	// När vi når mål, släpp och ge impuls
	if (DistanceToGrapplePoint < GrappleReleaseDistance)
	{
		ResetAndLaunch(Character, DeltaTime);
	}
}

void ASGGrapplingHook::UpdatePlayerPositionLocationBased(ACharacter* Character, float DeltaTime)
{
	if (Character == nullptr || !HeadAttached())
		return;
	
	if (HeadAttached())
	{
		float DistanceToGrapplePoint = FVector::Distance(GetAttachmentPoint(), Character->GetActorLocation());
		
		FVector NewPosition =  FVector::ZeroVector;
		
		if (DistanceToGrapplePoint > 350)
		{
			NewPosition = FMath::VInterpTo(Character->GetActorLocation(),
													GetAttachmentPoint(),
													DeltaTime,
													GetDragSpeed());	
		}
		else
		{
			NewPosition = FMath::VInterpConstantTo(Character->GetActorLocation(), GetAttachmentPoint(), DeltaTime, 1500);
		}
		// Riktning till grapplepoint
		FVector ToGrapplePoint = (AttachmentPoint - Character->GetActorLocation()).GetSafeNormal();

		// Konstant draghastighet
		float GrapplePullSpeed = 3000.0f;

		// Override velocity directly
		Character->GetCharacterMovement()->Velocity = ToGrapplePoint * GrapplePullSpeed;

		if (DistanceToGrapplePoint < GrappleReleaseDistance)
		{
			ResetAndLaunch(Character, DeltaTime);
		}
		else
		{
			// Använder Sweep för att när man interpolarer med ett högt konstant värde kan man fastna i väggar. Men man kan behöb aett högt värde
			// För att motverka bakåtlänges air control movement och därmed förhindra att spelaren inte kommer fram till grapplepointen och blir fast hängandes
			// under grapplepointen.
			FHitResult Hit;
			bool bConstantInterCollision = Character->SetActorLocation(NewPosition, true, &Hit);
			if (bConstantInterCollision)
			{
				if (GrappledActor == Hit.GetActor())
					ResetAndLaunch(Character, DeltaTime);
			}
		}			
	}
}

void ASGGrapplingHook::ResetAndLaunch(ACharacter* Character, float DeltaTime)
{
	ResetGrapple();

	FVector Impuls = FVector::ZeroVector;
	FVector Direction = (AttachmentPoint - PointOfDeparture).GetSafeNormal();
	Impuls = Direction * ImpulsAtArrival;

	// Om hook riktningen är uppåt så lägg till lite extra kraft uppåt!
	if (GetGrappleDirectionNormalized().Z > 0)
		Impuls.Z += ExtraUpwardsImpuls;

	Impuls *= DeltaTime * 10000;
	Character->LaunchCharacter(Impuls, true, true);
}

bool ASGGrapplingHook::AttachGrapple(AController* Controller, FHitResult& HitResult)
{
	if (!CanGrapple())
		return false;
	
	
	bool didHit = GrappleTrace(HitResult, Controller);

	GrappledActor = HitResult.GetActor();
	
	if (!didHit)
	{
		ResetGrapple();
		return false;
	}
	return true;
}

void ASGGrapplingHook::TravelDirectly(ACharacter* Character, FHitResult& HitResult)
{
	UGameplayStatics::PlaySound2D(this, GrappleFireSound);
	DisableGrappling();
	
	GetWorldTimerManager().SetTimer(GrappleTimerHandle, this, &ASGGrapplingHook::EnableGrappling, HookCooldown);
	StartCharacterLaunch(Character);
	
	bDidGrapple = true;
	bStartTravel = true;
	
	AttachmentPoint = HitResult.ImpactPoint;
	PointOfDeparture = GetActorLocation();
	
	SetGrappleVisibility(true);
	
	Head->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

