// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsAttributeSet.h"
#include "OWHCharacter.h"

void UStatsAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
}

void UStatsAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	float ALevel = GetLevel();
	float AXP = GetXP();

	if (AOWHCharacter* Character = Cast<AOWHCharacter>(GetOwningActor()))
	{
		// Get Stats component and calc stuff.
	}

	// Get the XP threshold for Level + 1 and increase level if possible.

	// On Level increase, award points.
}
