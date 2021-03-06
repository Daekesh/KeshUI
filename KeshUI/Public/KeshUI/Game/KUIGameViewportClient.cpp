// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#include "KeshUI/KeshUI.h"
#include "KeshUI/Game/KUIPlayerController.h"
#include "Runtime/Engine/Classes/Engine/Console.h"
#include "KeshUI/Game/KUIGameViewportClient.h"


UKUIGameViewportClient::UKUIGameViewportClient( const class FObjectInitializer& oObjectInitializer )
	: Super( oObjectInitializer )
{

}


bool UKUIGameViewportClient::InputChar( FViewport * Viewport, int32 ControllerId, TCHAR Character )
{
	if ( IgnoreInput() )
		return false;

	FString CharacterString;
	CharacterString += Character;

	bool bResult = ( ViewportConsole ? ViewportConsole->InputChar( ControllerId, CharacterString ) : false );

	if ( !bResult )
	{
		AKUIPlayerController* aPlayerController = Cast<AKUIPlayerController>( World->GetFirstPlayerController() );

		if ( aPlayerController != NULL )
			bResult = aPlayerController->InputCharEvent( Character );
	}

	if ( !bResult && Viewport != NULL )
	{
		if ( Viewport->IsSlateViewport() && Viewport->IsPlayInEditorViewport() )
			bResult = true;
	}

	return bResult;
}
