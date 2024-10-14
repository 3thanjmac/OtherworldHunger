// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/OWHIngredient.h"
#include "Components/SphereComponent.h"
#include "OWHCharacter.h"
#include "Components/OWHCharacterInventory.h"

// Sets default values
AOWHIngredient::AOWHIngredient()
{
	PrimaryActorTick.bCanEverTick = true;

	IngredientMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	InteractSphere->InitSphereRadius(50.0f);
	InteractSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = IngredientMesh;
	InteractSphere->SetupAttachment(RootComponent);

	// Set up collision responses
	InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &AOWHIngredient::OnOverlapBegin);
	InteractSphere->OnComponentEndOverlap.AddDynamic(this, &AOWHIngredient::OnOverlapEnd);
}

AActor* AOWHIngredient::Interact_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn == nullptr) { return nullptr; }

	AOWHCharacter* OwnerCharacter = Cast<AOWHCharacter>(InstigatorPawn);

	if (OwnerCharacter && OwnerCharacter->GetCharacterInventory())
	{
		OwnerCharacter->GetCharacterInventory()->AddIngredient(GetIngredientAsset());
		IngredientMesh->DestroyComponent();
		InteractSphere->DestroyComponent();
		OwnerCharacter->GetCharacterInventory()->DisplayIngredients();
		return this;
	}

	return nullptr;
}

void AOWHIngredient::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap Begin"));
	}
}

void AOWHIngredient::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap Ended"));
}

UOWHItem* AOWHIngredient::GetIngredientAsset() const
{
	if (ItemDataAsset == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemDataAsset not provided for this ingredient."));
		return nullptr;
	}
	return ItemDataAsset;
}

void AOWHIngredient::SetIngredientTag(const FGameplayTag& InIngredientTag)
{
	IngredientTag = InIngredientTag;
}


void AOWHIngredient::SetIngredientMesh(UStaticMeshComponent* IngredientMeshComp)
{
	IngredientMesh = IngredientMeshComp;
}
