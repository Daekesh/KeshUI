// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "KeshUI.h"
#include "KUIInterface.h"
#include "Widget/KUISimpleClickWidget.h"


UKUISimpleClickWidget::UKUISimpleClickWidget( const class FObjectInitializer& oObjectInitializer )
: Super( oObjectInitializer )
{
	bMouseOver = false;
	bDown = false;
	v2ClickOffset = FVector2D::ZeroVector;
	dgSimpleClickWidgetClick.BindUObject( this, &UKUISimpleClickWidget::OnClick );
}


bool UKUISimpleClickWidget::IsDown() const
{
	return bDown;
}


void UKUISimpleClickWidget::SetClickDelegate( UObject* oObject, FKUISimpleClickWidgetClickPrototype fnSimpleClickWidgetClickDelegate )
{
	if ( oObject == NULL || fnSimpleClickWidgetClickDelegate == NULL )
		dgSimpleClickWidgetClick.Unbind();

	else
		dgSimpleClickWidgetClick.BindUObject( oObject, fnSimpleClickWidgetClickDelegate );
}


void UKUISimpleClickWidget::OnMouseMove( const FKUIInterfaceContainerMouseLocationEvent& stEventInfo )
{
	Super::OnMouseMove( stEventInfo );

	if ( !CanReceieveMouseEvents() )
		return;

	bool bMouseOver = IsMouseOver();

	if ( this->bMouseOver == bMouseOver )
		return;

	this->bMouseOver = bMouseOver;
	KUISendSubEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );
}


bool UKUISimpleClickWidget::OnMouseButtonDown( const FKUIInterfaceContainerMouseButtonEvent& stEventInfo )
{
	if ( !CanReceieveMouseEvents() )
		return Super::OnMouseButtonDown( stEventInfo );

	if ( stEventInfo.eButton != EMouseButtons::Left )
		return Super::OnMouseButtonDown( stEventInfo );

	bDown = false;

	if ( !bMouseOver )
		return Super::OnMouseButtonDown( stEventInfo );

	if ( !IsVisibleRecursive() )
		return Super::OnMouseButtonDown( stEventInfo );

	if ( IsDisabledRecursive() )
		return Super::OnMouseButtonDown( stEventInfo );

	bDown = true;
	v2ClickOffset = stEventInfo.v2Location - GetScreenLocation();
	KUISendSubEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );

	return true;
}


bool UKUISimpleClickWidget::OnMouseButtonUp( const FKUIInterfaceContainerMouseButtonEvent& stEventInfo )
{
	if ( !CanReceieveMouseEvents() )
		return Super::OnMouseButtonUp( stEventInfo );

	if ( stEventInfo.eButton != EMouseButtons::Left )
		return Super::OnMouseButtonUp( stEventInfo );

	if ( !bDown )
		return Super::OnMouseButtonUp( stEventInfo );

	bool bTriggerClick = ( IsVisibleRecursive() && !IsDisabledRecursive() );

	if ( bTriggerClick && bMouseOver )
	{
		FKUIInterfaceWidgetClickEvent stClickEventInfo( EKUIInterfaceWidgetEventList::E_Click, v2ClickOffset );
		SendEvent( stClickEventInfo );
	}

	bDown = false;
	FKUIInterfaceEvent stStateEventInfo( EKUIInterfaceWidgetEventList::E_StateChange );
	SendEvent( stStateEventInfo );

	if ( bTriggerClick )
		return bMouseOver;

	return Super::OnMouseButtonUp( stEventInfo );
}


void UKUISimpleClickWidget::OnClick( UKUISimpleClickWidget* cmClicked, const FVector2D& v2ClickOffset )
{
	KUILogDebugUO( "Clicked!" );
}


bool UKUISimpleClickWidget::RespondsToEvent( uint8 iEventID ) const
{
	if ( Super::RespondsToEvent( iEventID ) )
		return true;

	return ( iEventID == EKUIInterfaceWidgetEventList::E_Click );
}


void UKUISimpleClickWidget::SendEvent( FKUIInterfaceEvent& stEventInfo )
{
	if ( IsTemplate() )
		return;

	Super::SendEvent( stEventInfo );

	if ( stEventInfo.iEventID == EKUIInterfaceWidgetEventList::E_Click )
	{
		dgSimpleClickWidgetClick.ExecuteIfBound( this, ( ( FKUIInterfaceWidgetClickEvent* ) &stEventInfo )->v2ClickOffset );
		OnClickBP( *( ( FKUIInterfaceWidgetClickEvent* ) &stEventInfo ) );
	}
}


bool UKUISimpleClickWidget::CanReceieveMouseEvents() const
{
	return true;
}
