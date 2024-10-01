// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHGameplayAbility_Climb.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "OWHCharacter.h"
#include "AbilitySystemGlobals.h"
#include "OWHAbilitySystemComponent.h"
#define Climbable ECC_GameTraceChannel1

bool UOWHGameplayAbility_Climb::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags /* = nullptr */, const FGameplayTagContainer* TargetTags /* = nullptr */, OUT FGameplayTagContainer* OptionalRelevantTags /* = nullptr */) const
{
	if (AOWHCharacter* OwnerCharacter = Cast<AOWHCharacter>(ActorInfo->AvatarActor))
	{
		if (OwnerCharacter->GetCharacterMovement() == nullptr)
		{
			return false;
		}

		FVector StartTrace = OwnerCharacter->GetActorLocation();
		FVector EndTrace = StartTrace + OwnerCharacter->GetActorForwardVector() * AttachmentDistance;
		FHitResult HitResult;

		OwnerCharacter->GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility);

		if (HitResult.bBlockingHit == false)
		{
			return false;
		}
		else if (HitResult.Normal.Z > 0.7)
		{
			return false;
		}
		else
		{
			FHitResult ClimbTest;
			OwnerCharacter->GetWorld()->LineTraceSingleByChannel(ClimbTest, StartTrace, EndTrace, Climbable);

			if (ClimbTest.bBlockingHit == false)
			{
				OwnerCharacter->ShowNotification("Can't Climb Here", ENotificationType::EError);
				return false;
			}
		}
	}

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UOWHGameplayAbility_Climb::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor);
	if (OwnerCharacter == nullptr || OwnerCharacter->GetCharacterMovement() == nullptr) { return; }

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	StartClimb(OwnerCharacter);
}

void UOWHGameplayAbility_Climb::StartClimb(ACharacter* OwnerCharacter)
{
	if (UCharacterMovementComponent* CharacterMovementComponent = OwnerCharacter->GetCharacterMovement())
	{
		FVector StartTrace = OwnerCharacter->GetActorLocation();
		FVector EndTrace = StartTrace + OwnerCharacter->GetActorForwardVector() * AttachmentDistance;
		FHitResult HitResult;

		OwnerCharacter->GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility);

		if (HitResult.bBlockingHit)
		{
			CharacterMovementComponent->SetMovementMode(MOVE_Flying);
			CharacterMovementComponent->bOrientRotationToMovement = false;
			OwnerCharacter->SetActorRotation(FRotator(UKismetMathLibrary::MakeRotFromX(-HitResult.Normal)));

			UCapsuleComponent* PlayerCapsule = OwnerCharacter->GetCapsuleComponent();

			FVector AttachLocation = HitResult.Location + PlayerCapsule->GetUnscaledCapsuleRadius() * HitResult.Normal;
			UKismetSystemLibrary::MoveComponentTo(PlayerCapsule, AttachLocation, UKismetMathLibrary::MakeRotFromX(-HitResult.Normal), false, false, 0.2f, false, EMoveComponentAction::Type::Move, FLatentActionInfo());

			DoClimb(OwnerCharacter);
		}
	}
}

void UOWHGameplayAbility_Climb::DoClimb(ACharacter* OwnerCharacter)
{
	if (UCharacterMovementComponent* CharacterMovementComponent = OwnerCharacter->GetCharacterMovement())
	{
		FVector StartTrace = OwnerCharacter->GetActorLocation() - OwnerCharacter->GetActorUpVector() * OwnerCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
		FVector EndTrace = StartTrace + OwnerCharacter->GetActorForwardVector() * AttachmentDistance;
		FHitResult HitResult;

		OwnerCharacter->GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility);
		if (AOWHCharacter* OWHCharacter = Cast<AOWHCharacter>(OwnerCharacter))
		{
			OWHCharacter->SetIsClimbing(true);
			if (CharacterMovementComponent->Velocity.IsZero())
			{
				OWHCharacter->TaperOffSFX();
			}
			else
			{
				OWHCharacter->TaperOnSFX();
			}
		}

		if (HitResult.bBlockingHit)
		{
			OwnerCharacter->GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &UOWHGameplayAbility_Climb::DoClimb, OwnerCharacter));
		}
		else
		{
			if (UOWHAbilitySystemComponent* OwningAbilityComponent = Cast<UOWHAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwningActorFromActorInfo())))
			{
				OwningAbilityComponent->CancelAbilityByClass(GetClass());
			}
			else
			{
				K2_CancelAbility();;
			}
			return;
		}
	}
}

void UOWHGameplayAbility_Climb::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ACharacter* OwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor);
	Cast<AOWHCharacter>(OwnerCharacter)->SetIsClimbing(false);
	if (OwnerCharacter == nullptr || OwnerCharacter->GetCharacterMovement() == nullptr) { return; }

	if (UCharacterMovementComponent* CharacterMovementComponent = OwnerCharacter->GetCharacterMovement())
	{
		CharacterMovementComponent->SetMovementMode(MOVE_Walking);
		CharacterMovementComponent->bOrientRotationToMovement = true;
	}
}


