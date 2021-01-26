// Fill out your copyright notice in the Description page of Project Settings.

#include "Antares/Pawns/PawnFighter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Antares/Components/MovementComponentBase.h"

APawnFighter::APawnFighter()
{
	PrimaryActorTick.bCanEverTick = true;

	//Attaching all components
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm")); 
	SpringArm->SetupAttachment(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	MovementComponent = CreateDefaultSubobject<UMovementComponentBase>(TEXT("Movement Component"));
	
}

void APawnFighter::BeginPlay()
{
	Super::BeginPlay();
}

void APawnFighter::Tick(float DeltaSeconds)
{
	MovementComponent->ControlMovement(DeltaSeconds);
	Super::Tick(DeltaSeconds);
}

void APawnFighter::SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent)
{
	// Check if PlayerInputComponent is valid (not NULL)
	check(PlayerInputComponent);

	// Bind our control axis' to callback functions
	PlayerInputComponent->BindAxis("Thrust", MovementComponent, &UMovementComponentBase::ThrustInput);
	PlayerInputComponent->BindAxis("MoveUp", MovementComponent, &UMovementComponentBase::MoveUpInput);
	PlayerInputComponent->BindAxis("MoveRight", MovementComponent, &UMovementComponentBase::MoveRightInput);
}

void APawnFighter::NotifyHit(class UPrimitiveComponent *MyComp, class AActor *Other, class UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Deflect along the surface when we collide.
	FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
}




