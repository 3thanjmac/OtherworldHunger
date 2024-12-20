// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHPlayerHUD.h"
#include "OWHRecipeUIScreen.h"

//TODO: Add notifications

void UOWHPlayerHUD::UpdateRecipe(FRecipeDataTable* NewRecipe)
{
	if (WBP_OWH_Recipe == nullptr) { return; }

	if (NewRecipe == nullptr)
	{
		WBP_OWH_Recipe->Hide();
	}

	WBP_OWH_Recipe->UpdateRecipe(NewRecipe);
}

void UOWHPlayerHUD::OnIngredientAddedToInventory(const FGameplayTag& IngredientTag, int32 NewCount)
{
	if (WBP_OWH_Recipe == nullptr) { return; }

	WBP_OWH_Recipe->UpdateIngredientCount(IngredientTag, NewCount);
}

void UOWHPlayerHUD::ShowNotification(FString Message, ENotificationType NotificationType)
{
	if (WBP_OWH_Notification == nullptr) { return; }

	WBP_OWH_Notification->ShowNotification(Message, NotificationType);
}

void UOWHPlayerHUD::ShowConfirmation(FString Message, FString LevelName)
{
	if (WBP_Confirm == nullptr) { return; }

	WBP_Confirm->Show(Message, LevelName);
}