// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "KeshUI.h"
#include "KUIInterfaceElement.h"
#include "Widget/KUICheckboxWidget.h"
#include "WidgetChildManager/KUIManagedCheckboxWidgetChildManager.h"


UKUIManagedCheckboxWidgetChildManager::UKUIManagedCheckboxWidgetChildManager( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	eDisabled = EKUIBoolMatch::V_Any;
	eMouseOver = EKUIBoolMatch::V_Any;
	eState = EKUICheckboxState::S_Max; // 3 states

	arOffsets.SetNum( 12 );
	
	for ( uint8 i = 0; i < 12; ++i )
		arOffsets[ i ] = FVector2D::ZeroVector;
}


UKUICheckboxWidget* UKUIManagedCheckboxWidgetChildManager::GetCheckbox() const
{
	return Cast<UKUICheckboxWidget>( cmWidget.Get() );
}


void UKUIManagedCheckboxWidgetChildManager::SetStates( TEnumAsByte<EKUIBoolMatch::Value> eDisabled, TEnumAsByte<EKUIBoolMatch::Value> eMouseOver, TEnumAsByte<EKUICheckboxState::State> eState )
{
	this->eDisabled = eDisabled;
	this->eMouseOver = eMouseOver;
	this->eState = eState;
}


void UKUIManagedCheckboxWidgetChildManager::SetOffset( FVector2D v2Offset, TEnumAsByte<EKUIBoolMatch::Value> eDisabled, TEnumAsByte<EKUIBoolMatch::Value> eMouseOver, TEnumAsByte<EKUICheckboxState::State> eState )
{
	for ( uint8 i = 0; i < 12; ++i )
	{
		if ( ( i & 1 ) && eDisabled == EKUIBoolMatch::V_False )
			continue;

		if ( !( i & 1 ) && eDisabled == EKUIBoolMatch::V_True )
			continue;

		if ( ( i & 2 ) && eMouseOver == EKUIBoolMatch::V_False )
			continue;

		if ( !( i & 2 ) && eMouseOver == EKUIBoolMatch::V_True )
			continue;

		if ( eState != EKUICheckboxState::S_Max )
		{
			// 3 states, 0, 4, 8
			if ( ( i & 12 ) == 0 && eState != EKUICheckboxState::S_NoValue )
				continue;

			if ( ( i & 12 ) == 4 && eState != EKUICheckboxState::S_Unchecked )
				continue;

			if ( ( i & 12 ) == 8 && eState != EKUICheckboxState::S_Checked )
				continue;
		}

		arOffsets[ i ] = v2Offset;
	}
}


void UKUIManagedCheckboxWidgetChildManager::Update()
{
	Super::Update();

	if ( GetCheckbox() == NULL )
	{
		KUIErrorDebugUO( "Null checkbox" );
		return;
	}

	if ( !oChild.IsValid() )
	{
		KUIErrorDebugUO( "Null child" );
		return;
	}

	bool bVisible = true;

	if ( eDisabled != EKUIBoolMatch::V_Any )
	{
		if ( GetCheckbox()->IsDisabled() )
		{
			if ( eDisabled == EKUIBoolMatch::V_False )
				bVisible = false;
		}

		else
		{
			if ( eDisabled == EKUIBoolMatch::V_True )
				bVisible = false;
		}
	}

	if ( bVisible && eMouseOver != EKUIBoolMatch::V_Any )
	{
		if ( GetCheckbox()->IsMouseOver() )
		{
			if ( eMouseOver == EKUIBoolMatch::V_False )
				bVisible = false;
		}

		else
		{
			if ( eMouseOver == EKUIBoolMatch::V_True )
				bVisible = false;
		}
	}

	if ( bVisible && eState != EKUICheckboxState::S_Max  )
	{
		if ( GetCheckbox()->GetValue() != eState )
			bVisible = false;		
	}

	oChild->SetVisible( bVisible );

	uint8 iIndex = 
		( GetCheckbox()->IsDisabled()  ? 1 : 0 ) + 
		( GetCheckbox()->IsMouseOver() ? 2 : 0 );

	switch ( GetCheckbox()->GetValue() )
	{
		/**
		 * Just for reference.
		 *
		case EKUICheckboxState::S_NoValue:
			iIndex += 0;
			break;
		 *
		 */

		case EKUICheckboxState::S_Unchecked:
			iIndex += 4;
			break;

		case EKUICheckboxState::S_Checked:
			iIndex += 8;
			break;
	}

	oChild->SetLocationStruct( arOffsets[ iIndex ] );
}
