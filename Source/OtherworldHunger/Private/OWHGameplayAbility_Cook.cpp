// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHGameplayAbility_Cook.h"
#include "OWHCharacter.h"
#include "OWHQuestsManager.h"
#include "Components/OWHCharacterInventory.h"
#include "OWHCookingPot.h"
#include "OWHNotificationUIScreen.h"
#include "Components/CapsuleComponent.h"

void UOWHGameplayAbility_Cook::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AOWHCharacter* OwnerCharacter = Cast<AOWHCharacter>(ActorInfo->AvatarActor))
	{
		if (RecipeManagerRef == nullptr)
		{
			RecipeManagerRef = OwnerCharacter->GetQuestsManager();
		}

		if (InventoryRef == nullptr)
		{
			InventoryRef = OwnerCharacter->GetCharacterInventory();
		}
	}
}

bool UOWHGameplayAbility_Cook::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags /* = nullptr */, const FGameplayTagContainer* TargetTags /* = nullptr */, OUT FGameplayTagContainer* OptionalRelevantTags /* = nullptr */) const
{
	AOWHCharacter* OwnerCharacter = Cast<AOWHCharacter>(ActorInfo->AvatarActor);

	if (OwnerCharacter == nullptr) { return false; }

	TArray<AActor*> OverlappingActors;
	OwnerCharacter->GetOverlappingActors(OverlappingActors, AOWHCookingPot::StaticClass());

	if (OverlappingActors.Num() == 0)
	{
		OwnerCharacter->ShowNotification("Please Go To the Cooking POT to be able to cook", ENotificationType::EWarning);
		return false;
	}

	AActor* Pot = OverlappingActors[0];
	FVector Dir = Pot->GetActorLocation() - (OwnerCharacter->GetActorLocation() - (OwnerCharacter->GetActorUpVector() * OwnerCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	Dir.Normalize();

	float DotProduct = FVector::DotProduct(OwnerCharacter->GetActorForwardVector(), Dir);

	if (DotProduct <= 0)
	{
		OwnerCharacter->ShowNotification("Look At the cooking Cooking POT to be able to cook", ENotificationType::EWarning);
		return false;
	}

	if (RecipeManagerRef && InventoryRef)
	{
		if (FRecipeDataTable* CurrentRecipe = RecipeManagerRef->GetCurrentRecipe())
		{
			if (InventoryRef->HasIngredients(CurrentRecipe->Ingredients) == false)
			{
				OwnerCharacter->ShowNotification("Missing Ingredients", ENotificationType::EError);
				return false;
			}
		}
	}

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UOWHGameplayAbility_Cook::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (RecipeManagerRef && InventoryRef)
	{
		if (FRecipeDataTable* CurrentRecipe = RecipeManagerRef->GetCurrentRecipe())
		{
			InventoryRef->RemoveIngredients(CurrentRecipe->Ingredients);

			if (AOWHCharacter* OwnerCharacter = Cast<AOWHCharacter>(ActorInfo->AvatarActor))
			{
				TArray<AActor*> OverlappingActors;
				OwnerCharacter->GetOverlappingActors(OverlappingActors, AOWHCookingPot::StaticClass());
				if (OverlappingActors.Num() > 0)
				{
					CookingPOT = Cast<AOWHCookingPot>(OverlappingActors[0]);
					if (CookingPOT)
					{
						OwnerCharacter->ShowNotification("Cooking Started", ENotificationType::EMessage);
						CookingPOT->Cook(true);
					}
				}
			}
		}
	}
}

void UOWHGameplayAbility_Cook::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (CookingPOT)
	{
		CookingPOT->Cook(false);
	}

	if (FinishCookingAudioTag.IsValid())
	{
		if (AOWHCharacter* OwnerCharacter = Cast<AOWHCharacter>(ActorInfo->AvatarActor))
		{
			OwnerCharacter->ShowNotification("Cooking Done", ENotificationType::EMessage);
			OwnerCharacter->PlaySFX(FinishCookingAudioTag);
		}
	}

	RecipeManagerRef->LoadRecipe();
}