// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/OWH_AbilitySystemLibrary.h"
#include "OWHAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "OWHCharacter.h"

void UOWH_AbilitySystemLibrary::GiveXP(AOWHCharacter* Character, int XP)
{
	if (Character)
	{
		Character->GiveXP(XP);
	}
}

void UOWH_AbilitySystemLibrary::SetLevel(AOWHCharacter* Character, int Level)
{
	if (Character)
	{
		Character->SetLevel(Level);
	}
}

void UOWH_AbilitySystemLibrary::GivePoints(AOWHCharacter* Character, int Points)
{
	if (Character)
	{
		Character->GivePoints(Points);
	}
}
