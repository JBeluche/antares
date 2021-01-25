// Copyright Epic Games, Inc. All Rights Reserved.

#include "AntaresGameMode.h"
#include "AntaresPawn.h"

AAntaresGameMode::AAntaresGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = AAntaresPawn::StaticClass();
}
