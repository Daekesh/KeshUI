// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KeshUI/KUIMacros.h"
#include "KUIGameState.generated.h"

class UKUIResearchInfo;
class UKUIBuildingInfo;
class UKUIProjectileInfo;
class AKUIPlayerState;
class AKUITeamInfo;

#define KUI_MATCH_START_TIME_NOT_STARTED -1.f

/**
 * The base game state class for KeshUI.
 */
UCLASS(ClassGroup="KeshUI|Game", Blueprintable, BlueprintType, NotPlaceable)
class KESHUI_API AKUIGameState : public AGameState
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( AKUIGameState )

	AKUIGameState( const class FObjectInitializer& oObjectInitializer );

protected:

	virtual void OnRep_MatchState() override;

};
