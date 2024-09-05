// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHGameplayAbility_Talk.h"
#include "OWHCharacter.h"
#include "Dialogues/OWHDialogueInterface.h"
#include "Kismet/KismetSystemLibrary.h"

bool UOWHGameplayAbility_Talk::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        return false;
    }

    if (!ActorInfo)
    {
        return false;
    }

    if (AOWHCharacter* Character = Cast<AOWHCharacter>(ActorInfo->AvatarActor.Get()))
    {
        return GetDialogueInterfaceActor(Character) != nullptr;
    }

    return false;
}

void UOWHGameplayAbility_Talk::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo, nullptr))
    {
        return;
    }

    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    AOWHCharacter* OwnerCharacter = Cast<AOWHCharacter>(ActorInfo->AvatarActor.Get());
    AActor* DialogueActor = OwnerCharacter ? GetDialogueInterfaceActor(OwnerCharacter) : nullptr;

    if (DialogueActor && OwnerCharacter)
    {
        IOWHDialogueInterface::Execute_StartOrContinueDialogue(DialogueActor, OwnerCharacter);
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

void UOWHGameplayAbility_Talk::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

AActor* UOWHGameplayAbility_Talk::GetDialogueInterfaceActor(AOWHCharacter* Character) const
{
    if (!Character)
    {
        return nullptr;
    }

    const FVector TraceStart = Character->GetActorLocation();
    const float TraceDistance = 200.f;

    TArray<AActor*> IgnoredActors;
    IgnoredActors.Add(Character);

    TArray<AActor*> OutActors;
    UKismetSystemLibrary::SphereOverlapActors(
        GetWorld(), TraceStart, TraceDistance,
        {}, AActor::StaticClass(),
        IgnoredActors, OutActors
    );

    for (AActor* Actor : OutActors)
    {
        if (Actor->Implements<UOWHDialogueInterface>())
        {
            return Actor;
        }
    }

    return nullptr;
}
