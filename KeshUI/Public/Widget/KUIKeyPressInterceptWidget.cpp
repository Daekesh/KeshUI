// Copyright 2014-2015 Matt Chapman. All Rights Reserved.


#include "KeshUI.h"
#include "KUIInterface.h"
#include "Widget/KUIKeyPressInterceptWidget.h"


UKUIKeyPressInterceptWidget::UKUIKeyPressInterceptWidget( const class FObjectInitializer& oObjectInitializer )
	: Super( oObjectInitializer )
{
	bAllowCancel = true;
	dgInterceptedKeyPress.Unbind();
	dgInterceptCancelled.Unbind();
}


bool UKUIKeyPressInterceptWidget::IsCancellable() const
{
	return bAllowCancel;
}


void UKUIKeyPressInterceptWidget::SetCancellable( bool bCancellable )
{
	bAllowCancel = bCancellable;
}


bool UKUIKeyPressInterceptWidget::WillInterceptNextKeyPress() const
{
	if ( GetInterface() == NULL )
		return false;

	return ( GetInterface()->GetFocus() == this );
}


void UKUIKeyPressInterceptWidget::InterceptNextKeyPress()
{
	if ( GetInterface() == NULL )
		return;

	GetInterface()->SetFocus( this );
}


void UKUIKeyPressInterceptWidget::SetInterceptedKeyPressDelegate( UObject* oObject, FKUIInterceptedKeyPressPrototype fnCallback )
{
	if ( oObject == NULL || fnCallback == NULL )
		dgInterceptedKeyPress.Unbind();

	else
		dgInterceptedKeyPress.BindUObject( oObject, fnCallback );
}


void UKUIKeyPressInterceptWidget::SetKeyPressCancelledDelegate( UObject* oObject, FKUIKeyInterceptCancelledPrototype fnCallback )
{
	if ( oObject == NULL || fnCallback == NULL )
		dgInterceptCancelled.Unbind();

	else
		dgInterceptCancelled.BindUObject( oObject, fnCallback );
}


bool UKUIKeyPressInterceptWidget::CanReceieveMouseEvents() const
{
	return true;
}


bool UKUIKeyPressInterceptWidget::CanReceieveKeyEvents() const
{
	return true;
}


bool UKUIKeyPressInterceptWidget::IsInputEventConsumer() const
{
	return true;
}


bool UKUIKeyPressInterceptWidget::OnKeyDown( const FKUIInterfaceContainerKeyEvent& stEventInfo )
{
	if ( stEventInfo.bHandled )
		return Super::OnKeyDown( stEventInfo );
	
	if ( GetInterface() == NULL || GetInterface()->GetFocus() != this )
		return Super::OnKeyDown( stEventInfo );

	if ( bAllowCancel && stEventInfo.eKey == EKeys::Escape )
	{
		dgInterceptCancelled.ExecuteIfBound( this );
		return true;
	}

	// Don't intercept modifier keys, but add them to the keybind later
	if ( stEventInfo.eKey.IsModifierKey() )
		return true;

	KeyIntercept( stEventInfo.eKey );
	return true;
}


bool UKUIKeyPressInterceptWidget::OnMouseButtonDown( const FKUIInterfaceContainerMouseButtonEvent& stEventInfo )
{
	if ( stEventInfo.bHandled )
		return Super::OnMouseButtonDown( stEventInfo );

	if ( GetInterface() == NULL || GetInterface()->GetFocus() != this )
		return Super::OnMouseButtonDown( stEventInfo );

	switch ( stEventInfo.eButton )
	{
		case EMouseButtons::Left:
			KeyIntercept( EKeys::LeftMouseButton );
			break;

		case EMouseButtons::Right:
			KeyIntercept( EKeys::RightMouseButton );
			break;

		case EMouseButtons::Middle:
			KeyIntercept( EKeys::MiddleMouseButton );
			break;

		case EMouseButtons::Thumb01:
			KeyIntercept( EKeys::ThumbMouseButton );
			break;

		case EMouseButtons::Thumb02:
			KeyIntercept( EKeys::ThumbMouseButton2 );
			break;

		default:
			return false;
	}

	return true;
}


void UKUIKeyPressInterceptWidget::KeyIntercept( const FKey& stKey )
{
	APlayerController* aPC = GetInterface()->GetPlayerController();

	if ( aPC == NULL )
		return;

	FKeyBind stKeyBind;
	stKeyBind.Key = stKey;
	stKeyBind.bIgnoreAlt = false;
	stKeyBind.bIgnoreCmd = false;
	stKeyBind.bIgnoreCtrl = false;
	stKeyBind.bIgnoreShift = false;
	stKeyBind.Alt = aPC->IsInputKeyDown( EKeys::LeftAlt );
	stKeyBind.Control = aPC->IsInputKeyDown( EKeys::LeftControl );
	stKeyBind.Cmd = aPC->IsInputKeyDown( EKeys::LeftCommand );
	stKeyBind.Shift = aPC->IsInputKeyDown( EKeys::LeftShift );

	dgInterceptedKeyPress.ExecuteIfBound( this, stKeyBind );
	
	if ( GetInterface() != NULL )
		GetInterface()->SetFocus( NULL );
}
