// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#include "KeshUI/KeshUI.h"
#include "KeshUI/Component/KUITriangleListInterfaceComponent.h"


UKUITriangleListInterfaceComponent::UKUITriangleListInterfaceComponent( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	arTriangles.SetNum( 0 );
	tTexture = NULL;
	v4Bounds.X = 0.f;
	v4Bounds.Y = 0.f;
	v4Bounds.Z = 0.f;
	v4Bounds.W = 0.f;
}


int16 UKUITriangleListInterfaceComponent::GetTriangleCount() const
{
	return arTriangles.Num();
}


void UKUITriangleListInterfaceComponent::SetTriangleCount( int16 iTriangleCount )
{
	arTriangles.SetNumZeroed( iTriangleCount );
}


const FCanvasUVTri& UKUITriangleListInterfaceComponent::GetTriangle( int32 iIndex ) const
{
	static FCanvasUVTri stDefault = FCanvasUVTri();

	if ( iIndex < 0 || iIndex >= arTriangles.Num() )
		return stDefault;

	return arTriangles[ iIndex ];
}


void UKUITriangleListInterfaceComponent::AddTriangleStruct( const FCanvasUVTri& stTriangle )
{
	arTriangles.Add( stTriangle );

	if ( arTriangles.Num() > 1 )
	{
		v4Bounds.X = min( v4Bounds.X, min( stTriangle.V0_Pos.X, min( stTriangle.V1_Pos.X, stTriangle.V2_Pos.X ) ) );
		v4Bounds.Y = min( v4Bounds.Y, min( stTriangle.V0_Pos.Y, min( stTriangle.V1_Pos.Y, stTriangle.V2_Pos.Y ) ) );
		v4Bounds.Z = max( v4Bounds.Z, max( stTriangle.V0_Pos.X, max( stTriangle.V1_Pos.X, stTriangle.V2_Pos.X ) ) );
		v4Bounds.W = max( v4Bounds.W, max( stTriangle.V0_Pos.Y, max( stTriangle.V1_Pos.Y, stTriangle.V2_Pos.Y ) ) );
	
	}
	
	else
	{
		v4Bounds.X = min( stTriangle.V0_Pos.X, min( stTriangle.V1_Pos.X, stTriangle.V2_Pos.X ) );
		v4Bounds.Y = min( stTriangle.V0_Pos.Y, min( stTriangle.V1_Pos.Y, stTriangle.V2_Pos.Y ) );
		v4Bounds.Z = max( stTriangle.V0_Pos.X, max( stTriangle.V1_Pos.X, stTriangle.V2_Pos.X ) );
		v4Bounds.W = max( stTriangle.V0_Pos.Y, max( stTriangle.V1_Pos.Y, stTriangle.V2_Pos.Y ) );
	}

	v2Size.X = v4Bounds.Z - v4Bounds.X;
	v2Size.Y = v4Bounds.W - v4Bounds.Y;
	InvalidateRenderCache();
}


void UKUITriangleListInterfaceComponent::AddTriangle( FVector2D v2ScreenPointA, FVector2D v2ScreenPointB, FVector2D v2ScreenPointC, 
	FVector2D v2TexturePointA, FVector2D v2TexturePointB, FVector2D v2TexturePointC,
	FLinearColor lcPointA, FLinearColor lcPointB, FLinearColor lcPointC )
{
	FCanvasUVTri stTriangle;

	stTriangle.V0_Pos = v2ScreenPointA;
	stTriangle.V0_UV = v2TexturePointA;
	stTriangle.V0_Color = lcPointA;

	stTriangle.V1_Pos = v2ScreenPointB;
	stTriangle.V1_UV = v2TexturePointB;
	stTriangle.V1_Color = lcPointB;

	stTriangle.V2_Pos = v2ScreenPointC;
	stTriangle.V2_UV = v2TexturePointC;
	stTriangle.V2_Color = lcPointC;

	AddTriangleStruct( stTriangle );
}


void UKUITriangleListInterfaceComponent::SetTriangleStruct( int32 iIndex, const FCanvasUVTri& stTriangle )
{
	if ( iIndex < 0 || iIndex >= arTriangles.Num() )
		return;

	bool bRecalcBounds = ( v4Bounds.X == arTriangles[ iIndex ].V0_Pos.X || v4Bounds.X == arTriangles[ iIndex ].V2_Pos.X || v4Bounds.X == arTriangles[ iIndex ].V2_Pos.X ) ||
		( v4Bounds.Z == arTriangles[ iIndex ].V0_Pos.X || v4Bounds.Z == arTriangles[ iIndex ].V2_Pos.X || v4Bounds.Z == arTriangles[ iIndex ].V2_Pos.X ) ||
		( v4Bounds.Y == arTriangles[ iIndex ].V0_Pos.Y || v4Bounds.Y == arTriangles[ iIndex ].V2_Pos.Y || v4Bounds.Y == arTriangles[ iIndex ].V2_Pos.Y ) ||
		( v4Bounds.W == arTriangles[ iIndex ].V0_Pos.Y || v4Bounds.W == arTriangles[ iIndex ].V2_Pos.Y || v4Bounds.W == arTriangles[ iIndex ].V2_Pos.Y );

	arTriangles[ iIndex ] = stTriangle;

	if ( bRecalcBounds )
	{
		for ( int32 i = 0; i < arTriangles.Num(); ++i )
		{
			// Assume we're setting from 0->X, so if we find an unset triangle, break.
			if ( arTriangles[ i ].V0_Pos == arTriangles[ i ].V1_Pos && arTriangles[ i ].V0_Pos == arTriangles[ i ].V2_Pos )
				break;

			if ( i > 1 )
			{
				v4Bounds.X = min( v4Bounds.X, min( arTriangles[ i ].V0_Pos.X, min( arTriangles[ i ].V1_Pos.X, arTriangles[ i ].V2_Pos.X ) ) );
				v4Bounds.Y = min( v4Bounds.Y, min( arTriangles[ i ].V0_Pos.Y, min( arTriangles[ i ].V1_Pos.Y, arTriangles[ i ].V2_Pos.Y ) ) );
				v4Bounds.Z = max( v4Bounds.Z, max( arTriangles[ i ].V0_Pos.X, max( arTriangles[ i ].V1_Pos.X, arTriangles[ i ].V2_Pos.X ) ) );
				v4Bounds.W = max( v4Bounds.W, max( arTriangles[ i ].V0_Pos.Y, max( arTriangles[ i ].V1_Pos.Y, arTriangles[ i ].V2_Pos.Y ) ) );

			}

			else
			{
				v4Bounds.X = min( arTriangles[ i ].V0_Pos.X, min( arTriangles[ i ].V1_Pos.X, arTriangles[ i ].V2_Pos.X ) );
				v4Bounds.Y = min( arTriangles[ i ].V0_Pos.Y, min( arTriangles[ i ].V1_Pos.Y, arTriangles[ i ].V2_Pos.Y ) );
				v4Bounds.Z = max( arTriangles[ i ].V0_Pos.X, max( arTriangles[ i ].V1_Pos.X, arTriangles[ i ].V2_Pos.X ) );
				v4Bounds.W = max( arTriangles[ i ].V0_Pos.Y, max( arTriangles[ i ].V1_Pos.Y, arTriangles[ i ].V2_Pos.Y ) );
			}
		}
	}

	else
	{
		v4Bounds.X = min( v4Bounds.X, min( stTriangle.V0_Pos.X, min( stTriangle.V1_Pos.X, stTriangle.V2_Pos.X ) ) );
		v4Bounds.Y = min( v4Bounds.Y, min( stTriangle.V0_Pos.Y, min( stTriangle.V1_Pos.Y, stTriangle.V2_Pos.Y ) ) );
		v4Bounds.Z = max( v4Bounds.Z, max( stTriangle.V0_Pos.X, max( stTriangle.V1_Pos.X, stTriangle.V2_Pos.X ) ) );
		v4Bounds.W = max( v4Bounds.W, max( stTriangle.V0_Pos.Y, max( stTriangle.V1_Pos.Y, stTriangle.V2_Pos.Y ) ) );
	}

	v2Size.X = v4Bounds.Z - v4Bounds.X;
	v2Size.Y = v4Bounds.W - v4Bounds.Y;
	InvalidateRenderCache();
}


void UKUITriangleListInterfaceComponent::SetTriangle( int32 iIndex, FVector2D v2ScreenPointA, FVector2D v2ScreenPointB, FVector2D v2ScreenPointC,
	FVector2D v2TexturePointA, FVector2D v2TexturePointB, FVector2D v2TexturePointC,
	FLinearColor lcPointA, FLinearColor lcPointB, FLinearColor lcPointC )
{
	FCanvasUVTri stTriangle;

	stTriangle.V0_Pos = v2ScreenPointA;
	stTriangle.V0_UV = v2TexturePointA;
	stTriangle.V0_Color = lcPointA;

	stTriangle.V1_Pos = v2ScreenPointB;
	stTriangle.V1_UV = v2TexturePointB;
	stTriangle.V1_Color = lcPointB;

	stTriangle.V2_Pos = v2ScreenPointC;
	stTriangle.V2_UV = v2TexturePointC;
	stTriangle.V2_Color = lcPointC;

	SetTriangleStruct( iIndex, stTriangle );
}


UTexture* UKUITriangleListInterfaceComponent::GetTexture() const
{
	return tTexture;
}


void UKUITriangleListInterfaceComponent::SetTexture( UTexture* tTexture )
{
	if ( this->tTexture == tTexture )
		return;

	this->tTexture = tTexture;

	Invalidate();
}


const FVector2D& UKUITriangleListInterfaceComponent::GetSize() const
{
	if ( arTriangles.Num() == 0 )
		return FVector2D::ZeroVector;
	
	return v2Size;
}


bool UKUITriangleListInterfaceComponent::IsPositionable() const
{
	return false;
}


bool UKUITriangleListInterfaceComponent::HasValidComponents() const
{
	if ( arTriangles.Max() == 0 )
		return false;

	if ( tTexture == NULL )
		return false;

	return true;
}


void UKUITriangleListInterfaceComponent::ConstructNewItem()
{
	if ( !HasValidComponents() )
		return;

	if ( tTexture == NULL )
		return;
	
	if ( tTexture->Resource == NULL )
		return;

	stItem.Reset();

	stItem = TSharedPtr<FCanvasTriangleItem>( new FCanvasTriangleItem(
		arTriangles,
		tTexture->Resource
	) );

	v2Size.X = v4Bounds.Z - v4Bounds.X;
	v2Size.Y = v4Bounds.W - v4Bounds.Y;

	Super::ConstructNewItem();
}
