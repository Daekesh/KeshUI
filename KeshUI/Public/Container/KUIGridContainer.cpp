// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.


#include "KeshUI.h"
#include "Container/KUIGridContainer.h"


UKUIGridContainer::UKUIGridContainer( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	ar2Elements.SetNum( 0 );
	arColumns.SetNum( 0 );
	arRows.SetNum( 0 );
}


void UKUIGridContainer::SetGridSlot( uint8 iRow, uint8 iColumn, UKUIInterfaceElement* oChild )
{
	if ( iRow >= arRows.Num() )
	{
		KUIErrorUO( "Invalid row: %d", iRow );
		return;
	}

	if ( iColumn >= arColumns.Num() )
	{
		KUIErrorUO( "Invalid column: %d", iColumn );
		return;
	}

	if ( oChild != NULL )
		RemoveGridSlot( oChild );

	UKUIInterfaceElement* const oCurrent = ar2Elements[ iRow ][ iColumn ].Get();

	if ( oCurrent != NULL )
	{
		RemoveChild( oCurrent );
		oCurrent->InvalidateAlignLocation();
	}

	ar2Elements[ iRow ][ iColumn ] = oChild;

	if ( oChild != NULL )
	{
		AddChild( oChild );
		oChild->InvalidateAlignLocation();
	}

	InvalidateLayout();
}


void UKUIGridContainer::RemoveGridSlot( UKUIInterfaceElement* oChild )
{
	if ( oChild == NULL )
	{
		KUIErrorDebugUO( "Trying to remove null child" );
		return;
	}

	bool bFound = false;

	for ( int32 i = 0; i < arRows.Num(); ++i )
	{
		for ( int32 j = 0; j < arColumns.Num(); ++j )
		{
			if ( ar2Elements[ i ][ j ].Get() != oChild )
				continue;

			bFound = true;
			ar2Elements[ i ][ j ] = NULL;
			break;
		}

		if ( bFound )
			break;
	}

	if ( !bFound )
	{
		KUIErrorDebugUO( "Child not found" );
		return;
	}

	InvalidateLayout();
}


bool UKUIGridContainer::RemoveChild( UKUIInterfaceElement* oChild )
{
	RemoveGridSlot( oChild );

	return Super::RemoveChild( oChild );
}


bool UKUIGridContainer::IsChildsLayoutManaged( UKUIInterfaceElement* oChild ) const
{
	if ( oChild == NULL )
		return false;

	for ( int32 i = 0; i < arRows.Num(); ++i )
		for ( int32 j = 0; j < arColumns.Num(); ++j )
			if ( ar2Elements[ i ][ j ].Get() == oChild )
				return true;

	return false;
}


void UKUIGridContainer::AddColumn( float fWidth )
{
	int32 iIndex = arColumns.Num();

	arColumns.Add( fWidth );

	for ( int32 i = 0; i < arRows.Num(); ++i )
	{
		ar2Elements[ i ].SetNum( iIndex + 1 );
		ar2Elements[ i ][ iIndex ] = NULL;
	}

	InvalidateLayout();
}


void UKUIGridContainer::AddRow( float fHeight )
{
	int32 iIndex = arRows.Num();

	arRows.Add( fHeight );

	ar2Elements.SetNum( arRows.Num() );
	ar2Elements[ iIndex ].SetNum( arColumns.Num() );

	for ( int32 i = 0; i < arColumns.Num(); ++i )
		ar2Elements[ iIndex ][ i ] = NULL;

	InvalidateLayout();
}


void UKUIGridContainer::SetColumnWidth( uint8 iColumn, float fWidth )
{
	if ( !arColumns.IsValidIndex( iColumn ) )
	{
		KUIErrorUO( "Invalid column: %d", iColumn );
		return;
	}

	arColumns[ iColumn ] = fWidth;

	InvalidateLayout();
}


void UKUIGridContainer::SetRowHeight( uint8 iRow, float fHeight )
{
	if ( !arRows.IsValidIndex( iRow ) )
	{
		KUIErrorUO( "Invalid row: %d", iRow );
		return;
	}

	arRows[ iRow ] = fHeight;

	InvalidateLayout();
}


void UKUIGridContainer::SetSize( float fWidth, float fHeight )
{
	if ( v2Size.X == fWidth && v2Size.Y == fHeight )
		return;
	
	Super::SetSize( fWidth, fHeight );

	InvalidateLayout();
}


void UKUIGridContainer::DoLayout()
{
	if ( arColumns.Num() == 0 || arRows.Num() == 0 )
		return;

	FVector2D v2RelativeSize = GetSize();
	FVector2D v2RelativeTotal = FVector2D::ZeroVector;

	for ( int32 i = 0; i < arColumns.Num(); ++i )
	{
		if ( arColumns[ i ] <= 0.f )
			continue;

		if ( arColumns[ i ] >= 1.f )
			v2RelativeSize.X -= arColumns[ i ];
		
		else
			v2RelativeTotal.X += arColumns[ i ];
	}

	for ( int32 i = 0; i < arRows.Num(); ++i )
	{
		if ( arRows[ i ] <= 0.f )
			continue;

		if ( arRows[ i] >= 1.f )
			v2RelativeSize.Y -= arRows[ i ];

		else
			v2RelativeTotal.Y += arRows[ i ];
	}

	// If we run out of space, all the relatively sized columns/rows will be hidden.
	if ( v2RelativeSize.X < 0.f )
		v2RelativeSize.X = 0.f;

	if ( v2RelativeSize.Y < 0.f )
		v2RelativeSize.Y = 0.f;

	TArray<float> arColumnEnds;
	TArray<float> arRowEnds;
	float fOffset;

	arColumnEnds.SetNum( arColumns.Num() );
	arRowEnds.SetNum( arRows.Num() );
	fOffset = 0.f;

	for ( int32 i = 0; i < arColumns.Num(); ++i )
	{
		// "Hidden" columns
		if ( arColumns[ i ] <= 0.f )
		{
			arColumnEnds[ i ] = fOffset;
			continue;
		}

		if ( arColumns[ i ] >= 1.f )
			fOffset += arColumns[ i ];

		else
			fOffset += ( arColumns[ i ] / v2RelativeTotal.X ) * v2RelativeSize.X;

		fOffset = floor( fOffset );
		arColumnEnds[ i ] = fOffset;
	}

	fOffset = 0.f;

	for ( int32 i = 0; i < arRows.Num(); ++i )
	{
		// "Hidden" rows
		if ( arRows[ i ] <= 0.f )
		{
			arRowEnds[ i ] = fOffset;
			continue;
		}

		if ( arRows[ i ] >= 1.f )
			fOffset += arRows[ i ];

		else
			fOffset += ( arRows[ i ] / v2RelativeTotal.Y ) * v2RelativeSize.Y;

		fOffset = floor( fOffset );
		arRowEnds[ i ] = fOffset;
	}

	FVector2D v2ChildLocation = FVector2D::ZeroVector;
	FVector2D v2ChildSize = FVector2D::ZeroVector;
	FVector2D v2SlotLocation = FVector2D::ZeroVector;
	FVector2D v2SlotSize = FVector2D::ZeroVector;

	for ( uint8 iRow = 0; iRow < arRows.Num(); ++iRow )
	{
		for ( uint8 iColumn = 0; iColumn < arColumns.Num(); ++iColumn )
		{
			if ( !ar2Elements[ iRow ][ iColumn ].IsValid() )
				continue;

			if ( arColumns[ iColumn ] > 0.f && arRows[ iRow ] > 0.f )
			{
				v2ChildSize = ar2Elements[ iRow ][ iColumn ]->GetSize();
				v2SlotLocation.X = ( iColumn == 0 ? 0.f : arColumnEnds[ iColumn - 1 ] );
				v2SlotLocation.Y = ( iRow == 0 ? 0.f : arRowEnds[ iRow - 1 ] );
				v2SlotSize.X = arColumnEnds[ iColumn ] - ( iColumn == 0 ? 0.f : arColumnEnds[ iColumn - 1 ] );
				v2SlotSize.Y = arRowEnds[ iRow ] - ( iRow == 0 ? 0.f : arRowEnds[ iRow - 1 ] );

				v2ChildLocation = ar2Elements[ iRow ][ iColumn ]->CalculateAlignLocation( 
					v2SlotLocation, 
					v2SlotSize, 
					v2ChildSize, 
					v2SlotLocation
				);

				if ( v2ChildLocation.X < 0.f )
					v2ChildLocation.X = 0.f;

				if ( v2ChildLocation.Y <= 0.f )
					v2ChildLocation.Y = 0.f;

				ar2Elements[ iRow ][ iColumn ]->SetLocation( v2ChildLocation.X, v2ChildLocation.Y );
				ar2Elements[ iRow ][ iColumn ]->SetVisible( true );
			}

			else
				ar2Elements[ iRow ][ iColumn ]->SetVisible( false );
		}
	}

	Super::DoLayout();
}
