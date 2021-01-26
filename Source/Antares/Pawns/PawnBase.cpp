// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnBase.h"
#include "PawnProjectileBase.h"
#include "Components/CapsuleComponent.h"
#include "Antares/Components/FireComponentBase.h"

// Sets default values
APawnBase::APawnBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Casule Component"));
	RootComponent = CapsuleComp;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(BaseMesh);
	AttackTriggerBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Attack Zone"));
	AttackTriggerBox->SetupAttachment(RootComponent);
	FireComponent = CreateDefaultSubobject<UFireComponentBase>(TEXT("Fire Component"));
}

// Called when the game starts or when spawned
void APawnBase::BeginPlay()
{
	AttackTriggerBox->ToggleVisibility(false);
	AttackTriggerBox->OnComponentBeginOverlap.AddDynamic(FireComponent, &UFireComponentBase::AttackTriggerBoxOverlapBegin);
	AttackTriggerBox->OnComponentEndOverlap.AddDynamic(FireComponent, &UFireComponentBase::AttackTriggerBoxOverlapEnd);

	Super::BeginPlay();
}

// Called every frame
void APawnBase::Tick(float DeltaTime)
{
	FireComponent->CheckFireCondition();
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APawnBase::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APawnBase::FireAt(AActor *Target)
{
	UE_LOG(LogTemp, Error, TEXT("Firing at %s"), *Target->GetName());

	if (Target)
	{
		if (PawnProjectileBase)
		{

			FVector Location = ProjectileSpawnPoint->GetComponentLocation();
			FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
			APawnProjectileBase *TempProjectile = GetWorld()->SpawnActor<APawnProjectileBase>(PawnProjectileBase, Location, Rotation);
			TempProjectile->SetOwner(this);
			UE_LOG(LogTemp, Error, TEXT("Created: %s"), *TempProjectile->GetName());

			TempProjectile->SetActorToChase(Target);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No projectile is set!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No enemy can be found!"));
	}
}