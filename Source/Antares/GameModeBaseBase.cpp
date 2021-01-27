// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBaseBase.h"
#include "Antares/Pawns/PawnBase.h"
#include "Kismet/GameplayStatics.h"

void AGameModeBaseBase::BeginPlay()
{
    Super::BeginPlay();
}

void AGameModeBaseBase::ActorDied(AActor *DeadActor) 
{
    if(DeadActor)
    {
        PawnBase = Cast<APawnBase>(DeadActor);

        PawnBase->HandleDestruction();
    }

}