/*
 * Copyright © 2025 Lane Hale. All rights reserved.
 *
 * Cpp_Paw.cpp
 *
 *  Created on: January 24, 2025
 *      Author: Lane Hale
 *
 * Description: This file implements the main logic for paw movement and
 *              pouncing mechanics in the game, including randomization
 *              of movement intervals and speeds.
 *
 *              The cat paw moves randomly back and forth at the top of
 *              the fish tank. The paw pounces at random intervals to try
 *              and pop the bubble to catch the fish inside it. Movements
 *              and intervals speed up as the game progresses.
 *
 *     Purpose: Global Game Jam (GGJ) 2025
 */

#include "Cpp_Paw.h"

// Set default values
ACpp_Paw::ACpp_Paw()
{
	leftRightWidth = 1950;
	// random number -975 to 975
	m = rand() % leftRightWidth;
	moveToLoc1 = static_cast<float>(m) - (leftRightWidth / 2.0f);

	LeftRightPawRotationPerTick = 0.5f;

	speedLeftRightPerTickLow = 5;
	speedLeftRightPerTickHigh = 15;
	speedLeftRightHigh = speedLeftRightPerTickHigh - speedLeftRightPerTickLow;

	secondsBeforePounceLow = 2;
	secondsBeforePounceHigh = 6;
	secondsBeforePounceHi = secondsBeforePounceHigh - secondsBeforePounceLow;
	// random number 2 to 6
	secondsBeforePounce = secondsBeforePounceLow + (rand() % secondsBeforePounceHi);

	delayPounceSecondsLow = 0.30f;
	delayPounceSecondsHigh = 0.75f;
	delayPounceSeedLow = delayPounceSecondsLow * 100;
	delayPounceSeedHigh = delayPounceSecondsHigh * 100 - delayPounceSeedLow;

	speedPounce = 1000.0f;
	maxPawAngle = 80.0f;

	increaseSpeedsAfterN_Pounces = 2;
	increaseLeftRightSpeedBy = 3;
	increasePounceSpeedBy = 250.0f;

	fastDelayTimeSeconds = 0.15f;
	fastDelayEachN_PouncesLow = 6;
	fastDelayEachN_PouncesHigh = 9;
	fastDelayEachN_PouncesHi = fastDelayEachN_PouncesHigh - fastDelayEachN_PouncesLow;
	// random number 6 to 9
	fastDelayEachN_Pounces = fastDelayEachN_PouncesLow + (rand() % fastDelayEachN_PouncesHi);

	seconds = 0.f;
	pounceCount = 0;
	pounced = false;
	angleChecked = false;
	setLeftRightPawRotation = false;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

float ACpp_Paw::getMoveToLoc1()
{
	return moveToLoc1;

}

// Called when the game starts or when spawned
void ACpp_Paw::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACpp_Paw::Tick(float DeltaTime)
{
	static int Once = 0;
	static float mvTo;

	if (Once == 0)
	{
		Once = 1;
		mvTo = getMoveToLoc1();
	}

	seconds += DeltaTime;

	mvTo = updatePawMovement(mvTo, DeltaTime);

	Super::Tick(DeltaTime);

}

float ACpp_Paw::updatePawMovement(float moveToLoc, float DeltaTime)
{
	pawLocation = GetActorLocation();
	pawRotation = GetActorRotation();

	// Is it time to pounce?
	if (seconds >= secondsBeforePounce)
	{
		// Get bubble's location and working variables one time only before pouncing
		if (pounced == false)
		{
			pounced = true;

			bubbleLoc = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

			direction = pawLocation - bubbleLoc;

			totalDistance = direction.Size();
			currentDistance = 0.f;

			direction = direction.GetSafeNormal(); // get normal direction

			pawStartLoc = pawLocation;
			movedLoc = pawLocation;

			if (pounceCount % fastDelayEachN_Pounces != 0)
			{
				// random number 30 to 75
				m = delayPounceSeedLow + (rand() % delayPounceSeedHigh);
				// make it .30 to .75
				delayBeforePounce = static_cast<float>(m) / 100.0f;
			}
			else
			{
				delayBeforePounce = fastDelayTimeSeconds;
				// random number 6 to 9
				fastDelayEachN_Pounces = fastDelayEachN_PouncesLow + 
											(rand() % fastDelayEachN_PouncesHi);
			}

			// Get vertical vector of pawLocation
			pawVertical.X = pawLocation.X;
			pawVertical.Z = bubbleLoc.Z;
			// Get direction of vertical
			direction2 = pawLocation - pawVertical;
			direction2 = direction2.GetSafeNormal();
			// Get the dot product
			dotProduct = FVector::DotProduct(direction, direction2);
			// Get angle in radians
			float angleInRadians = FMath::Acos(dotProduct);
			// Convert radians to degrees
			angleDegrees = FMath::RadiansToDegrees(angleInRadians);

			if (angleDegrees > maxPawAngle)
			{
				angleDegrees = maxPawAngle;
			}

			if (bubbleLoc.X < pawStartLoc.X)
			{
				angleDegrees *= -1;
			}

			// Compute how much to rotate paw during delay
			// Divide distance to rotate by number of ticks during delay
			rotatePawBy = abs((pawRotation.Pitch - angleDegrees) / (delayBeforePounce * 60));
		}

		// Stop moving paw and backup its location
		if (seconds < static_cast<float>(secondsBeforePounce + delayBeforePounce))
		{
			pawLocation.Z += 5.f;
			SetActorLocation(pawLocation);

			// rotate towards bubbleLoc
			if (pawRotation.Pitch < angleDegrees)
			{
				pawRotation.Pitch += rotatePawBy;
			}
			else if (pawRotation.Pitch > angleDegrees)
			{
				pawRotation.Pitch -= rotatePawBy;
			}

			SetActorRotation(pawRotation);
		}
		// Delay is over, we are pouncing now
		else if (currentDistance < totalDistance)
		{
			// move incrementally to movedLoc
			movedLoc -= direction * speedPounce * DeltaTime;

			// update paw's location
			SetActorLocation(movedLoc);

			// update distance moved so far
			currentDistance = (movedLoc - pawStartLoc).Size();
		}
		// Pounce is over, reset and try again if we missed the bubble
		else
		{
			++pounceCount;
			pawLocation = pawStartLoc;
			pawRotation = FRotator(0.f, 0.f, 0.f);
			SetActorLocation(pawLocation);
			SetActorRotation(pawRotation);
			pounced = false;
			seconds = 0.f;
			// random number 2 to 6
			secondsBeforePounce = secondsBeforePounceLow + (rand() % secondsBeforePounceHi);
			setLeftRightPawRotation = false;
			angleChecked = false;

			if (pounceCount > increaseSpeedsAfterN_Pounces)
			{
				speedPounce += increasePounceSpeedBy;
				speedLeftRightPerTickLow += increaseLeftRightSpeedBy;
				speedLeftRightPerTickHigh += increaseLeftRightSpeedBy;
			}
		}
		return moveToLoc;
	}

	// Paw moves to random location on right or left until time before pouncing is up
	if (moveToLoc > 0)
	{
		if (pawLocation.X < moveToLoc)
		{
			// random number 5 to 15
			m = speedLeftRightPerTickLow + (rand() % speedLeftRightHigh);
			pawLocation.X += static_cast<float>(m);

			if (!setLeftRightPawRotation)
			{
				pawRotation.Pitch = 20.f;
				setLeftRightPawRotation = true;
				decreasePawRotation = true;
			}
		}
		else
		{
			// random number -975 to 975
			m = rand() % leftRightWidth;
			moveToLoc = static_cast<float>(m) - (leftRightWidth / 2.0f);
		}
	}
	else // moveToLoc <= 0
	{
		if (pawLocation.X > moveToLoc)
		{
			// random number 5 to 15
			m = speedLeftRightPerTickLow + (rand() % speedLeftRightHigh);
			pawLocation.X -= static_cast<float>(m);

			if (!setLeftRightPawRotation)
			{
				pawRotation.Pitch = -20.f;
				setLeftRightPawRotation = true;
				decreasePawRotation = false;
			}
		}
		else
		{
			// random number -975 to 975
			m = rand() % leftRightWidth;
			moveToLoc = static_cast<float>(m) - (leftRightWidth / 2.0f);
		}
	}

	// Rotate paw back and forth
	if (decreasePawRotation)
	{
		if (pawRotation.Pitch > -21.f)
		{
			pawRotation.Pitch -= LeftRightPawRotationPerTick;
		}
		else
		{
			decreasePawRotation = false;
		}
	}
	else
	{
		if (pawRotation.Pitch < 21.f)
		{
			pawRotation.Pitch += LeftRightPawRotationPerTick;
		}
		else
		{
			decreasePawRotation = true;
		}
	}

	pawLocation = FVector(pawLocation.X, 0.f, pawLocation.Z);

	SetActorRotation(pawRotation);
	SetActorLocation(pawLocation);

	return moveToLoc;
}
