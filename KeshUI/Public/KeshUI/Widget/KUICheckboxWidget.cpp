// Copyright 2014-2015 Matt Chapman. All Rights Reserved.


#include "KeshUI/KeshUI.h"
#include "KeshUI/Widget/KUICheckboxWidget.h"


UKUICheckboxWidget::UKUICheckboxWidget( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	bEnableNoValueState = false;
	eState = EKUICheckboxState::S_Unchecked;
	dgSimpleClickWidgetClick.BindUObject( this, &UKUICheckboxWidget::OnCheckboxClick );
	dgCheckboxStateChange.BindUObject( this, &UKUICheckboxWidget::OnCheckboxStateChange );
}


bool UKUICheckboxWidget::IsNoValueStateEnabled() const
{
	return bEnableNoValueState;
}


void UKUICheckboxWidget::SetNoValueStateEnabled( bool bEnabled )
{
	if ( bEnableNoValueState == bEnabled )
		return;

	bEnableNoValueState = bEnabled;

	if ( !bEnableNoValueState && eState == EKUICheckboxState::S_NoValue )
		SetValue( EKUICheckboxState::S_Unchecked );
}


TEnumAsByte<EKUICheckboxState::State> UKUICheckboxWidget::GetValue() const
{
	return eState;
}


void UKUICheckboxWidget::SetValue( TEnumAsByte<EKUICheckboxState::State> eValue )
{
	if ( eState == eValue )
		return;

	this->eState = eValue;

	KUISendEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_ValueChange );
}


bool UKUICheckboxWidget::IsChecked() const
{
	return ( eState == EKUICheckboxState::S_Checked );
}


void UKUICheckboxWidget::SetStateChangeDelegate( UObject* oObject, FKUICheckboxStateChangePrototype fnStateChangeCallback )
{
	if ( oObject == NULL || fnStateChangeCallback == NULL )
		dgCheckboxStateChange.Unbind();

	else
		dgCheckboxStateChange.BindUObject( oObject, fnStateChangeCallback );
}


bool UKUICheckboxWidget::CanReceieveMouseEvents() const
{
	return true;
}


bool UKUICheckboxWidget::CanReceieveKeyEvents() const
{
	return true;
}


void UKUICheckboxWidget::OnCheckboxClick( UKUISimpleClickWidget* cmClicked, const FVector2D& v2ClickOffset )
{
	switch ( eState )
	{
		case EKUICheckboxState::S_Unchecked:
			SetValue( EKUICheckboxState::S_Checked );
			break;

		case EKUICheckboxState::S_Checked:
			SetValue( bEnableNoValueState ? EKUICheckboxState::S_NoValue : EKUICheckboxState::S_Unchecked );
			break;

		case EKUICheckboxState::S_NoValue:
			SetValue( EKUICheckboxState::S_Unchecked );
			break;
	}
}


bool UKUICheckboxWidget::RespondsToEvent( uint8 iEventID ) const
{
	if ( Super::RespondsToEvent( iEventID ) )
		return true;

	return ( iEventID == EKUIInterfaceWidgetEventList::E_ValueChange );
}


void UKUICheckboxWidget::SendEvent( FKUIInterfaceEvent& stEventInfo )
{
	if ( IsTemplate() )
		return;

	Super::SendEvent( stEventInfo );

	if ( stEventInfo.iEventID == EKUIInterfaceWidgetEventList::E_ValueChange )
	{
		dgCheckboxStateChange.ExecuteIfBound( this );
		OnCheckboxStateChangeBP( this );
	}
}


void UKUICheckboxWidget::OnCheckboxStateChange( UKUICheckboxWidget* cmCheckbox )
{
	KUILogDebugUO( "Checkbox State Change!%d %d %d", ( bDown ? 1 : 0 ), ( bMouseOver ? 1 : 0 ), ( uint8 ) eState );
}
