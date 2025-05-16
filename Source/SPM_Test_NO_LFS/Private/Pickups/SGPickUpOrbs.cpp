#include "Pickups/SGPickUpOrbs.h"
#include "NiagaraComponent.h"
#include "Player/SGPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void ASGPickUpOrbs::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<ASGPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	InterpSpeed = 1.0f;
}

ASGPickUpOrbs::ASGPickUpOrbs()
{
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(FName("PickUpOrb"));
	OrbEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("OrbEffect"));
	OrbEffect->SetupAttachment(RootComponent);

}

void ASGPickUpOrbs::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerCharacter)
	{
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), PlayerCharacter->GetActorRotation(), DeltaTime, InterpSpeed);
		SetActorRotation(NewRotation);
	}
}
