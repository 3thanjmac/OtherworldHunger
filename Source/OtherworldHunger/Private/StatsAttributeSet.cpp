// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsAttributeSet.h"
#include "OWHCharacter.h"

void UStatsAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UStatsAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	// If XP Changes.
	if (Attribute == GetXPAttribute())
	{
		float PlayerLevel = GetLevel();
		float PlayerXP = GetXP();

		if (PlayerLevel > 0.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("XP after Gain: %f | Player Level: %f"), PlayerXP, PlayerLevel);

			if (AOWHCharacter* Character = Cast<AOWHCharacter>(GetOwningActor()))
			{
				// Array[0] XP to reach Level 2 from the current level 1.
				const TArray<int32>& XPvsLevel = Character->GetXPToReachNextLevelArray();
				if (XPvsLevel.Num() >= PlayerLevel)
				{
					int NextLevelXP = XPvsLevel[PlayerLevel - 1];
					UE_LOG(LogTemp, Warning, TEXT("XP to Next Level: %d"), NextLevelXP);
					if (PlayerXP >= NextLevelXP)
					{
						SetLevel(PlayerLevel + 1);
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Max Level"));
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Player Level <= 0.0f"));
		}
	}
	else if (Attribute == GetLevelAttribute())
	{
		int PlayerLevel = (int)GetLevel();
		UE_LOG(LogTemp, Warning, TEXT("Level Up to %d"), PlayerLevel);

		if (AOWHCharacter* Character = Cast<AOWHCharacter>(GetOwningActor()))
		{
			int PointsPerLevel = Character->PointsGrantedPerLevel;
			if (PointsPerLevel > 0)
			{
				SetPoints(GetPoints() + PointsPerLevel);
			}
		}
	}
	else if (Attribute == GetPointsAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("Points Given: %f"), NewValue - OldValue);
	}
}
