// Fill out your copyright notice in the Description page of Project Settings.

#include "Antares/Pawns/PawnFighter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APawnFighter::APawnFighter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	AttackTriggerBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Attack Zone"));
	AttackTriggerBox->SetupAttachment(RootComponent);

	// Set handling parameters
	Acceleration = 500.f;
	TurnSpeed = 200.f;
	MaxSpeed = 500.f;
	MinSpeed = 500.f;
	CurrentForwardSpeed = 500.f;
}

void APawnFighter::BeginPlay()
{
	AttackTriggerBox->ToggleVisibility(false);
	AttackTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APawnFighter::AttackTriggerBoxOverlapBegin);
	AttackTriggerBox->OnComponentEndOverlap.AddDynamic(this, &APawnFighter::AttackTriggerBoxOverlapEnd);

	Super::BeginPlay();
}

void APawnFighter::Tick(float DeltaSeconds)
{
	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaSeconds, 0.f, 0.f);

	// Move plan forwards (with sweep so we stop when we collide with things)
	AddActorLocalOffset(LocalMove, true);

	// Calculate change in rotation this frame
	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaSeconds;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaSeconds;
	DeltaRotation.Roll = CurrentRollSpeed * DeltaSeconds;

	// Rotate plane
	AddActorLocalRotation(DeltaRotation);

	// Call any parent class Tick implementation
	Super::Tick(DeltaSeconds);

	//Check if you not shooting at someone.
	if(!AttackingEnnemy)
	{
		for(int i = 0; i < WaitingToFireAt.Num(); i++)
		{
			//Then check if someone is in waiting list.
			if(Cast<AActor>(WaitingToFireAt[i]))
			{
				//Shoot first one in waiting list
				ToggleTarget(WaitingToFireAt[i]);
			}
		}
	}
	else
	{
		FireAtTarget();
	}

}

void APawnFighter::SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent)
{
	// Check if PlayerInputComponent is valid (not NULL)
	check(PlayerInputComponent);

	// Bind our control axis' to callback functions
	PlayerInputComponent->BindAxis("Thrust", this, &APawnFighter::ThrustInput);
	PlayerInputComponent->BindAxis("MoveUp", this, &APawnFighter::MoveUpInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &APawnFighter::MoveRightInput);
}

void APawnFighter::ThrustInput(float Val)
{
	// Is there any input?
	bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
	// If input is not held down, reduce speed
	float CurrentAcc = bHasInput ? (Val * Acceleration) : (-0.5f * Acceleration);
	// Calculate new speed
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
	// Clamp between MinSpeed and MaxSpeed
	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}

void APawnFighter::MoveUpInput(float Val)
{
	// Target pitch speed is based in input
	float TargetPitchSpeed = (Val * TurnSpeed * -1.f);

	// When steering, we decrease pitch slightly
	TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

	// Smoothly interpolate to target pitch speed
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void APawnFighter::MoveRightInput(float Val)
{
	// Target yaw speed is based on input
	float TargetYawSpeed = (Val * TurnSpeed);

	// Smoothly interpolate to target yaw speed
	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

	// Is there any left/right input?
	const bool bIsTurning = FMath::Abs(Val) > 0.2f;

	// If turning, yaw value is used to influence roll
	// If not turning, roll to reverse current roll value.
	float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (GetActorRotation().Roll * -2.f);

	// Smoothly interpolate roll speed
	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void APawnFighter::NotifyHit(class UPrimitiveComponent *MyComp, class AActor *Other, class UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Deflect along the surface when we collide.
	FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
}

void APawnFighter::AttackTriggerBoxOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (!AttackingEnnemy)
	{
		ToggleTarget(OtherActor);
	}
	else
	{
		WaitingToFireAt.Add(OtherActor);
	}
}

void APawnFighter::AttackTriggerBoxOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AttackingEnnemy == OtherActor)
	{
		StopFiring(AttackingEnnemy);
	}
	else
	{
		RemoveFromWaitingList(OtherActor);
	}
}

void APawnFighter::ToggleTarget(AActor* ActorToAttack)
{
	AttackingEnnemy = ActorToAttack;
	RemoveFromWaitingList(ActorToAttack);
}

void APawnFighter::StopFiring(AActor* ActorToAttack)
{
	AttackingEnnemy = nullptr;
}

void APawnFighter::RemoveFromWaitingList(AActor* ActortoRemove)
{
	for (int i = 0; i < WaitingToFireAt.Num(); i++)
	{
		if(ActortoRemove == Cast<AActor>(WaitingToFireAt[i]))
		{
			WaitingToFireAt.Remove(ActortoRemove);
		}
	}
}

void APawnFighter::FireAtTarget()
{
	
}