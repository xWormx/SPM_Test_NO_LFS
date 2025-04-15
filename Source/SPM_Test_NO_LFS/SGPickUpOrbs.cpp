#include "SGPickUpOrbs.h"
#include "NiagaraComponent.h"
#include "SGPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void ASGPickUpOrbs::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<ASGPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

ASGPickUpOrbs::ASGPickUpOrbs()
{
	PrimaryActorTick.bCanEverTick = true;
	OrbEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("OrbEffect"));
	OrbEffect->SetupAttachment(RootComponent);
	
	InterpSpeed = 1.0f;
}


void ASGPickUpOrbs::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerCharacter)
	{
		FVector NewLocation = FMath::VInterpTo(GetActorLocation(), PlayerCharacter->GetActorLocation(), DeltaTime, InterpSpeed);
		SetActorLocation(NewLocation);
	}}