// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KUIMacros.h"
#include "KUIGameState.generated.h"

class UKUIResearchInfo;
class UKUIBuildingInfo;
class UKUIProjectileInfo;
class AKUIPlayerState;
class AKUITeamInfo;

#define KUI_MATCH_START_TIME_NOT_STARTED -1.f

/**
 * The base game state class for Barrage.
 */
UCLASS(ClassGroup="KeshUI | Game", Blueprintable, BlueprintType, NotPlaceable)
class KESHUI_API AKUIGameState : public AGameState
{
	GENERATED_UCLASS_BODY()
	KUI_CLASS_HEADER( AKUIGameState )
	
protected:

	virtual void OnRep_MatchState() override;

};
