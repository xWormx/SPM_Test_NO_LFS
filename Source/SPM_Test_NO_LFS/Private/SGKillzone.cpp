// Fill out your copyright notice in the Description page of Project Settings.


#include "SGKillzone.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASGKillzone::ASGKillzone()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	RootComponent = BoxComponent;

	BoxComponent->SetCollisionProfileName(TEXT("Trigger"));
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASGKillzone::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ASGKillzone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASGKillzone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASGKillzone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		UGameplayStatics::ApplyDamage(OtherActor, 2000.0f, nullptr, this, UDamageType::StaticClass());
	}
}

