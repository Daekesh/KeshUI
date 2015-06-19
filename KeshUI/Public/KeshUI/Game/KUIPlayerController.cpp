// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#include "KeshUI/KeshUI.h"
#include "KeshUI/KUIInterface.h"
#include "KeshUI/Game/KUIPlayerController.h"


AKUIPlayerController::AKUIPlayerController( const class FObjectInitializer& oObjectInitializer )
	: Super( oObjectInitializer )
{
	bHasPlayEnded = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	SetActorTickEnabled( true );
}


void AKUIPlayerController::PlayerTick( float fDeltaTime )
{
	Super::PlayerTick( fDeltaTime );

	if ( bHasPlayEnded )
		return;

	ProcessInput();
}


void AKUIPlayerController::ProcessInput()
{
	FVector2D v2Delta;

	GetInputMouseDelta( v2Delta.X, v2Delta.Y );

	if ( v2Delta.X != 0 || v2Delta.Y != 0 )
		OnMouseMoved( v2Delta );
}


void AKUIPlayerController::OnMouseMoved( const FVector2D& v2Delta )
{
	if ( GEngine != NULL && !GEngine->GameViewport->Viewport->HasFocus() )
		return;

	AKUIInterface* const aHud = Cast<AKUIInterface>( GetHUD() );

	if ( aHud != NULL )
		aHud->OnMouseMove( aHud->GetCursorLocation(), aHud->GetCursorLocation() + ( FVector2D( v2Delta.X, -v2Delta.Y ) * 20.f ) ); // Some scaling
}


bool AKUIPlayerController::InputKey( FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad )
{
	//KUILog( "Key Event: %s %s", (*Key.ToString()), *FString( EventType == EInputEvent::IE_Pressed ? "\\/" : ( EventType == EInputEvent::IE_Released ? "/\\" : "++" ) ) );

	if ( bHasPlayEnded )
		return true;

	if ( InputKeyEvent( Key, EventType ) )
	{
		//KUILog( "True" );
		return true;
	}

	//KUILog( "False" );

	return Super::InputKey( Key, EventType, AmountDepressed, bGamepad );
}


bool AKUIPlayerController::InputKeyEvent( FKey eKey, EInputEvent eEventType )
{
	if ( bHasPlayEnded )
		return true;

	AKUIInterface* const aInterface = Cast<AKUIInterface>( GetHUD() );

	if ( aInterface == NULL )
		return false;

	EMouseButtons::Type eButton = AKUIInterface::GetMouseButtonFromKey( eKey );

	switch ( eEventType )
	{
		case EInputEvent::IE_Pressed:
			if ( eButton != EMouseButtons::Invalid )
				return aInterface->OnMouseButtonDown( eButton, aInterface->GetCursorLocation() );

			else
				return aInterface->OnKeyDown( eKey );

			break;

		case EInputEvent::IE_Released:
			if ( eButton != EMouseButtons::Invalid )
				return aInterface->OnMouseButtonUp( eButton, aInterface->GetCursorLocation() );

			else
				return aInterface->OnKeyUp( eKey );

			break;

		case EInputEvent::IE_Repeat:
			if ( eButton == EMouseButtons::Invalid )
				return aInterface->OnKeyRepeat( eKey );

			break;
	}

	return false;
}


bool AKUIPlayerController::InputCharEvent( TCHAR chChar )
{
	if ( bHasPlayEnded )
		return true;

	AKUIInterface* const aInterface = Cast<AKUIInterface>( GetHUD() );

	//KUILog( "Char event: %s", *FString::Chr( chChar ) );

	if ( aInterface == NULL )
		return false;

	return aInterface->OnKeyChar( chChar );
}


void AKUIPlayerController::EndPlay( const EEndPlayReason::Type eEndPlayReason )
{
	bHasPlayEnded = true;

	Super::EndPlay( eEndPlayReason );
}
