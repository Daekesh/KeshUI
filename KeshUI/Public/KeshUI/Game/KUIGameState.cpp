// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#include "KeshUI/KeshUI.h"
#include "KeshUI/KUIInterface.h"
#include "KeshUI/Game/KUIGameState.h"


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
