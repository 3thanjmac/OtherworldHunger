// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/OWHIngredient.h"
#include "Components/ActorComponent.h"
#include "Components/OWHItem.h"
#include "OWHCharacterInventory.generated.h"

USTRUCT(BlueprintType)
struct FItemSlot
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool HasItem = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int StackSize = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UOWHItem* ItemAsset;

	bool operator==(UOWHItem* Other)
	{
		return this->ItemAsset == Other;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAddItemDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OTHERWORLDHUNGER_API UOWHCharacterInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	UOWHCharacterInventory();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void AddIngredient(UOWHItem* AddedItem);

	UFUNCTION(BlueprintCallable)
	void RemoveIngredient(AOWHIngredient* Ingredient);

	bool HasIngredients(TMap<FGameplayTag, int32> Ingredients);

	void RemoveIngredients(TMap<FGameplayTag, int32> Ingredients);

	int32 GetIngredientCount(const FGameplayTag& IngredientTag) const;

	UFUNCTION(BlueprintCallable)
	void DisplayIngredients();

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FAddItemDelegate OnSetItem;

	UPROPERTY(BlueprintReadWrite)
	TMap<FGameplayTag, int32> IngredientMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemSlot> InventorySlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int InventorySize = 16;

protected:
	virtual void BeginPlay() override;

};
