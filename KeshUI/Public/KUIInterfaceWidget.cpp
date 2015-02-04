// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#include "KeshUI.h"
#include "KUIInterfaceWidget.h"


UKUIInterfaceWidget::UKUIInterfaceWidget( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	bDisabled = false;
}


bool UKUIInterfaceWidget::IsDisabled() const
{
	return bDisabled;
}


void UKUIInterfaceWidget::SetDisabled( TEnumAsByte<EKUIBool::SetValue> eValue )
{
	FKUIInterfaceEvent stEventInfo;

	switch ( eValue )
	{
		case EKUIBool::SV_False:
			if ( !bDisabled )
				return;

			bDisabled = false;
			stEventInfo.iEventID = EKUIInterfaceWidgetEventList::E_StateChange;
			SendEvent( stEventInfo );
			break;

		case EKUIBool::SV_True:
			if ( bDisabled )
				return;

			bDisabled = true;
			stEventInfo.iEventID = EKUIInterfaceWidgetEventList::E_StateChange;
			SendEvent( stEventInfo );
			break;

		case EKUIBool::SV_Toggle:
			bDisabled = !bDisabled;
			stEventInfo.iEventID = EKUIInterfaceWidgetEventList::E_StateChange;
			SendEvent( stEventInfo );
			break;
	}
}


void UKUIInterfaceWidget::OnWidgetStateChange()
{
	UpdateChildManagers();
}


void UKUIInterfaceWidget::SetSize( float fWidth, float fHeight )
{
	if ( v2Size.X == fWidth && v2Size.Y == fHeight )
		return;

	Super::SetSize( fWidth, fHeight );

	KUISendEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );
}


bool UKUIInterfaceWidget::RespondsToEvent( uint8 iEventID ) const
{
	if ( Super::RespondsToEvent( iEventID ) )
		return true;

	return ( iEventID == EKUIInterfaceWidgetEventList::E_StateChange );
}


void UKUIInterfaceWidget::SendEvent( FKUIInterfaceEvent& stEventInfo )
{
	if ( IsTemplate() )
		return;

	Super::SendEvent( stEventInfo );

	if ( stEventInfo.iEventID == EKUIInterfaceWidgetEventList::E_StateChange )
	{
		OnWidgetStateChange();
		OnWidgetStateChangeBP();
	}

	else if ( stEventInfo.iEventID == EKUIInterfaceWidgetEventList::E_ValueChange )
	{
		OnWidgetStateChange();
		OnWidgetStateChangeBP();
	}

	else if ( stEventInfo.iEventID == EKUIInterfaceWidgetEventList::E_InlineValueChange )
	{
		OnWidgetStateChange();
		OnWidgetStateChangeBP();
	}
}
