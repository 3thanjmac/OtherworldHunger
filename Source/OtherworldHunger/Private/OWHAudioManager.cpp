// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHAudioManager.h"
#include "Components/AudioComponent.h"

AOWHAudioManager::AOWHAudioManager()
{
	BGAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGAudioComponent"));
	BGAudioComponent->SetupAttachment(GetRootComponent());

	AmbienceAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AmbienceAudioComponent"));
	AmbienceAudioComponent->SetupAttachment(GetRootComponent());

	SfxAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SfxAudioComponent"));
	SfxAudioComponent->SetupAttachment(GetRootComponent());

	PrimaryActorTick.bCanEverTick = true;
}

void AOWHAudioManager::StartLevel()
{
	if (BackGroundMusic && BGAudioComponent)
	{
		BackGroundMusic->bLooping = true;
		BackGroundMusic->Volume = BackGroundMusicVolume;
		BGAudioComponent->Sound = BackGroundMusic;
		BGAudioComponent->Play();
	}

	if (AmbiantAudio && AmbienceAudioComponent)
	{
		AmbiantAudio->bLooping = true;
		AmbiantAudio->Volume = AmbiantAudioVolume;
		AmbienceAudioComponent->Sound = AmbiantAudio;
		AmbienceAudioComponent->Play();
	}
}

void AOWHAudioManager::PlaySound(const FGameplayTag& AudioTag)
{
	if (SfxAudioComponent == nullptr || SFXData.Contains(AudioTag) == false || CurrentSFXTag == AudioTag) { return; }

	CurrentSFXTag = AudioTag;
	SfxAudioComponent->Sound = SFXData[AudioTag];
	SfxAudioComponent->Play();
}

void AOWHAudioManager::StopSound(const FGameplayTag& AudioTag)
{
	if (CurrentSFXTag.IsValid() == false || CurrentSFXTag != AudioTag || SFXData.Contains(AudioTag) == false) { return; }

	CurrentSFXTag = FGameplayTag();

	if (SFXData[AudioTag]->bLooping == false) { return; }

	SfxAudioComponent->Stop();
}

void AOWHAudioManager::TaperOn()
{
	SfxAudioComponent->AdjustVolume(0.1f, 1.0f, EAudioFaderCurve::Sin);
}

void AOWHAudioManager::TaperOff()
{
	SfxAudioComponent->AdjustVolume(0.1f, 0.0f, EAudioFaderCurve::Sin);
}