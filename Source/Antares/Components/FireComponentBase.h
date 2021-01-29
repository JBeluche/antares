// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FireComponentBase.generated.h"

class APawnBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANTARES_API UFireComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFireComponentBase();

protected:
	// Functions
	virtual void BeginPlay() override;
	void ToggleTarget(AActor* NewTarget);
	void RemoveTarget();
	void RemoveFromWaitingList(AActor* ActortoRemove);

	// Variables
	TArray<AActor*> EnemyWaitingList;
	bool bCanFire;
	AActor* TargetToShoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire", meta = (AllowPrivateAccess = "true"))
	float FireRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire", meta = (AllowPrivateAccess = "true"))
	float ProjectileSpeed;

	FTimerHandle FireRateTimerHandle;
	void Fire();

	APawnBase* PawnBase;


public:	

	UFUNCTION()
	void AttackTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void AttackTriggerBoxOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CheckFireCondition();
	float GetProjectileSpeed();

};
