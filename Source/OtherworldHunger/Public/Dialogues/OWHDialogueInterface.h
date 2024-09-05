// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OWHCharacter.h"
#include "OWHDialogueInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOWHDialogueInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OTHERWORLDHUNGER_API IOWHDialogueInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartOrContinueDialogue(AOWHCharacter* Player);

};
