// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogues/OWHDialogueComponent.h"

UOWHDialogueComponent::UOWHDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UOWHDialogueComponent::StartDialogue(const FString& InNPC_ID)
{
	if (DialoguesTable && !InNPC_ID.IsEmpty() && !bIsInDialogue && !FinishedDialogueIDs.Contains(InNPC_ID))
	{
		if (FDialogueDataPerNPC* DialogueData = DialoguesTable->FindRow<FDialogueDataPerNPC>(FName(InNPC_ID), ""))
		{
			bIsInDialogue = true;
			NPC_ID = InNPC_ID;
			CurrentDialogues = DialogueData;

			if (!CurrentDialogues->bCanRestartDialogue)
			{
				FinishedDialogueIDs.AddUnique(InNPC_ID);
			}

			int StartIndex = 0;
			for (FDialogueData& Dialogue : DialogueData->DialogueDataArray)
			{
				Dialogue.DialogueIndex = StartIndex;
				StartIndex++;
			}

			OnStartDialogue.Broadcast(InNPC_ID, DialogueData->SpeakerName);
			GoToNextDialogue(0);
		}
	}
}

void UOWHDialogueComponent::GoToNextDialogue(int NextDialogueIndex)
{
	if (!bIsInDialogue || !CurrentDialogues)
	{
		return;
	}
	if (NextDialogueIndex >= 0 && CurrentDialogues->DialogueDataArray.Num() > NextDialogueIndex)
	{
		const FDialogueData& NextDialogue = CurrentDialogues->DialogueDataArray[NextDialogueIndex];
		UE_LOG(LogTemp, Warning, TEXT("Next Index: %d | Dialogue: %s | Num: %d"), NextDialogueIndex, *NextDialogue.Dialogue, CurrentDialogues->DialogueDataArray.Num())
		OnNextDialogue.Broadcast(NPC_ID, NextDialogue);
	}
	else
	{
		bIsInDialogue = false;
		OnEndDialogue.Broadcast(NPC_ID);
	}
}

int UOWHDialogueComponent::GetNextDialogueIndexForResponse(int CurrentDialogueIndex, const FString& Response) const
{
	if (CurrentDialogues && CurrentDialogues->DialogueDataArray.Num() > CurrentDialogueIndex)
	{
		const TMap<FString, int>& ResponsesMap = CurrentDialogues->DialogueDataArray[CurrentDialogueIndex].ResponsesToNextDialogueIndex;
		UE_LOG(LogTemp, Warning, TEXT("Current Index: %d | Response: %s | Num: %d"), CurrentDialogueIndex, *Response, ResponsesMap.Num());
		if (ResponsesMap.Contains(Response) && CurrentDialogues->DialogueDataArray.Num() > ResponsesMap[Response])
		{
			return ResponsesMap[Response];
		}
	}
	return -1;
}

void UOWHDialogueComponent::EndDialogue(const FString& InNPC_ID)
{
	bIsInDialogue = false;
	OnEndDialogue.Broadcast(NPC_ID);
}

void UOWHDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
}
