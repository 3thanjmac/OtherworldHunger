// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DialogueWidget.generated.h"

/**
 * 
 */
UCLASS()
class OTHERWORLDHUNGER_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (BindWidget))
	UImage* BackgroundImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (BindWidget))
	UTextBlock* DialogueText = nullptr;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowDialogue(const FString& Dialogue);

	UFUNCTION(BlueprintCallable)
	void StartDialogue(const FString& DialogueID);

	UFUNCTION(BlueprintCallable)
	void NextDialogue(const FString& DialogueID, const FString& Dialogue);

	UFUNCTION(BlueprintCallable)
	void EndDialogue(const FString& DialogueID);
};
