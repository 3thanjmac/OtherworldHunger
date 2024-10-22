// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "OWHItem.generated.h"

/**
 * 
 */
UCLASS()
class OTHERWORLDHUNGER_API UOWHItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag NameTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString NameString;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int MaxStackSize = 16;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMesh* Mesh;

};
