// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.


#include "KeshUI.h"
#include "KUIInterface.h"
#include "Container/KUIListContainer.h"
#include "Container/KUIListRowColumnContainer.h"


UKUIListRowColumnContainer::UKUIListRowColumnContainer( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	arColumnElements.SetNum( 0 );
	arColumnWidths.SetNum( 0 );
	fSpacing = 0.f;
	iColumnCount = 0;
}


UKUIInterfaceElement* UKUIListRowColumnContainer::GetColumnElement( uint16 iColumn ) const
{
	if ( iColumn >= iColumnCount )
	{
		KUIErrorUO( "Invalid column: %d", iColumn );
		return NULL;
	}

	return arColumnElements[ iColumn ].Get();
}


void UKUIListRowColumnContainer::SetColumnElement( uint16 iColumn, UKUIInterfaceElement* oElement )
{
	if ( iColumn >= iColumnCount )
	{
		KUIErrorUO( "Invalid column: %d", iColumn );
		return;
	}

	if ( arColumnElements[ iColumn ].Get() == oElement )
		return;

	// Make sure it's not in the column array somewhere else
	if ( oElement != NULL )
		RemoveColumnElementByRef( oElement );

	UKUIInterfaceElement* const cmCurrent = arColumnElements[ iColumn ].Get();
	arColumnElements[ iColumn ] = NULL;

	if ( cmCurrent != NULL )
		RemoveChild( cmCurrent );

	if ( oElement != NULL )
	{
		AddChild( oElement );
		arColumnElements[ iColumn ] = oElement;
		oElement->InvalidateAlignLocation();
	}

	InvalidateLayout();
}


void UKUIListRowColumnContainer::RemoveColumnElementByRef( UKUIInterfaceElement* oElement )
{
	RemoveColumnElementByIndex( GetColumnElementIndexByRef( oElement ) );
}


void UKUIListRowColumnContainer::RemoveColumnElementByIndex( uint16 iColumn )
{
	if ( iColumn == KUI_LIST_INDEX_NONE )
	{
		KUIErrorDebugUO( "Trying to remove invalid column index" );
		return;
	}

	SetColumnElement( iColumn, NULL );
}


bool UKUIListRowColumnContainer::RemoveChild( UKUIInterfaceElement* oChild )
{
	RemoveColumnElementByRef( oChild );

	return Super::RemoveChild( oChild );
}


bool UKUIListRowColumnContainer::IsChildsLayoutManaged( UKUIInterfaceElement* oChild ) const
{
	if ( oChild == NULL )
		return false;

	if ( oChild->GetContainer() != this )
		return false;

	for ( int32 i = 0; i < iColumnCount; ++i )
		if ( arColumnElements[ i ].Get() == oChild )
			return true;

	return false;
}


uint16 UKUIListRowColumnContainer::GetColumnCount() const
{
	return iColumnCount;
}


uint8 UKUIListRowColumnContainer::AddColumn( float fWidth, UKUIInterfaceElement* oElement )
{
	SetColumnCount( GetColumnCount() + 1 );
	SetColumnWidth( GetColumnCount() - 1, fWidth );
	SetColumnElement( GetColumnCount() - 1, oElement );

	return ( GetColumnCount() - 1 );
}


void UKUIListRowColumnContainer::SetColumnCount( uint16 iColumnCount )
{
	if ( this->iColumnCount == iColumnCount )
		return;

	const uint16 iCurrentColumnCount = this->iColumnCount;
	this->iColumnCount = iColumnCount;

	if ( iCurrentColumnCount < iColumnCount )
	{
		arColumnElements.SetNum( iColumnCount );
		arColumnWidths.SetNum( iColumnCount );

		for ( uint16 i = iCurrentColumnCount; i < iColumnCount; ++i )
		{
			SetColumnElement( i, NULL );
			SetColumnWidth( i, 0.f );
		}
	}

	else
	{
		for ( uint16 i = iCurrentColumnCount - 1; i >= iColumnCount; --i )
			SetColumnElement( i, NULL );

		arColumnElements.SetNum( iColumnCount );
		arColumnWidths.SetNum( iColumnCount );
	}

	InvalidateLayout();
}


void UKUIListRowColumnContainer::RemoveColumns( uint16 iStart, uint16 iCount )
{
	if ( iCount == 0 )
		return;

	if ( iStart >= iColumnCount )
		return;

	if ( ( iColumnCount - iCount ) > iStart )
		iCount = iColumnCount - iStart;

	for ( uint16 i = 0; i < iCount; ++i )
	{
		arColumnElements[ iStart + i ] = arColumnElements[ iStart + iCount + i ];
		arColumnWidths[ iStart + i ] = arColumnWidths[ iStart + iCount + i ];
	}

	SetColumnCount( iColumnCount - iCount );	
	InvalidateLayout();		
}


float UKUIListRowColumnContainer::GetColumnWidth( uint16 iColumn ) const
{
	if ( iColumn >= iColumnCount )
	{
		KUIErrorUO( "Invalid column: %d", iColumn );
		return 0.f;
	}

	return arColumnWidths[ iColumn ];
}


void UKUIListRowColumnContainer::SetColumnWidth( uint16 iColumn, float fHeight )
{
	if ( iColumn >= iColumnCount )
	{
		KUIErrorUO( "Invalid column: %d", iColumn );
		return;
	}

	arColumnWidths[ iColumn ] = fHeight;
	
	InvalidateLayout();
}


float UKUIListRowColumnContainer::GetColumnSpacing() const
{
	return fSpacing;
}


void UKUIListRowColumnContainer::SetColumnSpacing( float fSpacing )
{
	if ( this->fSpacing == fSpacing )
		return;

	this->fSpacing = fSpacing;

	InvalidateLayout();
}


void UKUIListRowColumnContainer::DoLayout()
{
	if ( iColumnCount > 0 )
	{
		float fRelativeWidth = GetSize().X;
		float fRelativeTotal = 0.f;

		if ( iColumnCount > 1 )
			fRelativeWidth -= fSpacing * ( iColumnCount - 1 );

		for ( int32 i = 0; i < arColumnWidths.Num(); ++i )
		{
			if ( arColumnWidths[ i ] <= 0.f )
				continue;

			if ( arColumnWidths[ i ] >= 1.f )
				fRelativeWidth -= arColumnWidths[ i ];

			else
				fRelativeTotal += arColumnWidths[ i ];
		}

		// If we run out of space, all the relatively sized columns will be hidden.
		if ( fRelativeWidth < 0.f )
			fRelativeWidth = 0.f;

		TArray<float> arColumnEnds;
		float fOffset;

		arColumnEnds.SetNum( arColumnWidths.Num() );
		fOffset = 0.f;

		for ( int32 i = 0; i < arColumnWidths.Num(); ++i )
		{
			// "Hidden" columns
			if ( arColumnWidths[ i ] <= 0.f )
			{
				arColumnEnds[ i ] = fOffset;
				continue;
			}

			if ( arColumnWidths[ i ] >= 1.f )
				fOffset += arColumnWidths[ i ];

			else
				fOffset += ( arColumnWidths[ i ] / fRelativeTotal ) * fRelativeWidth;

			fOffset = floor( fOffset );
			arColumnEnds[ i ] = fOffset;
		}

		FVector2D v2SlotLocation = FVector2D::ZeroVector;
		FVector2D v2ChildSize = FVector2D::ZeroVector;
		FVector2D v2ChildLocation = FVector2D::ZeroVector;
		FVector2D v2SlotSize = FVector2D::ZeroVector;

		UKUIListContainer* ctListContainer = Cast<UKUIListContainer>( GetContainer() );

		for ( uint16 iColumn = 0; iColumn < iColumnCount; ++iColumn )
		{
			if ( arColumnElements[ iColumn ].IsValid() )
			{
				if ( iColumn > 0 )
					v2SlotLocation.X = arColumnEnds[ iColumn - 1 ];

				v2ChildSize = arColumnElements[ iColumn ]->GetSize();
				v2SlotSize.X = arColumnEnds[ iColumn ] - ( iColumn == 0 ? 0.f : arColumnEnds[ iColumn - 1 ] );

				if ( ctListContainer != NULL )
					v2SlotSize.Y = clamp( v2ChildSize.Y, ctListContainer->GetMinimumRowHeight(), ctListContainer->GetMaximumRowHeight() );

				else
					v2SlotSize.Y = v2ChildSize.Y;

				v2ChildLocation = arColumnElements[ iColumn ]->CalculateAlignLocation(
					v2SlotLocation,
					v2SlotSize,
					v2ChildSize,
					v2SlotLocation
				);

				if ( v2ChildLocation.X < 0.f )
					v2ChildLocation.X = 0.f;

				if ( v2ChildLocation.Y <= 0.f )
					v2ChildLocation.Y = 0.f;

				arColumnElements[ iColumn ]->SetLocation( v2ChildLocation.X, v2ChildLocation.Y );

				//arColumnElements[ iColumn ]->SetLocationStruct( v2SlotLocation );
			}
		}
	}

	Super::DoLayout();
}


void UKUIListRowColumnContainer::OnChildSizeChange( const FKUIInterfaceContainerElementEvent& stEventInfo )
{
	Super::OnChildSizeChange( stEventInfo );

	InvalidateLayout();
}


uint16 UKUIListRowColumnContainer::GetColumnElementIndexByRef( UKUIInterfaceElement* oElement ) const
{
	for ( int32 i = 0; i < arColumnElements.Num(); ++i )
	{
		if ( arColumnElements[ i ].Get() != oElement )
			continue;

		return i;
	}

	return KUI_LIST_INDEX_NONE;
}
