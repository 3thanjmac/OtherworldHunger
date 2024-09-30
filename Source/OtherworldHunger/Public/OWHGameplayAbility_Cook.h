// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OWHGameplayAbility.h"
#include "OWHQuestsManager.h"
#include "OWHGameplayAbility_Cook.generated.h"

/**
 * 
 */
UCLASS()
class OTHERWORLDHUNGER_API UOWHGameplayAbility_Cook : public UOWHGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags /* = nullptr */, const FGameplayTagContainer* TargetTags /* = nullptr */, OUT FGameplayTagContainer* OptionalRelevantTags /* = nullptr */) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UPROPERTY(EditAnywhere, Category = Cooking, meta = (AllowPrivateAccess = "true"))
	float CookingTime = 0.2f;

	UPROPERTY(EditAnywhere, Category = Cooking, meta = (AllowPrivateAccess = "true"))
	FGameplayTag FinishCookingAudioTag;

	class UOWHQuestsManager* RecipeManagerRef;
	class UOWHCharacterInventory* InventoryRef;
	class AOWHCookingPot* CookingPOT = nullptr;
};
