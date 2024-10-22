// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OWHGameplayAbility.h"
#include "OWHGameplayAbility_Interact.generated.h"

/**
 * 
 */
UCLASS()
class OTHERWORLDHUNGER_API UOWHGameplayAbility_Interact : public UOWHGameplayAbility
{
	GENERATED_BODY()

public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags /* = nullptr */,
	                                const FGameplayTagContainer* TargetTags /* = nullptr */, OUT FGameplayTagContainer* OptionalRelevantTags /* = nullptr */) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float InteractionRadius = 150.f;

private:
	void StartInteract(ACharacter* OwnerCharacter);
	void DoInteract(ACharacter* OwnerCharacter);

	void GetOverlappingActors(ACharacter* OwnerCharacter, TArray<AActor*>& InterfaceActors) const;
};
