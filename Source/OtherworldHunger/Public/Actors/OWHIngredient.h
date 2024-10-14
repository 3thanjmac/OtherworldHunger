// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OWHInteractableInterface.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Components/OWHItem.h"
#include "OWHIngredient.generated.h"

class USphereComponent;

UCLASS(BlueprintType)
class OTHERWORLDHUNGER_API AOWHIngredient : public AActor, public IOWHInteractableInterface
{
	GENERATED_BODY()

public:
	AOWHIngredient();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UOWHItem* ItemDataAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* IngredientMesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* InteractSphere;

protected:
	virtual AActor* Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag IngredientTag;

	//GETTERS AND SETTERS
public:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintGetter)
	UOWHItem* GetIngredientAsset() const;

	UFUNCTION(BlueprintSetter)
	void SetIngredientTag(const FGameplayTag& InIngredientTag);

	UFUNCTION(BlueprintSetter)
	void SetIngredientMesh(UStaticMeshComponent* IngredientMeshComp);

};
