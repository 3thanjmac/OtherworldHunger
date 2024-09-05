// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHGameplayAbility_Interact.h"
#include "GameFramework/Character.h"
#include "OWHCharacter.h"
#include "OWHInteractableInterface.h"
#include "Components/OWHCharacterInventory.h"
#include "Components/SphereComponent.h"
#include "OWHAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/CapsuleComponent.h"
//#include "Dialogues/OWHDialogueInterface.h"


bool UOWHGameplayAbility_Interact::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor);

	TArray<AActor*> OverlappingActors;
	OwnerCharacter->GetOverlappingActors(OverlappingActors, UOWHInteractableInterface::StaticClass());

	if (OverlappingActors.Num() == 0) { return false; }

	bool bFrontItemFound = false;
	for (AActor* Interactable : OverlappingActors)
	{
		const FVector CharacterToActorDirection = (Interactable->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal2D();
		const float DotProduct = FVector::DotProduct(OwnerCharacter->GetActorForwardVector(), CharacterToActorDirection);

		// Only if Player is facing the interactable and implements either if Interactable or Dialogue Interaface.
		if (DotProduct > 0.0f)
		{
			bFrontItemFound = true;
			break;
		}
	}

	if (!bFrontItemFound)
	{
		return false;
	}

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UOWHGameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor);
	if (OwnerCharacter == nullptr) { return; }

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	DoInteract(OwnerCharacter);
}

void UOWHGameplayAbility_Interact::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UOWHGameplayAbility_Interact::DoInteract(ACharacter* OwnerCharacter)
{
	AOWHCharacter* OWHCharacter = Cast<AOWHCharacter>(OwnerCharacter);
	if (!OWHCharacter)
	{
		K2_CancelAbility();
		return;
	}

	TArray<AActor*> OverlappingActors;
	OWHCharacter->GetOverlappingActors(OverlappingActors, UOWHInteractableInterface::StaticClass());

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor)
		{
			const FVector CharacterToActorDirection = (Actor->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal2D();
			const float DotProduct = FVector::DotProduct(OwnerCharacter->GetActorForwardVector(), CharacterToActorDirection);

			if (DotProduct > 0.0f)
			{
				if (IOWHInteractableInterface* Interactable = Cast<IOWHInteractableInterface>(Actor))
				{
					Interactable->Interact_Implementation(OWHCharacter); 
				}
				else if (Actor->ActorHasTag("SkystoneLevel"))
				{
					OWHCharacter->ShowConfirmation("Go to the Dunes of Skystone?", "Skystones");
				}
				else if (Actor->ActorHasTag("SwampLevel"))
				{
					OWHCharacter->ShowConfirmation("Go to the Forgotten Swamp?", "ForgottenSwamp");
				}
			}
		}
	}

	K2_CancelAbility();
}
