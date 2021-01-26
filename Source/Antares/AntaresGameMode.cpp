// Copyright Epic Games, Inc. All Rights Reserved.

#include "AntaresGameMode.h"
#include "Antares/Pawns/PawnFighter.h"

AAntaresGameMode::AAntaresGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = APawnFighter::StaticClass();
}
