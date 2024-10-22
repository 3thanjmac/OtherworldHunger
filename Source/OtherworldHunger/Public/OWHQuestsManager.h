// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "OWHQuestsManager.generated.h"

USTRUCT(BlueprintType)
struct FRecipeDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString RecipeName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, int32> Ingredients;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTHERWORLDHUNGER_API UOWHQuestsManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	void InitializeComp();

	FString GetCurrentRecipeName() { return CurrentRecipeName; };
	FRecipeDataTable* GetCurrentRecipe() { return CurrentRecipeName != "" ? Recipes[CurrentRecipeName] : nullptr; };

	UFUNCTION(BlueprintCallable)
	bool LoadRecipe();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UDataTable* RecipeDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	int32 CurrentRecipeIndex = -1;
	
	/** Cache player recipes */
	TMap<FString, FRecipeDataTable*> Recipes;

	TArray<FString> RecipesNames;
	FString CurrentRecipeName = "";

};
