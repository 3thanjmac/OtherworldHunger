// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OWHGameplayAbility.h"
#include "OWHGameplayAbility_Climb.generated.h"

/**
 * 
 */
UCLASS()
class OTHERWORLDHUNGER_API UOWHGameplayAbility_Climb : public UOWHGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags /* = nullptr */, const FGameplayTagContainer* TargetTags /* = nullptr */, OUT FGameplayTagContainer* OptionalRelevantTags /* = nullptr */) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	void StartClimb(ACharacter* OwnerCharacter);
	void DoClimb(ACharacter* OwnerCharacter);

private:
	UPROPERTY(EditDefaultsOnly, Category = Climb, meta = (AllowPrivateAccess = "true"))
	float AttachmentDistance = 100;

	bool TerminateClimb = false;
};
