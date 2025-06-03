#pragma once
#include "CoreMinimal.h"
#include "SPM_Test_NO_LFS.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "SGActorTemplate.generated.h"

USTRUCT()
struct FActorTemplate
{
    GENERATED_BODY()

    template<typename T>
    void MakeTemplateOfActor(T* Object);
    void MakeTemplateOfActorRuntime(AActor* Actor);

    template<typename T>
    void ApplyToActor(T* Actor, bool bPrepareForWorld = false);
    void ApplyToActorRuntime(AActor* Actor, bool bPrepareForWorld = false);

    float GravityScale = 0.f;
    bool bTickInitiallyEnabled = true;
    bool bCollisionInitiallyEnabled = true;
};

//------------ACHARACTER
template <>
inline void FActorTemplate::MakeTemplateOfActor<ACharacter>(ACharacter* Character)
{
    bTickInitiallyEnabled = Character->IsActorTickEnabled();
    bCollisionInitiallyEnabled = Character->GetRootComponent()->IsCollisionEnabled();

    if (const UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement())
    {
        GravityScale = CharacterMovement->GravityScale;
    }
}

template <>
inline void FActorTemplate::ApplyToActor<ACharacter>(ACharacter* Character, bool bPrepareForWorld)
{
    Character->SetActorHiddenInGame(!bPrepareForWorld);
    Character->SetActorEnableCollision(bPrepareForWorld ? bCollisionInitiallyEnabled : false);
    Character->SetActorTickEnabled(bPrepareForWorld ? bTickInitiallyEnabled : false);

    Character->GetCharacterMovement()->GravityScale = bPrepareForWorld ? GravityScale : 0.f;
    Character->GetController()->SetActorTickEnabled(bPrepareForWorld);
    if (!bPrepareForWorld)
    {
        Character->GetCharacterMovement()->StopMovementImmediately();
    }
}

//------------AACTOR
template <>
inline void FActorTemplate::MakeTemplateOfActor<AActor>(AActor* Actor)
{
    bTickInitiallyEnabled = Actor->IsActorTickEnabled();
    bCollisionInitiallyEnabled = Actor->GetRootComponent()->IsCollisionEnabled();

}

template <>
inline void FActorTemplate::ApplyToActor<AActor>(AActor* Actor, bool bPrepareForWorld)
{
    Actor->SetActorHiddenInGame(!bPrepareForWorld);
    Actor->SetActorEnableCollision(bPrepareForWorld ? bCollisionInitiallyEnabled : false);
    Actor->SetActorTickEnabled(bPrepareForWorld ? bTickInitiallyEnabled : false);

}

//------------RUNTIME
inline void FActorTemplate::MakeTemplateOfActorRuntime(AActor* Actor)
{
    if (ACharacter* Character = Cast<ACharacter>(Actor))
    {
        MakeTemplateOfActor<ACharacter>(Character);
    }
    else
    {
        MakeTemplateOfActor<AActor>(Actor);
    }
}

inline void FActorTemplate::ApplyToActorRuntime(AActor* Actor, bool bPrepareForWorld)
{
    if (ACharacter* Character = Cast<ACharacter>(Actor))
    {
        ApplyToActor<ACharacter>(Character, bPrepareForWorld);
    }
    else
    {
        ApplyToActor<AActor>(Actor, bPrepareForWorld);
    }
}
