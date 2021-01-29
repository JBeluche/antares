// Fill out your copyright notice in the Description page of Project Settings.


#include "Antares/AIControllerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Antares/Pawns/PawnFighter.h"
#include "Antares/Components/MovementComponentBase.h"

AAIControllerBase::AAIControllerBase()
{
    AIFighter = Cast<APawnFighter>(GetPawn());
	if (!ensure(AIFighter != nullptr)) return;
    
   //PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);


}

void AAIControllerBase::BeginPlay() 
{
    Super::BeginPlay();

    //Get the fighter

   /* AIFighter->MovementComponent->ThrustInput();
    AIFighter->MovementComponent->MoveUpInput();
    AIFighter->MovementComponent->MoveRightInput();*/

}

void AAIControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
    //Check if not shooting
        //If not check if you have an active enemy
            //If not check if there are enemies
                //If not... well do some nice dance tricks

    if(!AIFighter->bIsShooting)
    {

    }
}
   
/*
void AAIControllerBase::ChaseNearestEnemy()
{

}
*/


float NearestDistance = BIG_NUMBER;
	APawn* NearestPawn = nullptr;

	for (FConstPawnIterator Iterator = GetWorld()->GetPawnIterator(); Iterator; ++Iterator)
	{
		APawn* OtherPawn = *Iterator;

		if (!IsValid(OtherPawn)) continue;

		const float TempDistance = FVector::Dist(GetActorLocation(), OtherPawn->GetActorLocation());
		if (NearestDistance > TempDistance)
		{
			NearestDistance = TempDistance;
			NearestPawn = OtherPawn;
		}
	}

	if(NearestPawn)
	{
		DrawDebugSphere(GetWorld(), NearestPawn->GetActorLocation(), 50.0f, 16, FColor::Red);
	}