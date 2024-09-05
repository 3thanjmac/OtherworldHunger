// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "OWHQuestsManager.h"
#include "OWHNotificationUIScreen.h"
#include "OWHConfirmUIScreen.h"
#include "OWHCharacter.generated.h"

class UOWHDialogueComponent;

UCLASS()
class OTHERWORLDHUNGER_API AOWHCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AOWHCharacter();

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Recipe */
	void OnRecipeUpdate(FRecipeDataTable* NewRecipe);
	void OnIngredientAddedToInventory(const FGameplayTag& IngredientTag, int32 NewCount);
	void UpdateRecipe(FRecipeDataTable* NewRecipe);

	/** UI */
	void ShowNotification(FString Message, ENotificationType NotificationType);
	void ShowConfirmation(FString Message, FString LevelName);

	UFUNCTION(BlueprintCallable)
	void PauseGame();
	UFUNCTION(BlueprintCallable)
	void ResumeGame();

	/** Audio */
	void PlaySFX(const FGameplayTag& AudioTag);
	void StopSFX(const FGameplayTag& AudioTag);

	/** Getters */
	class UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	class UOWHCharacterInventory* GetCharacterInventory() const;
	UOWHQuestsManager* GetQuestsManager() const { return QuestManagerComponent; }
	UOWHDialogueComponent* GetDialogueComponent() const { return DialogueComponent; }
	class AOWHAudioManager* GetAudioManager();
	class UOWHAbilitySystemComponent* GetOWHAbilitySystemComponent() const;

	UFUNCTION(BlueprintGetter)
	bool GetIsClimbing();

	UPROPERTY(BlueprintReadOnly)
	bool bIsClimbing = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsCookingEnabled = false;

	/** Setters */
	UFUNCTION(BlueprintCallable)
	void SetPlayerHUD(UUserWidget* InPlayerHUD);
	void SetIsClimbing(bool climbing);

	/** Fall Damage */
	UFUNCTION(BlueprintCallable)
	float FallDamage(float Velocity);

protected:
	virtual void PossessedBy(AController* NewController) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	void StopMove(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FGameplayTag Climb;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	const class UBasicAttributeSet* Attributes;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	class UOWHCharacterInventory* CharacterInventory;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Ability system comp */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystemComponent;

	/** Quest Manager comp */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Recipe, meta = (AllowPrivateAccess = "true"))
	UOWHQuestsManager* QuestManagerComponent;

	/** Dialogue comp */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Recipe, meta = (AllowPrivateAccess = "true"))
	UOWHDialogueComponent* DialogueComponent;

	/** MappingContext */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
	FGameplayTag FootStepsTag;

	/** Cached Ref.s */
	class UOWHPlayerHUD* PlayerHUD;
	class AOWHAudioManager* AudioManager;
	APlayerController* OWHController;
};
