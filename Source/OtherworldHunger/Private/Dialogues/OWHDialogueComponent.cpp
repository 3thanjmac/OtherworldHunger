// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogues/OWHDialogueComponent.h"

UOWHDialogueComponent::UOWHDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UOWHDialogueComponent::StartDialogue(FString InDialogueID)
{
	if (DialoguesTable && !InDialogueID.IsEmpty() && !bIsInDialogue)
	{
		if (FDialogueData* DialogueData = DialoguesTable->FindRow<FDialogueData>(FName(InDialogueID), ""))
		{
			bIsInDialogue = true;
			DialogueID = InDialogueID;
			CurrentDialogues = DialogueData->Dialogues;
			OnStartDialogue.Broadcast(InDialogueID);
			GoToNextDialogue();
		}
	}
}

void UOWHDialogueComponent::GoToNextDialogue()
{
	if (!bIsInDialogue)
	{
		return;
	}
	if (CurrentDialogues.Num() != 0)
	{
		const FString NextDialogue = CurrentDialogues[0];
		CurrentDialogues.RemoveAt(0);
		OnNextDialogue.Broadcast(DialogueID, NextDialogue);
	}
	else
	{
		bIsInDialogue = false;
		CurrentDialogues.Empty();
		OnEndDialogue.Broadcast(DialogueID);
	}
}

void UOWHDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
}
