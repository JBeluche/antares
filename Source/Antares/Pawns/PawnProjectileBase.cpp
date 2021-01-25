// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProjectileBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
APawnProjectileBase::APawnProjectileBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = BoxComponent;

	ProjectileParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Camera"));
	ProjectileParticleSystem->SetupAttachment(RootComponent);

	ChasedActor = nullptr;


}

// Called when the game starts or when spawned
void APawnProjectileBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APawnProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ChasedActor)
	{
		ChaseTarget(DeltaTime);
	}
}

// Called to bind functionality to input
void APawnProjectileBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APawnProjectileBase::ChaseTarget(float DeltaTime)
{
	//Get current and target location
	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = ChasedActor->GetActorLocation();

	//Get new Location
	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
	CurrentLocation += Speed * DeltaTime * Direction;


	//Set location
	SetActorLocation(CurrentLocation);
		UE_LOG(LogTemp, Error, TEXT("%s going to: "), *GetName(), *CurrentLocation.ToString());


}

void APawnProjectileBase::SetActorToChase(AActor* ActorToChase)
{
	ChasedActor = ActorToChase;
}


