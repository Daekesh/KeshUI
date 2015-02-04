// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "KeshUI.h"
#include "Component/KUILineInterfaceComponent.h"


UKUILineInterfaceComponent::UKUILineInterfaceComponent( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	bItemInvalidated = true;
	
	vOrigin = FVector::ZeroVector;
	vEnd = FVector::ZeroVector;
	fThickness = 1.f;
}


FVector UKUILineInterfaceComponent::GetOrigin() const
{
	return vOrigin;
}


void UKUILineInterfaceComponent::SetOriginStruct( const FVector& vOrigin )
{
	SetOrigin( vOrigin.X, vOrigin.Y, vOrigin.Z );
}


void UKUILineInterfaceComponent::SetOrigin( float fX, float fY, float fZ )
{
	if ( vOrigin.X == fX && vOrigin.Y == fY && vOrigin.Z == fZ )
		return;

	vOrigin.X = fX;
	vOrigin.Y = fY;
	vOrigin.Z = fZ;

	InvalidateRenderCache();
}


FVector UKUILineInterfaceComponent::GetEnd() const
{
	return vEnd;
}


void UKUILineInterfaceComponent::SetEndStruct( const FVector& vEnd )
{
	SetEnd( vEnd.X, vEnd.Y, vEnd.Z );
}


void UKUILineInterfaceComponent::SetEnd( float fX, float fY, float fZ )
{
	if ( vEnd.X == fX && vEnd.Y == fY && vEnd.Z == fZ )
		return;

	vEnd.X = fX;
	vEnd.Y = fY;
	vEnd.Z = fZ;

	InvalidateRenderCache();
}


float UKUILineInterfaceComponent::GetThickness() const
{
	return fThickness;
}


void UKUILineInterfaceComponent::SetThickness( float fThickness )
{
	if ( this->fThickness == fThickness )
		return;

	this->fThickness = fThickness;

	InvalidateRenderCache();
}


FVector2D UKUILineInterfaceComponent::GetSize() const
{
	return FVector2D(
		abs( vOrigin.X - vEnd.X ),
		abs( vOrigin.Y - vEnd.Y )
	);
}


bool UKUILineInterfaceComponent::IsPositionable() const
{
	return false;
}


bool UKUILineInterfaceComponent::HasValidComponents() const
{
	if ( vOrigin.X == vEnd.X && vOrigin.Y == vEnd.Y && vOrigin.Z == vEnd.Z )
		return false;

	if ( fThickness <= 0.f )
		return false;

	return true;
}


void UKUILineInterfaceComponent::ConstructNewItem()
{
	if ( !HasValidComponents() )
		return;

	stItem.Reset();

	stItem = TSharedPtr<FCanvasLineItem>( new FCanvasLineItem(
		vOrigin,
		vEnd
	) );

	( ( FCanvasLineItem* ) stItem.Get() )->LineThickness = fThickness;

	Super::ConstructNewItem();
}


void UKUILineInterfaceComponent::Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject )
{
	if ( stItem.IsValid() )
	{
		( ( FCanvasLineItem* ) stItem.Get() )->Origin = vOrigin;
		( ( FCanvasLineItem* ) stItem.Get() )->EndPos = vEnd;
		( ( FCanvasLineItem* ) stItem.Get() )->LineThickness = fThickness;
	}

	Super::Render( aHud, oCanvas, v2Origin, oRenderCacheObject );
}
