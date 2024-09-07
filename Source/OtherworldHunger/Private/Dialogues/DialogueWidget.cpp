// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogues/DialogueWidget.h"

void UDialogueWidget::StartDialogue(const FString& DialogueID)
{
	SetVisibility(ESlateVisibility::Visible);
}

void UDialogueWidget::NextDialogue(const FString& DialogueID, const FString& Dialogue)
{
	ShowDialogue(Dialogue);
}

void UDialogueWidget::EndDialogue(const FString& DialogueID)
{
	SetVisibility(ESlateVisibility::Collapsed);
}
