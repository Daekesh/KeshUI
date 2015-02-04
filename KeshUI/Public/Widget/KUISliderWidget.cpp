// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.


#include "KeshUI.h"
#include "KUIInterfaceElement.h"
#include "Widget/KUISliderWidget.h"


UKUISliderWidget::UKUISliderWidget( const class FObjectInitializer& oObjectInitializer )
: Super( oObjectInitializer )
{
	oHandle = NULL;
	fDeadZone = 1.f;
	fEndSize = 2.f;
	v2HandleOffset = FVector2D::ZeroVector;
	bHorizontal = true;
	iStepCount = 0;
	bDragging = false;
	fValue = 0.f;
	fMouseDownOffset = 0.f;
	dgSliderChange.BindUObject( this, &UKUISliderWidget::OnSliderValueChange );
}


UKUIInterfaceElement* UKUISliderWidget::GetHandle() const
{
	return oHandle.Get();
}


void UKUISliderWidget::SetHandle( UKUIInterfaceElement* oHandle )
{
	if ( this->oHandle.IsValid() )
		RemoveChild( this->oHandle.Get() );

	this->oHandle = oHandle;

	if ( this->oHandle.IsValid() )
		AddChild( this->oHandle.Get() );

	KUISendEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );
}


FVector2D UKUISliderWidget::GetHandleOffset() const
{
	return v2HandleOffset;
}


void UKUISliderWidget::SetHandleOffset( const FVector2D& v2HandleOffset )
{
	if ( this->v2HandleOffset.X == v2HandleOffset.X && this->v2HandleOffset.Y == v2HandleOffset.Y )
		return;

	this->v2HandleOffset = v2HandleOffset;
	
	KUISendEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );
}


float UKUISliderWidget::GetDeadZone() const
{
	return fDeadZone;
}


void UKUISliderWidget::SetDeadZone( float fDeadZone )
{
	if ( this->fDeadZone == fDeadZone )
		return;

	this->fDeadZone = fDeadZone;
}


float UKUISliderWidget::GetEndSize() const
{
	return fEndSize;
}


void UKUISliderWidget::SetEndSize( float fEndSize )
{
	if ( this->fEndSize == fEndSize )
		return;

	this->fEndSize = fEndSize;

	KUISendEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );
}


bool UKUISliderWidget::IsHorizontal() const
{
	return bHorizontal;
}


void UKUISliderWidget::SetHorizontal( bool bHorizontal )
{
	if ( this->bHorizontal == bHorizontal )
		return;

	this->bHorizontal = bHorizontal;

	KUISendEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );
}


uint8 UKUISliderWidget::GetStepCount() const
{
	return iStepCount;
}


void UKUISliderWidget::SetStepCount( uint8 iStepCount )
{
	if ( this->iStepCount == iStepCount )
		return;

	this->iStepCount = iStepCount;

	KUISendEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );
}


float UKUISliderWidget::GetValue() const
{
	return fValue;

	const float fTravelDistance = GetTravelDistance() - ( fDeadZone * 2.f );

	if ( fTravelDistance <= 0.f )
		return 0.f;

	return ( fValue - fDeadZone ) / fTravelDistance;
}


void UKUISliderWidget::SetValue( float fValue )
{
	//KUILogUO( "Value %f -> %f", this->fValue, fValue );

	if ( iStepCount == 0 )
		fValue = clamp( fValue, 0.f, 1.f );

	else
		fValue = round( clamp( fValue, 0.f, ( float ) iStepCount ) );

	if ( this->fValue == fValue )
		return;

	this->fValue = fValue;

	KUISendEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );
	KUISendSubEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_ValueChange );
}


float UKUISliderWidget::GetTravelDistance() const
{
	if ( oHandle.IsValid() )
	{
		if ( bHorizontal )
			return max( 0.f, GetSize().X - oHandle->GetSize().X - fEndSize - fEndSize );

		else
			return max( 0.f, GetSize().Y - oHandle->GetSize().Y - fEndSize - fEndSize );
	}

	if ( bHorizontal )
		return max( 0.f, GetSize().X - fEndSize - fEndSize );

	else
		return max( 0.f, GetSize().Y - fEndSize - fEndSize );
}


float UKUISliderWidget::GetStepSize() const
{
	if ( iStepCount == 0 )
		return 0.f;

	return ( GetTravelDistance() / ( float ) iStepCount );
}


void UKUISliderWidget::OnWidgetStateChange()
{
	Super::OnWidgetStateChange();

	if ( !oHandle.IsValid() )
		return;
	
	FVector2D v2Size = GetSize();

	if ( v2Size.X == 0.f || v2Size.Y == 0.f )
		return;

	const float fTravelDistance = GetTravelDistance();
	float fOffset = 0.f;

	if ( iStepCount == 0 )
		fOffset = fValue * fTravelDistance;

	else
		fOffset = ( fValue / ( float ) iStepCount ) * fTravelDistance;

	//KUILogUO( "OnWidgetStateChange Offset %f %f", fOffset, fEndSize );

	fOffset += fEndSize;

	if ( bHorizontal )
	{
		//fOffset -= oHandle->GetSize().X / 2.f;
		oHandle->SetLocation( v2HandleOffset.X + fOffset, v2HandleOffset.Y );
		//KUILogUO( "OnWidgetStateChange Horizontal: %f", v2HandleOffset.X + fOffset );
	}

	else
	{
		//fOffset -= oHandle->GetSize().Y / 2.f;
		oHandle->SetLocation( v2HandleOffset.X, v2HandleOffset.Y + fOffset );
		//KUILogUO( "OnWidgetStateChange Vertical: %f", v2HandleOffset.Y + fOffset );
	}
}


bool UKUISliderWidget::CanReceieveMouseEvents() const
{
	return true;
}


bool UKUISliderWidget::CanReceieveKeyEvents() const
{
	return true;
}


void UKUISliderWidget::SetValueChangeDelegate( UObject* oObject, FKUISliderValueChangePrototype fnSliderValueChangeCallback )
{
	if ( oObject == NULL || fnSliderValueChangeCallback == NULL )
		dgSliderChange.Unbind();

	else
		dgSliderChange.BindUObject( oObject, fnSliderValueChangeCallback );
}


void UKUISliderWidget::OnMouseMove( const FKUIInterfaceContainerMouseLocationEvent& stEventInfo )
{
	Super::OnMouseMove( stEventInfo );

	if ( !CanReceieveMouseEvents() )
		return;

	if ( !bDragging )
		return;

	if ( IsDisabledRecursive() )
		return;

	OnDrag( bHorizontal ? stEventInfo.v2NewLocation.X : stEventInfo.v2NewLocation.Y );
}


bool UKUISliderWidget::OnMouseButtonDown( const FKUIInterfaceContainerMouseButtonEvent& stEventInfo )
{
	if ( !CanReceieveMouseEvents() )
		return Super::OnMouseButtonDown( stEventInfo );

	if ( stEventInfo.eButton != EMouseButtons::Left )
		return Super::OnMouseButtonDown( stEventInfo );

	if ( !IsVisibleRecursive() )
		return Super::OnMouseButtonDown( stEventInfo );

	if ( IsDisabledRecursive() )
		return Super::OnMouseButtonDown( stEventInfo );

	if ( !IsMouseOver() )
		return Super::OnMouseButtonDown( stEventInfo );

	if ( oHandle.IsValid() && oHandle->IsMouseOver() )
	{
		if ( bHorizontal )
			fMouseDownOffset = stEventInfo.v2Location.X - oHandle->GetLastScreenRenderLocation().X - oHandle->GetSize().X / 2.f;

		else
			fMouseDownOffset = stEventInfo.v2Location.Y - oHandle->GetLastScreenRenderLocation().Y - oHandle->GetSize().Y / 2.f;
	}

	else
		fMouseDownOffset = 0.f;

	bDragging = true;
	OnDrag( bHorizontal ? stEventInfo.v2Location.X : stEventInfo.v2Location.Y );
	KUISendSubEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );

	return true;
}


bool UKUISliderWidget::OnMouseButtonUp( const FKUIInterfaceContainerMouseButtonEvent& stEventInfo )
{
	if ( !CanReceieveMouseEvents() )
		return Super::OnMouseButtonUp( stEventInfo );

	if ( stEventInfo.eButton != EMouseButtons::Left )
		return Super::OnMouseButtonUp( stEventInfo );

	if ( !IsVisibleRecursive() )
		return Super::OnMouseButtonUp( stEventInfo );

	if ( IsDisabledRecursive() )
		return Super::OnMouseButtonUp( stEventInfo );

	if ( !bDragging )
		return Super::OnMouseButtonUp( stEventInfo );

	bDragging = false;
	KUISendSubEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );

	return true;
}


void UKUISliderWidget::OnDrag( float fLocation )
{
	const float fTravelDistance = GetTravelDistance();

	if ( fTravelDistance == 0.f )
		return;

	float fValue = 0.f;

	if ( bHorizontal )
		fValue = fLocation - v2LastScreenRenderLocation.X - fEndSize - fMouseDownOffset - ( oHandle.IsValid() ? oHandle->GetSize().X / 2.f : 0.f );

	else
		fValue = fLocation - v2LastScreenRenderLocation.Y - fEndSize - fMouseDownOffset - ( oHandle.IsValid() ? oHandle->GetSize().Y / 2.f : 0.f );

	fValue = clamp( fValue, 0.f, fTravelDistance ) / fTravelDistance;

	if ( iStepCount == 0 )
		SetValue( fValue );

	else
		SetValue( fValue * ( float ) iStepCount );
}


void UKUISliderWidget::OnSliderValueChange( UKUISliderWidget* cmSlider )
{
	KUILogDebugUO( "Changed Value!: %f", fValue );
}


bool UKUISliderWidget::RespondsToEvent( uint8 iEventID ) const
{
	if ( Super::RespondsToEvent( iEventID ) )
		return true;

	return ( iEventID == EKUIInterfaceWidgetEventList::E_ValueChange );
}


void UKUISliderWidget::SendEvent( FKUIInterfaceEvent& stEventInfo )
{
	if ( IsTemplate() )
		return;

	Super::SendEvent( stEventInfo );

	if ( stEventInfo.iEventID == EKUIInterfaceWidgetEventList::E_ValueChange )
	{
		dgSliderChange.ExecuteIfBound( this );
		OnSliderValueChangeBP( this );
	}
}
