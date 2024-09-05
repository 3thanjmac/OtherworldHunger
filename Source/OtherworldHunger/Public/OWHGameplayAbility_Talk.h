// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OWHGameplayAbility.h"
#include "OWHGameplayAbility_Talk.generated.h"

class AOWHCharacter;
/**
 * 
 */
UCLASS()
class OTHERWORLDHUNGER_API UOWHGameplayAbility_Talk : public UOWHGameplayAbility
{
	GENERATED_BODY()

public:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags /* = nullptr */,
		const FGameplayTagContainer* TargetTags /* = nullptr */, OUT FGameplayTagContainer* OptionalRelevantTags /* = nullptr */) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
		bool bWasCancelled) override;

private:

	AActor* GetDialogueInterfaceActor(AOWHCharacter* Character) const;
};
