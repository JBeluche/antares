// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponentBase.generated.h"

class AGameModeBaseBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANTARES_API UHealthComponentBase : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void TakeDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

private:

	UHealthComponentBase();

	UPROPERTY(EditAnywhere)
	float DefaultHealth = 100.0f;

	float Health = 0.0f;

	AGameModeBaseBase* GameModeRef;
		
};
