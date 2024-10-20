// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/OWHCharacterInventory.h"
#include "GameplayTagsManager.h"
#include "OWHCharacter.h"


UOWHCharacterInventory::UOWHCharacterInventory()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UOWHCharacterInventory::BeginPlay()
{
	Super::BeginPlay();
}

void UOWHCharacterInventory::AddIngredient(UOWHItem* AddedItem)
{
	if (AddedItem == nullptr) { return; }

	FGameplayTag Ingredient = AddedItem->NameTag;
	for (TPair<FGameplayTag, int32>& Pair : IngredientMap)
	{
		if (Pair.Key.IsValid() == false) { continue; }

		if (Pair.Key == Ingredient)
		{
			for (FItemSlot& CheckSlot : ItemSlots)
			{
				if (CheckSlot.HasItem)
				{
					if (CheckSlot.ItemAsset == AddedItem && CheckSlot.StackSize < AddedItem->MaxStackSize)
					{
						CheckSlot.StackSize++;
						Pair.Value++;
						if (AOWHCharacter* OwnerCharacter = Cast<AOWHCharacter>(GetOwner()))
						{
							OwnerCharacter->OnIngredientAddedToInventory(Ingredient, IngredientMap[Ingredient]);
						}

						OnSetItem.Broadcast();
						return;
					}
				}
				else { continue; }
			}
		}
	}

	IngredientMap.Add(Ingredient, 1);

	for (FItemSlot& CheckSlot : ItemSlots)
	{
		if (!CheckSlot.HasItem)
		{
			CheckSlot.HasItem = true;
			CheckSlot.StackSize++;
			CheckSlot.ItemAsset = AddedItem;
			OnSetItem.Broadcast();
			return;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Inventory Full!"));
		}
	}

	if (AOWHCharacter* OwnerCharacter = Cast<AOWHCharacter>(GetOwner()))
	{
		OwnerCharacter->OnIngredientAddedToInventory(Ingredient, IngredientMap[Ingredient]);
	}
}

void UOWHCharacterInventory::AddItem(UOWHItem* AddedItem)
{
	if (AddedItem->NameTag.ToString().StartsWith("Weapon", ESearchCase::IgnoreCase))
	{
		for (FItemSlot& CheckSlot : WeaponSlots)
		{
			if (!CheckSlot.HasItem)
			{
				CheckSlot.HasItem = true;
				CheckSlot.StackSize = 1;
				CheckSlot.ItemAsset = AddedItem;
				OnSetWeapon.Broadcast();
				return;
			}
		}
	}
	else
	{
		for (FItemSlot& CheckSlot : FoodSlots)
		{
			if (CheckSlot.HasItem)
			{
				if (CheckSlot.ItemAsset == AddedItem && CheckSlot.StackSize < AddedItem->MaxStackSize)
				{
					CheckSlot.StackSize++;
					OnSetFood.Broadcast();
					return;
				}
			}
			else { continue; }
		}

		for (FItemSlot& CheckSlot : FoodSlots)
		{
			if (!CheckSlot.HasItem)
			{
				CheckSlot.HasItem = true;
				CheckSlot.StackSize++;
				CheckSlot.ItemAsset = AddedItem;
				OnSetFood.Broadcast();
				return;
			}
		}
	}
}

bool UOWHCharacterInventory::HasIngredients(TMap<FGameplayTag, int32> Ingredients)
{
	if (IngredientMap.Num() == 0) { return false; }

	for (TPair<FGameplayTag, int32>& Ingredient : Ingredients)
	{
		bool IngredientFount = false;
		for (TPair<FGameplayTag, int32>& Pair : IngredientMap)
		{
			if (Pair.Key == Ingredient.Key)
			{
				IngredientFount = true;
				if (Pair.Value < Ingredient.Value)
				{
					return false;
				}

				break;
			}
		}

		if (IngredientFount == false)
		{
			return false;
		}
	}

	return true;
}

void UOWHCharacterInventory::RemoveIngredient(const FGameplayTag& IngredientTag)
{
	if (IngredientTag.IsValid() == false) { return; }

	if (IngredientMap.Find(IngredientTag))
	{
		if (IngredientMap[IngredientTag] > 0)
		{
			IngredientMap[IngredientTag] = IngredientMap[IngredientTag] - 1;
			if (IngredientMap[IngredientTag] == 0)
			{
				IngredientMap.Remove(IngredientTag);
			}
		}
	}
}

void UOWHCharacterInventory::RemoveIngredients(TMap<FGameplayTag, int32> Ingredients)
{
	TArray<FGameplayTag> IngredientsToRemove;

	for (TPair<FGameplayTag, int32>& Ingredient : Ingredients)
	{
		for (TPair<FGameplayTag, int32>& Pair : IngredientMap)
		{
			if (Pair.Key == Ingredient.Key)
			{
				FItemSlot* SlotPtr = ItemSlots.FindByKey(Ingredient.Key);

				SlotPtr->StackSize -= Ingredient.Value;
				Pair.Value -= Ingredient.Value;

				if (Pair.Value <= 0)
				{
					IngredientsToRemove.Add(Pair.Key);
				}
				if (SlotPtr->StackSize <= 0)
				{
					FItemSlot EmptySlot;
					*SlotPtr = EmptySlot;
				}
			}
		}
	}

	for (FGameplayTag Ingredient : IngredientsToRemove)
	{
		IngredientMap.Remove(Ingredient);
	}

	OnSetItem.Broadcast();
}

int32 UOWHCharacterInventory::GetIngredientCount(const FGameplayTag& IngredientTag) const
{
	if (IngredientMap.Contains(IngredientTag) == false) { return 0; }

	return IngredientMap[IngredientTag];
}

void UOWHCharacterInventory::DisplayIngredients()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Yellow, TEXT("INGREDIENT LIST"));
	for (const TPair<FGameplayTag, int32>& Pair : IngredientMap)
	{
		FString KeyString = Pair.Key.GetTagName().ToString();
		FString DebugMessage = FString::Printf(TEXT("Key: %s, Value: %d"), *KeyString, Pair.Value);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, DebugMessage);
		}
		if (AOWHCharacter* OwnerCharacter = Cast<AOWHCharacter>(GetOwner()))
		{
			for (int IngredientCount = 0; IngredientCount < Pair.Value; IngredientCount++)
				OwnerCharacter->OnIngredientAddedToInventory(Pair.Key, IngredientMap[Pair.Key]);
		}
	}
}


void UOWHCharacterInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
