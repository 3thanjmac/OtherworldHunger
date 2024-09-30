// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogues/DialogueWidget.h"
#include "Dialogues/OWHDialogueComponent.h"

UDialogueWidget::UDialogueWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsFocusable = true;
}

void UDialogueWidget::StartDialogue(const FString& NPC_ID)
{
	SetVisibility(ESlateVisibility::Visible);
}

void UDialogueWidget::NextDialogue(const FString& NPC_ID, const FDialogueData& DialogueData)
{
	ShowDialogue(DialogueData);
}

void UDialogueWidget::EndDialogue(const FString& NPC_ID)
{
	SetVisibility(ESlateVisibility::Collapsed);
}
