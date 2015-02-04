// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#include "KeshUI.h"
#include "KUIInterfaceElement.h"
#include "Widget/KUIButtonWidget.h"
#include "WidgetChildManager/KUIManagedButtonWidgetChildManager.h"


UKUIManagedButtonWidgetChildManager::UKUIManagedButtonWidgetChildManager( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	eDisabled = EKUIBoolMatch::V_Any;
	eMouseOver = EKUIBoolMatch::V_Any;
	eDown = EKUIBoolMatch::V_Any;

	arOffsets.SetNum( 8 );
	
	for ( uint8 i = 0; i < 8; ++i )
		arOffsets[ i ] = FVector2D::ZeroVector;
}


UKUIButtonWidget* UKUIManagedButtonWidgetChildManager::GetButton() const
{
	return Cast<UKUIButtonWidget>( cmWidget.Get() );
}


void UKUIManagedButtonWidgetChildManager::SetStates( TEnumAsByte<EKUIBoolMatch::Value> eDisabled, TEnumAsByte<EKUIBoolMatch::Value> eMouseOver, TEnumAsByte<EKUIBoolMatch::Value> eDown )
{
	this->eDisabled = eDisabled;
	this->eMouseOver = eMouseOver;
	this->eDown = eDown;
}


void UKUIManagedButtonWidgetChildManager::SetOffset( FVector2D v2Offset, TEnumAsByte<EKUIBoolMatch::Value> eDisabled, TEnumAsByte<EKUIBoolMatch::Value> eMouseOver, TEnumAsByte<EKUIBoolMatch::Value> eDown )
{
	for ( uint8 i = 0; i < 8; ++i )
	{
		if ( ( i & 1 ) && eDisabled == EKUIBoolMatch::V_False )
			continue;

		if ( !( i & 1 ) && eDisabled == EKUIBoolMatch::V_True )
			continue;

		if ( ( i & 2 ) && eMouseOver == EKUIBoolMatch::V_False )
			continue;

		if ( !( i & 2 ) && eMouseOver == EKUIBoolMatch::V_True )
			continue;

		if ( ( i & 4 ) && eDown == EKUIBoolMatch::V_False )
			continue;

		if ( !( i & 4 ) && eDown == EKUIBoolMatch::V_True )
			continue;

		arOffsets[ i ] = v2Offset;
	}
}


void UKUIManagedButtonWidgetChildManager::Update()
{
	Super::Update();

	if ( GetButton() == NULL )
	{
		KUIErrorDebugUO( "Null button" );
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
		if ( GetButton()->IsDisabled() )
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
		if ( GetButton()->IsMouseOver() )
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

	if ( bVisible && eDown != EKUIBoolMatch::V_Any )
	{
		if ( GetButton()->IsDown() )
		{
			if ( eDown == EKUIBoolMatch::V_False )
				bVisible = false;
		}

		else
		{
			if ( eDown == EKUIBoolMatch::V_True )
				bVisible = false;
		}
	}

	oChild->SetVisible( bVisible );

	oChild->SetLocationStruct( arOffsets[
		( GetButton()->IsDisabled()  ? 1 : 0 ) + 
		( GetButton()->IsMouseOver() ? 2 : 0 ) + 
		( GetButton()->IsDown()      ? 4 : 0 )
	] );
}
