// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "OWH_AbilitySystemLibrary.generated.h"

/**
 *
 */
UCLASS(meta = (ScriptName = "OWH_AbilitySystemLibrary"))
class OTHERWORLDHUNGER_API UOWH_AbilitySystemLibrary : public UAbilitySystemBlueprintLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = Ability)
	static void GiveXP(AOWHCharacter* Character, int XP);

	UFUNCTION(BlueprintCallable, Category = Ability)
	static void SetLevel(AOWHCharacter* Character, int Level);

	UFUNCTION(BlueprintCallable, Category = Ability)
	static void GivePoints(AOWHCharacter* Character, int Points);

};

