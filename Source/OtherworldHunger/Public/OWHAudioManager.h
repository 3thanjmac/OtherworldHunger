// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "OWHAudioManager.generated.h"

class UAudioComponent;

UCLASS()
class OTHERWORLDHUNGER_API AOWHAudioManager : public AActor
{
	GENERATED_BODY()

public:
	AOWHAudioManager();

	void StartLevel();
	void PlaySound(const FGameplayTag& AudioTag);
	void StopSound(const FGameplayTag& AudioTag);
	void TaperOn();
	void TaperOff();

public:
	UPROPERTY(EditAnywhere)
	USoundWave* BackGroundMusic;
	UPROPERTY(EditAnywhere)
	float BackGroundMusicVolume;

	UPROPERTY(EditAnywhere)
	USoundWave* AmbiantAudio;
	UPROPERTY(EditAnywhere)
	float AmbiantAudioVolume;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, USoundWave*> SFXData;

private:
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* BGAudioComponent;

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AmbienceAudioComponent;

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* SfxAudioComponent;
	
	/** Cache current playing SFX tag for easier access */
	FGameplayTag CurrentSFXTag;
};
