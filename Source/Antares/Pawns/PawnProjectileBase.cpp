// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProjectileBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


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

	Speed = 20.0f;
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
	FVector RotationLocation = GetActorLocation();
	FVector TargetLocation = ChasedActor->GetActorLocation();

	//Get new Location
	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
	CurrentLocation += Speed * DeltaTime * Direction;
	RotationLocation += 10000.0f * DeltaTime * Direction;

	//Get New Rotation
	FRotator CurrentRotation = UKismetMathLibrary::FindLookAtRotation(RotationLocation, TargetLocation); 

	//Set location and rotation
	SetActorRotation(CurrentRotation);
	SetActorLocation(CurrentLocation);


}

void APawnProjectileBase::SetActorToChase(AActor* ActorToChase, float ProjectileSpeed)
{
	ChasedActor = ActorToChase;
	Speed = ProjectileSpeed;
}

void APawnProjectileBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* MyOwner = GetOwner();
	if(OtherActor == MyOwner)
	{
		return;
	}
	else if(OtherActor && OtherActor != this && OtherActor !=  MyOwner)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, DamageType);
		Destroy();
	}
}

