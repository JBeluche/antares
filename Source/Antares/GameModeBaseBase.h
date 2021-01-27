// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeBaseBase.generated.h"

class APawnBase;

UCLASS()
class ANTARES_API AGameModeBaseBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void ActorDied(AActor* DeadActor);
	

private:
	APawnBase* PawnBase;
	
protected:
	virtual void BeginPlay() override;

};
