// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#include "KeshUI.h"
#include "KUIInterfaceContainer.h"
#include "Component/KUIPolygonInterfaceComponent.h"


UKUIPolygonInterfaceComponent::UKUIPolygonInterfaceComponent( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	v2Position = FVector2D::ZeroVector;
	v2Radius = FVector2D::ZeroVector;
	iSideCount = 0;
	tTexture = NULL;
}


FVector2D UKUIPolygonInterfaceComponent::GetPosition() const
{
	return v2Position;
}


void UKUIPolygonInterfaceComponent::SetPositionStruct( const FVector2D& vPosition )
{
	SetPosition( vPosition.X, vPosition.Y );
}


void UKUIPolygonInterfaceComponent::SetPosition( float fX, float fY )
{
	if ( v2Position.X == fX && v2Position.Y == fY )
		return;

	v2Position.X = fX;
	v2Position.Y = fY;

	if ( GetContainer() != NULL )
	{
		FKUIInterfaceContainerElementEvent stContainerEventInfo( EKUIInterfaceContainerEventList::E_ChildLocationChange, this );
		GetContainer()->SendEvent( stContainerEventInfo );
	}

	Invalidate();
}


FVector2D UKUIPolygonInterfaceComponent::GetRadius() const
{
	return v2Radius;
}


void UKUIPolygonInterfaceComponent::SetRadiusStruct( const FVector2D& v2Radius )
{
	SetRadius( v2Radius.X, v2Radius.Y );
}


void UKUIPolygonInterfaceComponent::SetRadius( float fWidth, float fHeight )
{
	fHeight = ( fHeight <= 0.f ? fWidth : fHeight );

	if ( v2Radius.X == fWidth && v2Radius.Y == fHeight )
		return;

	v2Radius.X = fWidth;
	v2Radius.Y = fHeight;

	if ( GetContainer() != NULL )
	{
		FKUIInterfaceContainerElementEvent stEventInfo( EKUIInterfaceContainerEventList::E_ChildSizeChange, this );
		GetContainer()->SendEvent( stEventInfo );
	}

	Invalidate();
}


int32 UKUIPolygonInterfaceComponent::GetSideCount() const
{
	return iSideCount;
}


void UKUIPolygonInterfaceComponent::SetSideCount( int32 iSideCount )
{
	if ( this->iSideCount == iSideCount )
		return;

	this->iSideCount = iSideCount;

	Invalidate();
}


UTexture* UKUIPolygonInterfaceComponent::GetTexture() const
{
	return tTexture;
}


void UKUIPolygonInterfaceComponent::SetTexture( UTexture* tTexture )
{
	if ( this->tTexture == tTexture )
		return;

	this->tTexture = tTexture;	

	Invalidate();
}


FVector2D UKUIPolygonInterfaceComponent::GetSize() const
{
	return ( v2Radius * 2.f );
}


bool UKUIPolygonInterfaceComponent::HasValidComponents() const
{
	if ( v2Radius.X <= 0 || v2Radius.Y <= 0 )
		return false;

	if ( iSideCount < 3 || iSideCount > 10000 ) // Sanity check!
		return false;

	return true;
}


void UKUIPolygonInterfaceComponent::ConstructNewItem()
{
	if ( !HasValidComponents() )
		return;

	stItem.Reset();

	if ( tTexture == NULL )
		stItem = TSharedPtr<FCanvasNGonItem>( new FCanvasNGonItem(
			v2Position,
			v2Radius,
			iSideCount,
			GetDrawColor().ReinterpretAsLinear()
		) );

	else
		stItem = TSharedPtr<FCanvasNGonItem>( new FCanvasNGonItem(
			v2Position,
			v2Radius,
			iSideCount,
			tTexture->CreateResource(),
			GetDrawColor().ReinterpretAsLinear()
		) );

	Super::ConstructNewItem();
}
