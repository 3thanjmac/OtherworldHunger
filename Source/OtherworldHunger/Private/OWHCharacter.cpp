// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHCharacter.h"
#include "Abilities/GameplayAbility.h"
#include "Actors/OWHIngredient.h"
#include "BasicAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "OWHAudioManager.h"
#include "OWHPlayerHUD.h"
#include "OWHGameplayAbility_Climb.h"
#include "OWHGameplayAbility_Interact.h"
#include "OWHInteractableInterface.h"
#include "OWHAbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "Components/OWHCharacterInventory.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Dialogues/OWHDialogueComponent.h"

AOWHCharacter::AOWHCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	AbilitySystemComponent = CreateDefaultSubobject<UOWHAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	QuestManagerComponent = CreateDefaultSubobject<UOWHQuestsManager>(TEXT("QuestManagerComp"));

	DialogueComponent = CreateDefaultSubobject<UOWHDialogueComponent>(TEXT("DialogueComponent"));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	CharacterInventory = CreateDefaultSubobject<UOWHCharacterInventory>(TEXT("CharacterInventoryComp"));
	PrimaryActorTick.bCanEverTick = true;
}

void AOWHCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AOWHCharacter::OnHit);

	if (IsValid(AbilitySystemComponent))
	{
		Attributes = AbilitySystemComponent->GetSet<UBasicAttributeSet>();
	}
}

void AOWHCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		OWHController = PlayerController;
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (GetLocalRole() == ENetRole::ROLE_Authority && GetOWHAbilitySystemComponent())
	{
		GetOWHAbilitySystemComponent()->InitAbilityActorInfo(this, this);
		GetOWHAbilitySystemComponent()->InitAbilities();
	}

	if (QuestManagerComponent)
	{
		QuestManagerComponent->InitializeComp();
	}

	if (GetAudioManager())
	{
		AudioManager->StartLevel();
	}
}

void AOWHCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOWHCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AOWHCharacter::Look);

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	}
}

void AOWHCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	//PlaySFX(FootStepsTag);

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AOWHCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AOWHCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bIsClimbing)
	{
		GetOWHAbilitySystemComponent()->ActivateAbilityByTag(Climb);
	}
}

UOWHCharacterInventory* AOWHCharacter::GetCharacterInventory() const
{
	return CharacterInventory;
}

AOWHAudioManager* AOWHCharacter::GetAudioManager()
{
	if (AudioManager) { return AudioManager; }

	for (TActorIterator<AOWHAudioManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (*ActorItr != nullptr)
		{
			AudioManager = *ActorItr;
			break;
		}
	}

	return AudioManager;
}

UOWHAbilitySystemComponent* AOWHCharacter::GetOWHAbilitySystemComponent() const
{
	return Cast<UOWHAbilitySystemComponent>(AbilitySystemComponent);
}

bool AOWHCharacter::GetIsClimbing()
{
	return bIsClimbing;
}

void AOWHCharacter::OnRecipeUpdate(FRecipeDataTable* NewRecipe)
{
	// tell player
	// update ui
}

void AOWHCharacter::SetPlayerHUD(UUserWidget* InPlayerHUD)
{
	PlayerHUD = Cast<UOWHPlayerHUD>(InPlayerHUD);

	if (PlayerHUD && QuestManagerComponent && QuestManagerComponent->LoadRecipe())
	{
		PlayerHUD->UpdateRecipe(QuestManagerComponent->GetCurrentRecipe());
	}
}

void AOWHCharacter::SetIsClimbing(bool climbing)
{
	bIsClimbing = climbing;
}

void AOWHCharacter::OnIngredientAddedToInventory(const FGameplayTag& IngredientTag, int32 NewCount)
{
	if (PlayerHUD && QuestManagerComponent && QuestManagerComponent->GetCurrentRecipeName() != "")
	{
		PlayerHUD->OnIngredientAddedToInventory(IngredientTag, NewCount);
	}
}

void AOWHCharacter::UpdateRecipe(FRecipeDataTable* NewRecipe)
{
	if (PlayerHUD && QuestManagerComponent && QuestManagerComponent->GetCurrentRecipeName() != "")
	{
		PlayerHUD->UpdateRecipe(NewRecipe);

		if (CharacterInventory)
		{
			TArray<FGameplayTag> IngredientsTags;
			NewRecipe->Ingredients.GetKeys(IngredientsTags);

			for (const FGameplayTag& IngredientTag : IngredientsTags)
			{
				PlayerHUD->OnIngredientAddedToInventory(IngredientTag, CharacterInventory->GetIngredientCount(IngredientTag));
			}
		}
	}
}

void AOWHCharacter::ShowNotification(FString Message, ENotificationType NotificationType)
{
	if (PlayerHUD == nullptr) { return; }

	PlayerHUD->ShowNotification(Message, NotificationType);
}

void AOWHCharacter::ShowConfirmation(FString Message, FString LevelName)
{
	if (PlayerHUD == nullptr) { return; }

	PlayerHUD->ShowConfirmation(Message, LevelName);
	PauseGame();
}

void AOWHCharacter::PauseGame()
{
	FInputModeUIOnly UIOnly;
	OWHController->SetShowMouseCursor(true);
	OWHController->SetInputMode(UIOnly);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	UGameplayStatics::SetViewportMouseCaptureMode(GetWorld(), EMouseCaptureMode::NoCapture);
}

void AOWHCharacter::ResumeGame()
{
	FInputModeGameOnly GameOnly;
	OWHController->SetShowMouseCursor(false);
	OWHController->SetInputMode(GameOnly);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	UGameplayStatics::SetViewportMouseCaptureMode(GetWorld(), EMouseCaptureMode::CapturePermanently);
}

void AOWHCharacter::PlaySFX(const FGameplayTag& AudioTag)
{
	if (AudioTag.IsValid() == false || GetAudioManager() == nullptr) { return; }

	AudioManager->PlaySound(AudioTag);
}

void AOWHCharacter::StopSFX(const FGameplayTag& AudioTag)
{
	if (AudioTag.IsValid() == false || GetAudioManager() == nullptr) { return; }

	AudioManager->StopSound(AudioTag);
}

float AOWHCharacter::FallDamage(float Velocity)
{
	/* Min velocity for fall damage set by addend */
	Velocity = (Velocity + 1600) * 0.1;
	/* Exponential calculation */
	Velocity = FMath::Pow(Velocity, 2) * -0.005;
	return FMath::RoundToInt(Velocity);
}