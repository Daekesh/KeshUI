	// Copyright 2014-2015 Matt Chapman. All Rights Reserved.


#include "KeshUI.h"
#include "Widget/KUIComponentButtonWidget.h"


UKUIComponentButtonWidget::UKUIComponentButtonWidget( const class FObjectInitializer& oObjectInitializer )
: Super( oObjectInitializer )
{
	arButtonComponents.SetNum( EKUIButtonState::S_Max );
	arElementComponents.SetNum( EKUIButtonState::S_Max );
	v2ClickedElementOffset = FVector2D::ZeroVector;
}


UKUIInterfaceElement* UKUIComponentButtonWidget::GetButtonComponent( EKUIButtonState::State eState ) const
{
	return arButtonComponents[ eState ];
}


UKUIInterfaceElement* UKUIComponentButtonWidget::GetElementComponent( EKUIButtonState::State eState ) const
{
	return arElementComponents[ eState ];
}


void UKUIComponentButtonWidget::SetButtonComponent( EKUIButtonState::State eState, UKUIInterfaceElement* oComponent )
{
	if ( arButtonComponents[ eState ] != NULL )
		RemoveChild( arButtonComponents[ eState ] );

	arButtonComponents[ eState ] = oComponent;

	AddChild( arButtonComponents[ eState ] );

	arButtonComponents[ eState ]->SetZIndex( 0 );

	KUISendEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );
}


void UKUIComponentButtonWidget::SetButtonComponentSize( float fX, float fY )
{
	for ( uint8 i = EKUIButtonState::S_Default; i < EKUIButtonState::S_Max; ++i )
	{
		if ( arButtonComponents[ i ] == NULL )
			continue;

		arButtonComponents[ i ]->SetSize( fX, fY );
	}
}


void UKUIComponentButtonWidget::SetButtonComponentSizeStruct( FVector2D v2Size )
{
	SetButtonComponentSize( v2Size.X, v2Size.Y );
}


void UKUIComponentButtonWidget::SetElementComponent( EKUIButtonState::State eState, UKUIInterfaceElement* oComponent )
{
	if ( arElementComponents[ eState ] != NULL )
		RemoveChild( arElementComponents[ eState ] );

	arElementComponents[ eState ] = oComponent;

	AddChild( arElementComponents[ eState ] );

	arElementComponents[ eState ]->SetZIndex( 1 );
	arElementComponents[ eState ]->SetHorizontalAlignment( EKUIInterfaceHAlign::HA_Centre );
	arElementComponents[ eState ]->SetVerticalAlignment( EKUIInterfaceVAlign::VA_Centre );

	KUISendEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );
}


void UKUIComponentButtonWidget::OnWidgetStateChange()
{
	Super::OnWidgetStateChange();

	iButtonComponentIndex = EKUIButtonState::S_Default;

	if ( bMouseOver )
		iButtonComponentIndex += EKUIButtonState::S_MouseOver;

	if ( bDown )
		iButtonComponentIndex += EKUIButtonState::S_Clicked;

	if ( bDisabled )
		iButtonComponentIndex += EKUIButtonState::S_Disabled;

	iElementComponentIndex = iButtonComponentIndex;

	iButtonComponentIndex = GetComponentIndex( arButtonComponents, ( EKUIButtonState::State ) iButtonComponentIndex );
	iElementComponentIndex = GetComponentIndex( arElementComponents, ( EKUIButtonState::State ) iElementComponentIndex );

	for ( int32 i = 0; i < EKUIButtonState::S_Max; ++i )
	{
		if ( arButtonComponents[ i ] != NULL )
		{
			if ( i == iButtonComponentIndex )
			{
				arButtonComponents[ i ]->SetVisible( true );

				if ( i & EKUIButtonState::S_Clicked )
					arButtonComponents[ i ]->SetLocationStruct( v2ClickedElementOffset );

				else
					arButtonComponents[ i ]->SetLocationStruct( FVector2D::ZeroVector );
			}

			else if ( arButtonComponents[ i ] != arButtonComponents[ iButtonComponentIndex ] )
				arButtonComponents[ i ]->SetVisible( false );
		}

		if ( arElementComponents[ i ] != NULL )
		{
			if ( i == iElementComponentIndex )
			{
				arElementComponents[ i ]->SetVisible( true );

				if ( i & EKUIButtonState::S_Clicked )
					arElementComponents[ i ]->SetLocationStruct( v2ClickedElementOffset );

				else
					arElementComponents[ i ]->SetLocationStruct( FVector2D::ZeroVector );
			}

			else if ( arElementComponents[ i ] != arElementComponents[ iElementComponentIndex ] )
				arElementComponents[ i ]->SetVisible( false );
		}
	}
}


EKUIButtonState::State UKUIComponentButtonWidget::GetComponentIndex( const TArray<UKUIInterfaceElement*>& arComponents, EKUIButtonState::State eState ) const
{
	switch ( eState )
	{
		case EKUIButtonState::S_Default:
			break;

		case EKUIButtonState::S_MouseOver:
			if ( arComponents[ EKUIButtonState::S_MouseOver ] == NULL )
				eState = EKUIButtonState::S_Default;

			break;

		case EKUIButtonState::S_Clicked:
			if ( arComponents[ EKUIButtonState::S_Clicked ] == NULL )
				eState = EKUIButtonState::S_Default;

			break;

		case EKUIButtonState::S_MouseOver_Clicked:
			if ( arComponents[ EKUIButtonState::S_MouseOver_Clicked ] == NULL )
			{
				if ( arComponents[ EKUIButtonState::S_Clicked ] != NULL )
					eState = EKUIButtonState::S_Clicked;

				else if ( arComponents[ EKUIButtonState::S_MouseOver ] != NULL )
					eState = EKUIButtonState::S_MouseOver;

				else
					eState = EKUIButtonState::S_Default;
			}

			break;

		case EKUIButtonState::S_Disabled:
			if ( arComponents[ EKUIButtonState::S_Disabled ] == NULL )
				eState = EKUIButtonState::S_Default;

			break;

		case EKUIButtonState::S_MouseOver_Disabled:
			if ( arComponents[ EKUIButtonState::S_MouseOver_Disabled ] == NULL )
			{
				if ( arComponents[ EKUIButtonState::S_Disabled ] != NULL )
					eState = EKUIButtonState::S_Disabled;

				else if ( arComponents[ EKUIButtonState::S_MouseOver ] != NULL )
					eState = EKUIButtonState::S_MouseOver;

				else
					eState = EKUIButtonState::S_Default;
			}

			break;

		case EKUIButtonState::S_Clicked_Disabled:
			if ( arComponents[ EKUIButtonState::S_Clicked_Disabled ] == NULL )
			{
				if ( arComponents[ EKUIButtonState::S_Clicked ] != NULL )
					eState = EKUIButtonState::S_Clicked;

				else if ( arComponents[ EKUIButtonState::S_MouseOver ] != NULL )
					eState = EKUIButtonState::S_MouseOver;

				else
					eState = EKUIButtonState::S_Default;
			}

			break;

		case EKUIButtonState::S_MouseOver_Clicked_Disabled:
			if ( arComponents[ EKUIButtonState::S_MouseOver_Clicked_Disabled ] == NULL )
			{
				if ( arComponents[ EKUIButtonState::S_Disabled ] != NULL )
					eState = EKUIButtonState::S_Disabled;

				else if ( arComponents[ EKUIButtonState::S_Clicked ] != NULL )
					eState = EKUIButtonState::S_Clicked;

				else if ( arComponents[ EKUIButtonState::S_MouseOver ] != NULL )
					eState = EKUIButtonState::S_MouseOver;

				else
					eState = EKUIButtonState::S_Default;
			}

			break;
	}

	return eState;
}
