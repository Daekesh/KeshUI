// Copyright 2014-2015 Matt Chapman. All Rights Reserved.


#include "KeshUI.h"
#include "KUIInterfaceElement.h"
#include "KUIInterfaceContainer.h"
#include "Container/KUIRootContainer.h"
#include "Container/KUITooltipContainer.h"
#include "Container/KUICursorContainer.h"
#include "Component/KUIBoxInterfaceComponent.h"
#include "KUICancellable.h"
#include "KUIAssetLibrary.h"
#include "KUIInterface.h"


TWeakObjectPtr<AKUIInterface> aLatestInstance;


AKUIInterface::AKUIInterface( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	KUI_UO_INIT_DEBUG()

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	SetActorTickEnabled( true );

	bVisible = true;
	bCursorVisible = true;
	v2ScreenResolution = FVector2D::ZeroVector;
	v2CursorLocation = FVector2D( KUI_INTERFACE_FIRST_CURSOR_UPDATE, 0.f );
	v2CursorVector = FVector2D::ZeroVector;
	arCancellables.SetNum( 0 );
	ctFocused = NULL;
	arAssetLibraries.SetNum( 0 );

	ctRootContainers.SetNum( 4 );

	KUICreateDefaultSubobjectAssign( ctRootContainers[ EKUIInterfaceRoot::R_Root ], UKUIRootContainer, "Root Container" );
	ctRootContainers[ EKUIInterfaceRoot::R_Root ]->SetInterface( this );

	KUICreateDefaultSubobjectAssign( ctRootContainers[ EKUIInterfaceRoot::R_Overlay ], UKUIRootContainer, "Overlay Container" );
	ctRootContainers[ EKUIInterfaceRoot::R_Overlay ]->SetInterface( this );

	KUICreateDefaultSubobjectAssign( ctRootContainers[ EKUIInterfaceRoot::R_Tooltip ], UKUITooltipContainer, "Tooltip Container" );
	ctRootContainers[ EKUIInterfaceRoot::R_Tooltip ]->SetInterface( this );

	KUICreateDefaultSubobjectAssign( ctRootContainers[ EKUIInterfaceRoot::R_Cursor ], UKUICursorContainer, "Cursor Container" );
	ctRootContainers[ EKUIInterfaceRoot::R_Cursor ]->SetInterface( this );

	arMouseButtonDownLocations.SetNum( 3 );
	arMouseButtonDownLocations[ EMouseButtons::Left ] = FVector2D::ZeroVector;
	arMouseButtonDownLocations[ EMouseButtons::Right ] = FVector2D::ZeroVector;
	arMouseButtonDownLocations[ EMouseButtons::Middle ] = FVector2D::ZeroVector;

	aLatestInstance = this;
}

AKUIInterface* AKUIInterface::GetLatestInstance()
{
	return aLatestInstance.Get();
}


APlayerController* AKUIInterface::GetPlayerController() const
{
	return GetOwningPlayerController();
}


UKUIInterfaceContainer* AKUIInterface::GetRootContainer( uint8 iContainerId ) const
{
	if ( ctRootContainers.IsValidIndex( iContainerId ) )
		return ctRootContainers[ iContainerId ];

	return NULL;
}


bool AKUIInterface::IsVisible() const
{
	return bVisible;
}


void AKUIInterface::SetVisible( bool bVisible )
{
	if ( this->bVisible == bVisible )
		return;

	this->bVisible = bVisible;

	KUIBroadcastEvent( FKUIInterfaceContainerVisibilityEvent, EKUIInterfaceContainerEventList::E_VisibilityChange, bVisible );
}


bool AKUIInterface::IsCursorVisible() const
{
	return bCursorVisible;
}


void AKUIInterface::SetCursorVisible( bool bVisible )
{
	if ( bCursorVisible == bVisible )
		return;

	bCursorVisible = bVisible;

	KUIBroadcastEventObj( ctRootContainers[ EKUIInterfaceRoot::R_Cursor ], FKUIInterfaceContainerVisibilityEvent, EKUIInterfaceContainerEventList::E_VisibilityChange, bVisible );
}


FVector2D AKUIInterface::GetScreenResolution() const
{
	return AKUIInterface::v2ScreenResolution;
}


FVector2D AKUIInterface::GetCursorLocation() const
{
	return AKUIInterface::v2CursorLocation;
}


FVector2D AKUIInterface::GetCursorVector() const
{
	return AKUIInterface::v2CursorVector;
}


FVector2D AKUIInterface::GetMouseButtonDownLocation( EMouseButtons::Type eButton ) const
{
	if ( !AKUIInterface::arMouseButtonDownLocations.IsValidIndex( eButton ) )
		return FVector2D::ZeroVector;

	return AKUIInterface::arMouseButtonDownLocations[ eButton ];
}


FVector2D AKUIInterface::GetMouseButtonDownLocationBP( uint8 eButton ) const
{
	return AKUIInterface::GetMouseButtonDownLocation( static_cast< EMouseButtons::Type>( eButton ) );
}


bool AKUIInterface::IsInputDown( FKey eKey ) const
{
	if ( PlayerOwner == NULL )
	{
		KUIErrorUO( "Null player owner" );
		return false;
	}

	return PlayerOwner->IsInputKeyDown( eKey );
}

	
float AKUIInterface::GetInputDownTime( FKey eKey ) const
{
	if ( PlayerOwner == NULL )
	{
		KUIErrorUO( "Null player owner" );
		return 0.f;
	}

	return PlayerOwner->GetInputKeyTimeDown( eKey );
}


EMouseButtons::Type AKUIInterface::GetMouseButtonFromKey( FKey eKey )
{
	if ( eKey == EKeys::LeftMouseButton )
		return EMouseButtons::Left;
	
	if ( eKey == EKeys::RightMouseButton )
		return EMouseButtons::Right;

	if ( eKey == EKeys::MiddleMouseButton )
		return EMouseButtons::Middle;

	if ( eKey == EKeys::ThumbMouseButton )
		return EMouseButtons::Thumb01;

	if ( eKey == EKeys::ThumbMouseButton2 )
		return EMouseButtons::Thumb02;

	return EMouseButtons::Invalid;
}


UKUIInterfaceElement* AKUIInterface::GetFocus() const
{
	return ctFocused.Get();
}


void AKUIInterface::SetFocus( UKUIInterfaceContainer* ctNewFocus )
{
	if ( ctFocused.Get() == ctNewFocus )
		return;

	UKUIInterfaceContainer* const ctOldFocus = ctFocused.Get();

	if ( ctOldFocus != NULL )
	{
		FKUIInterfaceEvent stEventInfo( EKUIInterfaceContainerEventList::E_Blur );
		ctOldFocus->SendEvent( stEventInfo );
	}

	this->ctFocused = ctNewFocus;

	if ( ctNewFocus != NULL )
	{
		FKUIInterfaceEvent stEventInfo( EKUIInterfaceContainerEventList::E_Focus );
		ctNewFocus->SendEvent( stEventInfo );
	}

	FKUIInterfaceContainerElementEvent stEventInfo( EKUIInterfaceContainerEventList::E_FocusChange, ctNewFocus );
	BroadcastEvent( stEventInfo );

	if ( !IsTemplate() )
		OnFocusChangeBP( ctOldFocus, ctNewFocus );
}


void AKUIInterface::Tick( float fDeltaTime )
{
	FKUIInterfaceContainerTickEvent stEventInfo( EKUIInterfaceContainerEventList::E_Tick, fDeltaTime );
	BroadcastEvent( stEventInfo );
	
	if ( !IsTemplate() )
		OnTickBP( fDeltaTime );
}


void AKUIInterface::DrawHUD()
{
	Super::DrawHUD();

	Render( Canvas );
}


void AKUIInterface::Render( UCanvas* oCanvas )
{
	if ( !bShowHUD )
		return;

	if ( GEngine != NULL && GEngine->GameViewport != NULL )
	{	
		// Update the screen res.
		FVector2D v2ScreenResolution = FVector2D::ZeroVector;
		GEngine->GameViewport->GetViewportSize( v2ScreenResolution );

		if ( !this->v2ScreenResolution.Equals( v2ScreenResolution, 0.5f ) )
			OnScreenResolutionChange( this->v2ScreenResolution, v2ScreenResolution );

		this->v2ScreenResolution = v2ScreenResolution;

		if ( this->v2CursorLocation.X == KUI_INTERFACE_FIRST_CURSOR_UPDATE )
			this->v2CursorLocation = FVector2D( floor( this->v2ScreenResolution.X / 2.f ), floor( this->v2ScreenResolution.Y / 2.f ) );
	}
	
	if ( !IsTemplate() )
		OnRenderBP( Canvas );

#if KUI_INTERFACE_MOUSEOVER_DEBUG
	cmDebugMouseOver = NULL;
	v2DebugMouseOverLocation = FVector2D::ZeroVector;
	v2DebugMouseOverSize = FVector2D::ZeroVector;
#endif // KUI_INTERFACE_MOUSEOVER_DEBUG

	if ( IsVisible() )
	{
		for ( int32 i = 0; i < ctRootContainers.Num(); ++i )
		{
			if ( ctRootContainers[ i ] == NULL )
				continue;

			if ( i == EKUIInterfaceRoot::R_Cursor )
				continue;

#if KUI_INTERFACE_MOUSEOVER_DEBUG
			bDebugMouseOver = arDebugMouseOver[ i ];
#endif // KUI_INTERFACE_MOUSEOVER_DEBUG
			ctRootContainers[ i ]->Render( this, oCanvas, FVector2D::ZeroVector );
		}
	}

	if ( IsCursorVisible() )
	{
#if KUI_INTERFACE_MOUSEOVER_DEBUG
		bDebugMouseOver = arDebugMouseOver[ EKUIInterfaceRoot::R_Cursor ];
#endif // KUI_INTERFACE_MOUSEOVER_DEBUG
		ctRootContainers[ EKUIInterfaceRoot::R_Cursor ]->Render( this, oCanvas, v2CursorLocation );
	}

#if KUI_INTERFACE_MOUSEOVER_DEBUG
	bDebugMouseOver = false;

	if ( cmDebugMouseOver.IsValid() )
	{
		cmDebugMouseOverTestBox->SetLocationStruct( v2DebugMouseOverLocation + FVector2D( 1.f, 1.f ) );
		cmDebugMouseOverTestBox->SetSizeStruct( v2DebugMouseOverSize - FVector2D( 1.f, 1.f ) );
		cmDebugMouseOverTestBox->Render( this, oCanvas, FVector2D::ZeroVector, NULL );
	}

	if ( cmDebugMouseOver.Get() != cmDebugMouseOverLastTick.Get() )
	{
		KUILogUO( 
			"UI Mouse Over: %s: %s -> %s: %s (%f,%f) -> (%f,%f)", 
			*( cmDebugMouseOverLastTick.Get() ? ( cmDebugMouseOverLastTick.Get()->GetOuter() ? cmDebugMouseOverLastTick.Get()->GetOuter()->GetName() : "NULL" ) : "NULL" ),
			*( cmDebugMouseOverLastTick.Get() ? cmDebugMouseOverLastTick->GetName() : FString( "NULL" ) ), 
			*( cmDebugMouseOver.Get() ? ( cmDebugMouseOver.Get()->GetOuter() ? cmDebugMouseOver.Get()->GetOuter()->GetName() : "NULL" ) : "NULL" ),
			*( cmDebugMouseOver.Get() ? cmDebugMouseOver->GetName() : FString( "NULL" ) ), 
			ExpandV2( v2DebugMouseOverLocation ),
			ExpandV2( v2DebugMouseOverSize )
		);
	}

	cmDebugMouseOverLastTick = cmDebugMouseOver.Get();
#endif // KUI_INTERFACE_MOUSEOVER_DEBUG
}


void AKUIInterface::AddElement( uint8 iContainer, UKUIInterfaceElement* oElement )
{
	ctRootContainers[ iContainer ]->AddChild( oElement );

	if ( !IsTemplate() )
		OnElementAddedBP( iContainer, oElement );
}


void AKUIInterface::AddCancellable( UObject* oCancellable )
{
	if ( oCancellable == NULL )
	{
		KUIErrorUO( "Trying to add a null cancellable" );
		return;
	}

	IKUICancellable* const iCancellable = Cast<IKUICancellable>( oCancellable );

	if ( iCancellable == NULL )
	{
		KUIErrorUO( "Cancellable is not a cancellable" );
		return;
	}

	for ( int32 i = 0; i < arCancellables.Num(); ++i )
	{
		if ( arCancellables[ i ].Get() == oCancellable )
			return;

		if ( arCancellables[ i ].Get() == NULL )
		{
			arCancellables[ i ] = TWeakObjectPtr<UObject>( oCancellable );
			
			if ( !IsTemplate() )
				OnCancellableAddedBP( oCancellable );

			return;
		}
	}

	TWeakObjectPtr<UObject> ptrCancellable = TWeakObjectPtr<UObject>( oCancellable );

	arCancellables.Add( ptrCancellable );

	if ( !IsTemplate() )
		OnCancellableAddedBP( oCancellable );
}


void AKUIInterface::RemoveCancellable( UObject* oCancellable )
{
	if ( oCancellable == NULL )
	{
		KUIErrorUO( "Trying to remove a null cancellable" );
		return;
	}

	IKUICancellable* const iCancellable = Cast<IKUICancellable>( oCancellable );

	if ( iCancellable == NULL )
	{
		KUIErrorUO( "Cancellable is not a cancellable" );
		return;
	}

	int32 iIndex = INDEX_NONE;

	for ( int32 i = 0; i < arCancellables.Num(); ++i )
	{
		if ( arCancellables[ i ].Get() != oCancellable )
			continue;

		iIndex = i;
		break;
	}

	if ( iIndex == INDEX_NONE )
		return;

	for ( int32 i = iIndex + 1; i < arCancellables.Num(); ++i )
		arCancellables[ i - 1 ] = arCancellables[ i ];

	if ( !IsTemplate() )
		OnCancellableRemovedBP( oCancellable );
}


bool AKUIInterface::Cancel()
{
	IKUICancellable* oCancellable = NULL;
	
	for ( int32 i = 0; i < arCancellables.Num(); ++i )
	{
		if ( arCancellables[ i ].Get() == NULL )
		{
			arCancellables.SetNum( i );
			break;
		}

		oCancellable = Cast<IKUICancellable>( arCancellables[ i ].Get() );
	}


	if ( oCancellable != NULL )
	{
		OnCancel( Cast<UObject>( oCancellable ) );

		if ( !IsTemplate() )
			OnCancelBP( Cast<UObject>( oCancellable ) );

		oCancellable->Cancel();
		return true;
	}

	return false;
}


void AKUIInterface::OnCancel( UObject* oCancellable )
{

}


UObject** AKUIInterface::GetAsset( const FName& nName ) const
{
	if ( IsTemplate() )
		return NULL;

	UObject** oAsset = NULL;

	for ( int32 i = 0; i < arAssetLibraries.Num(); ++i )
	{
		if ( arAssetLibraries[ i ] == NULL )
			continue;

		oAsset = arAssetLibraries[ i ]->GetAsset( nName );

		if ( oAsset != NULL )
			return oAsset;
	}

	KUIErrorUO( "Failed to load asset: %s", *( nName.ToString() ) );

	return NULL;
}


void AKUIInterface::BroadcastEvent( FKUIInterfaceEvent& stEventInfo, bool bTopDown, bool bIncludeCursor )
{
	if ( !bTopDown )
	{
		for ( int32 i = 0; i < ctRootContainers.Num(); ++i )
		{
			if ( ctRootContainers[ i ] == NULL )
				continue;

			if ( !bIncludeCursor && i == EKUIInterfaceRoot::R_Cursor )
				continue;

			ctRootContainers[ i ]->BroadcastEvent( stEventInfo, bTopDown );
		}
	}

	else
	{
		for ( int32 i = ctRootContainers.Num() - 1; i >= 0; --i )
		{
			if ( ctRootContainers[ i ] == NULL )
				continue;

			if ( !bIncludeCursor && i == EKUIInterfaceRoot::R_Cursor )
				continue;

			ctRootContainers[ i ]->BroadcastEvent( stEventInfo, bTopDown );
		}
	}
}


void AKUIInterface::OnMouseMove( const FVector2D& v2OldLocation, const FVector2D& v2NewLocation )
{
	if ( this->v2CursorLocation.X == KUI_INTERFACE_FIRST_CURSOR_UPDATE )
		return;

	const FVector2D v2FixedNewLocation = FVector2D(
		clamp( v2NewLocation.X, 0.f, v2ScreenResolution.X - 1.f ),
		clamp( v2NewLocation.Y, 0.f, v2ScreenResolution.Y - 1.f )
	);

	v2CursorVector = v2NewLocation - v2OldLocation;

	if ( v2CursorVector.IsNearlyZero( 0.1f ) )
		return;

	this->v2CursorLocation = v2FixedNewLocation;

	KUIBroadcastSubEvent( FKUIInterfaceContainerMouseLocationEvent, EKUIInterfaceContainerEventList::E_MouseMove, v2OldLocation, v2FixedNewLocation );

	if ( !IsTemplate() )
		OnMouseMoveBP( v2OldLocation, v2FixedNewLocation );
}


bool AKUIInterface::OnMouseButtonDown( EMouseButtons::Type eButton, const FVector2D& v2Location )
{
	if ( arMouseButtonDownLocations.IsValidIndex( eButton ) )
		arMouseButtonDownLocations[ eButton ] = v2Location;

	FKUIInterfaceContainerMouseButtonEvent stEventInfo( EKUIInterfaceContainerEventList::E_MouseButtonDown, false, eButton, v2Location );
	BroadcastEvent( stEventInfo, true );

	if ( !IsTemplate() )
		OnMouseButtonDownBP( eButton, v2Location );

	return stEventInfo.bHandled;
}
	

bool AKUIInterface::OnMouseButtonUp( EMouseButtons::Type eButton, const FVector2D& v2Location )
{
	FKUIInterfaceContainerMouseButtonEvent stEventInfo( EKUIInterfaceContainerEventList::E_MouseButtonUp, false, eButton, v2Location );
	BroadcastEvent( stEventInfo, true );

	if ( !IsTemplate() )
		OnMouseButtonUpBP( eButton, v2Location );

	if ( !stEventInfo.bHandled )
	{
		KUILogDebug( "Mouse up not handled: %d", ( uint8 ) eButton );
		SetFocus( NULL );
	}

	return stEventInfo.bHandled;
}


bool AKUIInterface::OnKeyDown( FKey eKey )
{
	FKUIInterfaceContainerKeyEvent stEventInfo( EKUIInterfaceContainerEventList::E_KeyDown, false, eKey );

	if ( IsConsumingKeyEvents() )
		ctFocused->SendEvent( stEventInfo );

	else
	{
		BroadcastEvent( stEventInfo, true );

		if ( !IsTemplate() )
			OnKeyDownBP( eKey );
	}

	return stEventInfo.bHandled;
}


bool AKUIInterface::OnKeyUp( FKey eKey )
{
	FKUIInterfaceContainerKeyEvent stEventInfo( EKUIInterfaceContainerEventList::E_KeyUp, false, eKey );

	if ( IsConsumingKeyEvents() )
		ctFocused->SendEvent( stEventInfo );

	else
	{
		BroadcastEvent( stEventInfo, true );

		if ( !IsTemplate() )
			OnKeyUpBP( eKey );
	}

	return stEventInfo.bHandled;
}


bool AKUIInterface::OnKeyRepeat( FKey eKey )
{
	FKUIInterfaceContainerKeyEvent stEventInfo( EKUIInterfaceContainerEventList::E_KeyRepeat, false, eKey );

	if ( IsConsumingKeyEvents() )
		ctFocused->SendEvent( stEventInfo );

	else
	{
		BroadcastEvent( stEventInfo, true );

		if ( !IsTemplate() )
			OnKeyRepeatBP( eKey );
	}

	return stEventInfo.bHandled;
}


bool AKUIInterface::OnKeyChar( TCHAR chChar )
{
	if ( !IsConsumingKeyEvents() )
		return false;

	FKUIInterfaceContainerCharEvent stEventInfo( EKUIInterfaceContainerEventList::E_KeyChar, false, chChar );

	if ( IsConsumingKeyEvents() )
		ctFocused->SendEvent( stEventInfo );

	else
	{
		BroadcastEvent( stEventInfo, true );

		if ( !IsTemplate() )
			OnKeyCharBP( stEventInfo.strChar );
	}

	return stEventInfo.bHandled;
}


void AKUIInterface::OnScreenResolutionChange( const FVector2D& v2OldRes, const FVector2D& v2NewRes )
{
	v2ScreenResolution = v2NewRes;

	KUIBroadcastEvent( FKUIInterfaceContainerScreenResolutionEvent, EKUIInterfaceContainerEventList::E_ScreenResolutionChange, v2OldRes, v2NewRes );

	if ( !IsTemplate() )
		OnScreenResolutionChangeBP( v2OldRes, v2NewRes );
}


void AKUIInterface::OnMatchStart()
{
	KUIBroadcastEvent( FKUIInterfaceEvent, EKUIInterfaceContainerEventList::E_MatchStart );

	if ( !IsTemplate() )
		OnMatchStartBP();
}


void AKUIInterface::OnMatchPaused( uint32 iPlayerId )
{
	KUIBroadcastEvent( FKUIInterfaceContainerPlayerEvent, EKUIInterfaceContainerEventList::E_MatchPaused, iPlayerId );

	if ( !IsTemplate() )
		OnMatchPausedBP( iPlayerId );
}


void AKUIInterface::OnMatchUnpaused( uint32 iPlayerId )
{
	KUIBroadcastEvent( FKUIInterfaceContainerPlayerEvent, EKUIInterfaceContainerEventList::E_MatchUnpaused, iPlayerId );

	if ( !IsTemplate() )
		OnMatchUnpauseBP( iPlayerId );
}


void AKUIInterface::OnMatchEnd()
{
	KUIBroadcastEvent( FKUIInterfaceEvent, EKUIInterfaceContainerEventList::E_MatchEnd );

	if ( !IsTemplate() )
		OnMatchEndBP();
}


void AKUIInterface::OnPlayerDeath( uint32 iPlayerId )
{
	KUIBroadcastEvent( FKUIInterfaceContainerPlayerEvent, EKUIInterfaceContainerEventList::E_PlayerDeath, iPlayerId )

	if ( !IsTemplate() )
		OnPlayerDeathBP( iPlayerId );
}


bool AKUIInterface::IsConsumingKeyEvents() const
{
	if ( ctFocused == NULL )
		return false;

	return ctFocused->IsKeyEventConsumer();
}


void AKUIInterface::PostInitializeComponents()
{
	Super::PostInitializeComponents();

#if KUI_INTERFACE_MOUSEOVER_DEBUG
	arDebugMouseOver.SetNum( ctRootContainers.Num() );
	arDebugMouseOver[ EKUIInterfaceRoot::R_Root ] = true;
	arDebugMouseOver[ EKUIInterfaceRoot::R_Overlay ] = false;
	arDebugMouseOver[ EKUIInterfaceRoot::R_Tooltip ] = false;
	arDebugMouseOver[ EKUIInterfaceRoot::R_Cursor ] = false;

	bDebugMouseOver = false;
	cmDebugMouseOver = NULL;
	cmDebugMouseOverLastTick = NULL;
	v2DebugMouseOverLocation = FVector2D::ZeroVector;
	v2DebugMouseOverSize = FVector2D::ZeroVector;

	KUINewNamedObjectAssign( cmDebugMouseOverTestBox, UKUIBoxInterfaceComponent, "Mouse Over Test Box" );
	cmDebugMouseOverTestBox->SetDrawColor( 255.f, 0.f, 0.f );
	cmDebugMouseOverTestBox->SetVisible( false );

	AddElement( EKUIInterfaceRoot::R_Root, cmDebugMouseOverTestBox );
#endif // KUI_INTERFACE_MOUSEOVER_DEBUG
}


void AKUIInterface::OnMatchStateChange( FName nMatchState )
{
	KUIBroadcastEvent( FKUIInterfaceContainerMatchStateEvent, EKUIInterfaceContainerEventList::E_MatchStateChange, nMatchState );

	if ( nMatchState == MatchState::InProgress )
		OnMatchStart();

	else if ( nMatchState == MatchState::WaitingPostMatch )
		OnMatchEnd();

	if ( !IsTemplate() )
		OnMatchStateChangeBP( nMatchState );
}
