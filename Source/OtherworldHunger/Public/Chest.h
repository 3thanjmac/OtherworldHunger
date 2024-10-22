// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OWHInteractableInterface.h"
#include "Chest.generated.h"

UCLASS()
class OTHERWORLDHUNGER_API AChest : public AActor, public IOWHInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_InteractWithChest(APawn* InstigatorPawn);

	virtual AActor* Interact_Implementation(APawn* InstigatorPawn) override;

};
