/*
 * Copyright © 2025 Lane Hale. All rights reserved.
 *
 * Cpp_Paw.h
 *
 *  Created on: January 24, 2025
 *      Author: Lane Hale
 *
 * Description: This header file defines the ACpp_Paw class, which
 *              includes variables and function declarations for
 *              controlling paw behavior and movement in the game.
 *        Note: UPROPERTY variables can be altered in Unreal Engine.
 *              Set the global Tick value to 30 FPS in Unreal Engine.
 *
 *     Purpose: Global Game Jam (GGJ) 2025
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cpp_Paw.generated.h"

UCLASS()
class GGJ2025_HALE_API ACpp_Paw : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACpp_Paw();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float getMoveToLoc1();

	float updatePawMovement(float moveToLoc, float DeltaTime);

private:
	int m;
	int pounceCount;

	UPROPERTY(EditDefaultsOnly)
	int leftRightWidth;

	UPROPERTY(EditDefaultsOnly)
	int speedLeftRightPerTickLow;
	UPROPERTY(EditDefaultsOnly)
	int speedLeftRightPerTickHigh;
	int speedLeftRightHigh;

	UPROPERTY(EditDefaultsOnly)
	float LeftRightPawRotationPerTick;

	UPROPERTY(EditDefaultsOnly)
	int secondsBeforePounceLow;

	UPROPERTY(EditDefaultsOnly)
	int secondsBeforePounceHigh;
	int secondsBeforePounceHi;

	int secondsBeforePounce;

	UPROPERTY(EditDefaultsOnly)
	float delayPounceSecondsLow;
	UPROPERTY(EditDefaultsOnly)
	float delayPounceSecondsHigh;

	int delayPounceSeedLow;
	int delayPounceSeedHigh;

	float delayBeforePounce;

	UPROPERTY(EditDefaultsOnly)
	float speedPounce;

	UPROPERTY(EditDefaultsOnly)
	float maxPawAngle;

	UPROPERTY(EditDefaultsOnly, Category = "Cpp Paw Difficulty")
	int increaseSpeedsAfterN_Pounces;

	UPROPERTY(EditDefaultsOnly, Category = "Cpp Paw Difficulty")
	float increasePounceSpeedBy;

	UPROPERTY(EditDefaultsOnly, Category = "Cpp Paw Difficulty")
	int increaseLeftRightSpeedBy;

	UPROPERTY(EditDefaultsOnly, Category="Cpp Paw Difficulty")
	int fastDelayEachN_PouncesLow;
	UPROPERTY(EditDefaultsOnly, Category = "Cpp Paw Difficulty")
	int fastDelayEachN_PouncesHigh;
	int fastDelayEachN_PouncesHi;
	int fastDelayEachN_Pounces;

	UPROPERTY(EditDefaultsOnly, Category = "Cpp Paw Difficulty")
	float fastDelayTimeSeconds;

	float moveToLoc1;
	float totalDistance;
	float currentDistance;
	float seconds;
	float dotProduct;
	float angleDegrees;
	float rotatePawBy;

	bool setLeftRightPawRotation;
	bool decreasePawRotation;
	bool pounced;
	bool angleChecked;

	FVector bubbleLoc;
	FVector pawLocation;
	FVector pawStartLoc;
	FVector pawVertical;
	FVector movedLoc;
	FVector direction;
	FVector direction2;

	FRotator pawRotation;
};
