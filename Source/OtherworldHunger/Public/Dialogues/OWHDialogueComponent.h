// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "OWHDialogueComponent.generated.h"

USTRUCT(BlueprintType)
struct FDialogueData
{
	GENERATED_BODY();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogues")
	int DialogueIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogues")
	FString Dialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogues")
	TMap<FString, int> ResponsesToNextDialogueIndex;
};

USTRUCT(BlueprintType)
struct FDialogueDataPerNPC : public FTableRowBase
{
	GENERATED_BODY();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogues")
	FString SpeakerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogues")
	bool bCanRestartDialogue = false;

	// Every NPC in the Data Table will have this array.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogues")
	TArray<FDialogueData> DialogueDataArray;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDialogueNotifyDelegate, const FString&, NPC_ID, const FDialogueData&, Dialogue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDialogueDelegate, const FString&, NPC_ID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDialogueStartDelegate, const FString&, NPC_ID, const FString&, SpeakerName);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTHERWORLDHUNGER_API UOWHDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOWHDialogueComponent();

	UFUNCTION(BlueprintCallable)
	void StartDialogue(const FString& InNPC_ID);

	UFUNCTION(BlueprintCallable)
	void GoToNextDialogue(int NextDialogueIndex);

	UFUNCTION(BlueprintCallable)
	int GetNextDialogueIndexForResponse(int CurrentDialogueIndex, const FString& Response) const;

	UFUNCTION(BlueprintCallable)
	void EndDialogue(const FString& InNPC_ID);

	UFUNCTION(BlueprintCallable)
	bool IsInDialogue() const { return bIsInDialogue; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogues")
	UDataTable* DialoguesTable = nullptr;

	bool bIsInDialogue = false;

	FString NPC_ID;
	FDialogueDataPerNPC* CurrentDialogues;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDialogueStartDelegate OnStartDialogue;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDialogueNotifyDelegate OnNextDialogue;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDialogueDelegate OnEndDialogue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FString> FinishedDialogueIDs;

protected:
	virtual void BeginPlay() override;

};
