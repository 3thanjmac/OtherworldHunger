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

	UDialogueWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (BindWidget))
	UImage* BackgroundImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (BindWidget))
	UTextBlock* DialogueText = nullptr;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowDialogue(const FDialogueData& DialogueData);

	UFUNCTION(BlueprintCallable)
	void StartDialogue(const FString& DialogueID);

	UFUNCTION(BlueprintCallable)
	void NextDialogue(const FString& DialogueID, const FDialogueData& DialogueData);

	UFUNCTION(BlueprintCallable)
	void EndDialogue(const FString& DialogueID);
};
