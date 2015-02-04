// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "KeshUI.h"
#include "KUIInterface.h"
#include "Game/KUIGameState.h"


AKUIGameState::AKUIGameState( const class FObjectInitializer& oObjectInitializer )
	: Super( oObjectInitializer )
{

}


void AKUIGameState::OnRep_MatchState()
{
	Super::OnRep_MatchState();

	APlayerController* const aPlayerController = KUILocalPlayer();

	if ( aPlayerController == NULL )
	{
		KUIErrorUO( "Null player controller" );
		return;
	}

	AKUIInterface* const aHud = Cast<AKUIInterface>( aPlayerController->GetHUD() );

	if ( aHud == NULL )
	{
		KUIErrorUO( "Null ingame hud" );
		return;
	}

	aHud->OnMatchStateChange( MatchState );
}
