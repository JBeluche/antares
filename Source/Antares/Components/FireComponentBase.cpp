// Fill out your copyright notice in the Description page of Project Settings.


#include "Antares/Components/FireComponentBase.h"
#include "Antares/Pawns/PawnBase.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UFireComponentBase::UFireComponentBase()
{
	PrimaryComponentTick.bCanEverTick = false;
	FireRate = 1.0f;
	ProjectileSpeed = 20.f;
}

void UFireComponentBase::BeginPlay()
{
	Super::BeginPlay();
	bCanFire = true;
}

void UFireComponentBase::CheckFireCondition()
{
	//Check if you are not shooting at someone.
	if(!TargetToShoot)
	{
		for(int i = 0; i < EnemyWaitingList.Num(); i++)
		{
			//Then check if someone is in waiting list.
			if(Cast<AActor>(EnemyWaitingList[i]))
			{
				//Shoot first one in waiting list
				ToggleTarget(EnemyWaitingList[i]);
			}
		}
	}
	else if(TargetToShoot)
	{

		if(GetWorld()->GetTimerManager().IsTimerActive(FireRateTimerHandle))
		{
		}
		else 
		{
			GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &UFireComponentBase::Fire, FireRate, true);
		}
	}

}

void UFireComponentBase::AttackTriggerBoxOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	//If there was no target, set a target. Else add to waiting list
	if (!TargetToShoot)
	{
		ToggleTarget(OtherActor);
	}
	else
	{
		EnemyWaitingList.Add(OtherActor);
	}
}

void UFireComponentBase::AttackTriggerBoxOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//If the leaving actor was target, remove him. If he was in the list. Remove from list
	if (TargetToShoot == OtherActor)
	{
		RemoveTarget();
	}
	else
	{
		RemoveFromWaitingList(OtherActor);
	}
}

void UFireComponentBase::ToggleTarget(AActor* NewTarget)
{
	TargetToShoot = NewTarget;
	RemoveFromWaitingList(NewTarget);
}

void UFireComponentBase::RemoveTarget()
{
	TargetToShoot = nullptr;
}

void UFireComponentBase::RemoveFromWaitingList(AActor* ActortoRemove)
{
	for (int i = 0; i < EnemyWaitingList.Num(); i++)
	{
		if(ActortoRemove == Cast<AActor>(EnemyWaitingList[i]))
		{
			EnemyWaitingList.Remove(ActortoRemove);
		}
	}
}

void UFireComponentBase::Fire()
{
	Cast<APawnBase>(GetOwner())->FireAt(TargetToShoot);
}

float UFireComponentBase::GetProjectileSpeed()
{
	return ProjectileSpeed;
}

