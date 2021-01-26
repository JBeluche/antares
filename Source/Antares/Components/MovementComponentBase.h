// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "MovementComponentBase.generated.h"

/**
 * 
 */
UCLASS()
class ANTARES_API UMovementComponentBase : public UMovementComponent
{
	GENERATED_BODY()
public:
	void ControlMovement(float DeltaSeconds);

	UMovementComponentBase();

	/** Bound to the thrust axis */
	void ThrustInput(float Val);
	
	/** Bound to the vertical axis */
	void MoveUpInput(float Val);

	/** Bound to the horizontal axis */
	void MoveRightInput(float Val);
	
private:
	/** Movement */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Acceleration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float TurnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MinSpeed;

	float CurrentForwardSpeed;
	float CurrentYawSpeed;
	float CurrentPitchSpeed;
	float CurrentRollSpeed;



};
