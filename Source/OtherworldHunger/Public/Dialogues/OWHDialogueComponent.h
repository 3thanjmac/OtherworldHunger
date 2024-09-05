// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "OWHDialogueComponent.generated.h"

USTRUCT(BlueprintType)
struct FDialogueData : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogues")
	TArray<FString> Dialogues;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDialogueNotifyDelegate, const FString&, DialogueID, const FString&, Dialogue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDialogueDelegate, const FString&, DialogueID);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTHERWORLDHUNGER_API UOWHDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOWHDialogueComponent();

	UFUNCTION(BlueprintCallable)
	void StartDialogue(FString InDialogueID);

	UFUNCTION(BlueprintCallable)
	void GoToNextDialogue();

	UFUNCTION(BlueprintCallable)
	bool IsInDialogue() const { return bIsInDialogue; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogues")
	UDataTable* DialoguesTable = nullptr;

	bool bIsInDialogue = false;

	FString DialogueID;
	TArray<FString> CurrentDialogues;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDialogueDelegate OnStartDialogue;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDialogueNotifyDelegate OnNextDialogue;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDialogueDelegate OnEndDialogue;

protected:
	virtual void BeginPlay() override;

};
