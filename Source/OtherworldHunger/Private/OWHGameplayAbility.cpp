// Fill out your copyright notice in the Description page of Project Settings.

#include "OWHGameplayAbility.h"
#include "OWHAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "OWHCharacter.h"

void UOWHGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityInputAction == nullptr) { return; }

	if (ActorInfo->AvatarActor.Get())
	{
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get()))
		{
			const APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());

			if (PlayerController == nullptr)
			{
				return;
			}

			// Binding enhanced inputs
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				EnhancedInputComponent->BindAction(AbilityInputAction, AbilityInputTriggerEvent, this, &UOWHGameplayAbility::OnInputStatusChanged);
				if (ToggableAbility == false)
				{
					EnhancedInputComponent->BindAction(AbilityInputAction, ETriggerEvent::Completed, this, &UOWHGameplayAbility::OnInputStatusChanged);
				}
			}
		}
	}
}

void UOWHGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AOWHCharacter* OwnerCharacter = Cast<AOWHCharacter>(ActorInfo->AvatarActor))
	{
		OwnerCharacter->PlaySFX(AudioTag);
	}
}

void UOWHGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AOWHCharacter* OwnerCharacter = Cast<AOWHCharacter>(ActorInfo->AvatarActor))
	{
		OwnerCharacter->StopSFX(AudioTag);
	}
}

void UOWHGameplayAbility::OnInputStatusChanged(const FInputActionValue& Value)
{
	if (UOWHAbilitySystemComponent* OwningAbilityComponent = Cast<UOWHAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
	{
		if (AbilityTags.GetByIndex(0).IsValid())
		{
			if (Value.Get<bool>())
			{
				if (ToggableAbility && OwningAbilityComponent->IsAbilityActiveByClass(GetClass()))
				{
					OwningAbilityComponent->CancelAbilityByTag(AbilityTags.GetByIndex(0));
					return;
				}

				OwningAbilityComponent->ActivateAbilityByTag(AbilityTags.GetByIndex(0));
			}
			else
			{
				//OwningAbilityComponent->CancelAbilityByTag(AbilityTags.GetByIndex(0));
			}
		}
	}
}
