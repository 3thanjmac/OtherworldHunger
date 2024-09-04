// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAttributeSet.h"

void UBasicAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
    /** Stamina Clamp */
    if (Attribute == GetStaminaAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0, MaxStamina.GetBaseValue());
    }
    /** Health Clamp */
    else if (Attribute == GetHealthAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0, MaxHealth.GetBaseValue());
    }
}