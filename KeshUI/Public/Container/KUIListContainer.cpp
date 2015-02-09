// Copyright 2014-2015 Matt Chapman. All Rights Reserved.


#include "KeshUI.h"
#include "Container/KUIListRowContainer.h"
#include "KUIInterface.h"
#include "Widget/KUISimpleClickWidget.h"
#include "Container/KUIListRowColumnContainer.h"
#include "Container/KUIListContainer.h"


UKUIListContainer::UKUIListContainer( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	arRows.SetNum( 0 );
	fMinHeight = 0.f;
	fMaxHeight = 0.f;
	fSpacing = 0.f;
	iRowCount = 0;
	bEnableSelect = false;
	bEnableMultiSelect = false;
	ctLastSelected = NULL;
	arSelectedRows.SetNum( 1 );
	dgSelectionChange.Unbind();

	KUICreateDefaultSubobjectAssign( cmClickArea, UKUISimpleClickWidget, "Clickable Area" );
	cmClickArea->FillContainer();
	cmClickArea->SetZIndex( 0 );
	cmClickArea->SetClickDelegate( this, static_cast<FKUISimpleClickWidgetClickPrototype>( &UKUIListContainer::OnClick ) );

	AddChild( cmClickArea );
}


bool UKUIListContainer::IsSelectEnabled() const
{
	return bEnableSelect;
}


void UKUIListContainer::SetSelectEnabled( bool bEnabled )
{
	if ( bEnableSelect == bEnabled )
		return;

	bEnableSelect = bEnabled;

	if ( !bEnabled )
		SetSelectedRowByRef( NULL );
}


UKUIListRowContainer* UKUIListContainer::GetRow( uint16 iRow ) const
{
	if ( iRow >= iRowCount )
	{
		KUIErrorUO( "Invalid row: %d", iRow );
		return NULL;
	}

	return arRows[ iRow ].Get();
}


void UKUIListContainer::SetRow( uint16 iRow, UKUIListRowContainer* ctRow )
{
	if ( iRow >= iRowCount )
	{
		KUIErrorUO( "Invalid row: %d", iRow );
		return;
	}

	if ( arRows[ iRow ].Get() == ctRow )
		return;

	// Make sure it's not in the row array somewhere else
	if ( ctRow != NULL )
		RemoveRowByRef( ctRow );

	UKUIListRowContainer* const ctCurrent = arRows[ iRow ].Get();
	arRows[ iRow ] = NULL;

	if ( ctCurrent != NULL )
		RemoveChild( ctCurrent );

	if ( ctRow != NULL )
	{
		AddChild( ctRow );

		// Only update rows is this didn't fail
		if ( ctRow->GetContainer() == this )
		{
			arRows[ iRow ] = ctRow;
			arRows[ iRow ]->InvalidateAlignLocation();
		}
	}

	InvalidateLayout();
}


void UKUIListContainer::RemoveRowByRef( UKUIListRowContainer* ctRow )
{
	RemoveRowByIndex( GetRowIndexByRef( ctRow ) );
}


void UKUIListContainer::RemoveRowByIndex( uint16 iRow )
{
	if ( iRow == KUI_LIST_INDEX_NONE )
	{
		KUIErrorDebugUO( "Trying to remove invalid row index" );
		return;
	}

	SetRow( iRow, NULL );
}


void UKUIListContainer::AddChild( UKUIInterfaceElement* oChild )
{
	UKUIListRowContainer* ctRow = Cast<UKUIListRowContainer>( oChild );

	if ( ctRow != NULL )
		ctRow->UpdateSelected( false );

	Super::AddChild( oChild );
}


bool UKUIListContainer::RemoveChild( UKUIInterfaceElement* oChild )
{
	UKUIListRowContainer* ctRow = Cast<UKUIListRowContainer>( oChild );

	if ( ctRow != NULL )
	{
		RemoveSelectedRowByRef( ctRow );
		RemoveRowByRef( ctRow );
	}

	return Super::RemoveChild( oChild );
}


uint16 UKUIListContainer::GetRowCount() const
{
	return iRowCount;
}


uint8 UKUIListContainer::AddRow( UKUIListRowContainer* ctRow )
{
	SetRowCount( GetRowCount() + 1 );
	SetRow( GetRowCount() - 1, ctRow );

	return ( GetRowCount() - 1 );
}


UKUIListRowColumnContainer* UKUIListContainer::AddRowColumns( const TArray<float>& arWidths )
{
	KUINewObject( ctNewRow, UKUIListRowColumnContainer );

	for ( int32 i = 0; i < arWidths.Num(); ++i )
		ctNewRow->AddColumn( arWidths[ i ] );

	AddRow( ctNewRow );

	return ctNewRow;
}


void UKUIListContainer::SetRowCount( uint16 iRowCount )
{
	if ( this->iRowCount == iRowCount )
		return;

	int32 iCurrentRowCount = this->iRowCount;

	if ( iCurrentRowCount < iRowCount )
	{
		this->iRowCount = iRowCount;
		arRows.SetNum( iRowCount );

		for ( int32 i = iCurrentRowCount; i < iRowCount; ++i )
			SetRow( i, NULL );
	}

	else
	{
		for ( int32 i = iCurrentRowCount - 1; i >= iRowCount; --i )
			SetRow( i, NULL );

		this->iRowCount = iRowCount;
		arRows.SetNum( iRowCount );
	}

	InvalidateLayout();
}


void UKUIListContainer::RemoveRows( uint16 iStart, uint16 iCount )
{
	if ( iCount == 0 )
		return;

	if ( iStart >= iRowCount )
		return;

	if ( ( iRowCount - iCount ) > iStart )
		iCount = iRowCount - iStart;

	for ( uint16 i = 0; i < iCount; ++i )
	{
		UKUIListRowContainer* ctTemp = arRows[ iStart + i ].Get();
		arRows[ iStart + i ] = arRows[ iStart + iCount + i ];
		arRows[ iStart + iCount + i ] = ctTemp;
	}

	SetRowCount( iRowCount - iCount );	
	InvalidateLayout();		
}


float UKUIListContainer::GetMinimumRowHeight() const
{
	return fMinHeight;
}


void UKUIListContainer::SetMinimumRowHeight( float fHeight )
{
	if ( fMinHeight == fHeight )
		return;

	fMinHeight = fHeight;

	InvalidateLayout();
}


float UKUIListContainer::GetMaximumRowHeight() const
{
	return fMaxHeight;
}


void UKUIListContainer::SetMaximumRowHeight( float fHeight )
{
	if ( fMaxHeight == fHeight )
		return;

	fMaxHeight = fHeight;

	InvalidateLayout();
}


void UKUIListContainer::SetRowHeight( float fHeight )
{
	SetMinimumRowHeight( fHeight );
	SetMaximumRowHeight( fHeight );
}


float UKUIListContainer::GetRowSpacing() const
{
	return fSpacing;
}


void UKUIListContainer::SetRowSpacing( float fSpacing )
{
	if ( this->fSpacing == fSpacing )
		return;

	this->fSpacing = fSpacing;

	InvalidateLayout();
}


void UKUIListContainer::SetSize( float fWidth, float fHeight )
{
	if ( v2Size.X == fWidth && v2Size.Y == fHeight )
		return;
	
	Super::SetSize( fWidth, fHeight );

	InvalidateLayout();
}


void UKUIListContainer::DoLayout()
{
	if ( iRowCount >= 0 )
	{
		FVector2D v2SlotLocation = FVector2D::ZeroVector;

		for ( uint16 iRow = 0; iRow < iRowCount; ++iRow )
		{
			float fRowHeight = 0.f;

			TArray<UKUIInterfaceElement*> arAlignStack;

			if ( arRows[ iRow ].IsValid() )
			{
				if ( !arRows[ iRow ]->IsVisible() )
					continue;

				arRows[ iRow ]->InvalidateAlignLocation();
				arRows[ iRow ]->CalculateAlignLocation( arAlignStack );
				fRowHeight += arRows[ iRow ]->GetSize().Y;
			}

			if ( fMinHeight > 0.f && fRowHeight < fMinHeight )
				fRowHeight = fMinHeight;

			if ( fMaxHeight > 0.f && fRowHeight > fMaxHeight )
				fRowHeight = fMaxHeight;

			if ( arRows[ iRow ].IsValid() )
				arRows[ iRow ]->SetLocationStruct( v2SlotLocation );

			v2SlotLocation.Y += fRowHeight + fSpacing;
		}

		v2SlotLocation.Y -= fSpacing;
		SetSize( GetSize().X, v2SlotLocation.Y );
	}

	Super::DoLayout();
}


void UKUIListContainer::OnChildSizeChange( const FKUIInterfaceContainerElementEvent& stEventInfo )
{
	Super::OnChildSizeChange( stEventInfo );

	InvalidateLayout();
}


uint16 UKUIListContainer::GetRowIndexByRef( UKUIListRowContainer* ctRow ) const
{
	for ( int32 i = 0; i < arRows.Num(); ++i )
	{
		if ( arRows[ i ].Get() != ctRow )
			continue;

		return i;
	}

	return KUI_LIST_INDEX_NONE;
}


uint16 UKUIListContainer::GetSelectedRowIndexByRef( UKUIListRowContainer* ctRow ) const
{
	if ( ctRow == NULL )
		return KUI_LIST_INDEX_NONE;

	if ( ctRow->GetContainer() != this )
		return KUI_LIST_INDEX_NONE;

	for ( uint16 i = 0; i < arSelectedRows.Num(); ++i )
	{
		if ( arSelectedRows[ i ].Get() != ctRow )
			continue;

		return i;
	}

	return KUI_LIST_INDEX_NONE;
}


bool UKUIListContainer::IsRowSelected( UKUIListRowContainer* ctRow ) const
{
	if ( ctRow == NULL )
		return false;

	if ( ctRow->GetContainer() != this )
		return false;

	return ctRow->IsSelected();
}


void UKUIListContainer::OnRowSelectionClick( UKUIListRowContainer* ctRow )
{
	if ( !bEnableSelect )
		return;

	if ( GetInterface() == NULL )
		return;

	APlayerController* const aPlayerController = GetInterface()->GetPlayerController();

	if ( aPlayerController == NULL )
		return;

	if ( !IsMultiSelectEnabled() )
		SetSelectedRowByRef( ctRow );
	
	else
	{
		uint16 iLastSelectedIndex = KUI_LIST_INDEX_NONE;

		if ( ctLastSelected.Get() != NULL )
			iLastSelectedIndex = GetRowIndexByRef( ctLastSelected.Get() );

		if ( aPlayerController->IsInputKeyDown( EKeys::LeftControl ) )
		{
			// Unselect single row and reset history
			if ( ctRow != NULL && ctRow->IsSelected() )
			{
				RemoveSelectedRowByRef( ctRow );
				iLastSelectedIndex = KUI_LIST_INDEX_NONE;
				ctLastSelected = NULL;
				return;
			}

			else
				AddSelectedRowByRef( ctRow );
		}

		else if ( aPlayerController->IsInputKeyDown( EKeys::LeftShift ) )
		{
			if ( iLastSelectedIndex == KUI_LIST_INDEX_NONE )
				SetSelectedRowByRef( ctRow );

			else if ( ctRow != NULL )
			{
				uint16 iClickedIndex = GetRowIndexByRef( ctRow );

				if ( iClickedIndex == KUI_LIST_INDEX_NONE )
					SetSelectedRowByRef( ctRow );

				else if ( iClickedIndex == iLastSelectedIndex )
					return;

				else
				{
					TArray<UKUIListRowContainer*> arRows;
					arRows.SetNum( abs( iClickedIndex - iLastSelectedIndex ) );

					if ( iClickedIndex > iLastSelectedIndex )
					{
						for ( int32 i = iLastSelectedIndex + 1; i <= iClickedIndex; ++i )
							arRows[ i - iLastSelectedIndex - 1 ] = this->arRows[ i ].Get();
					}

					else
					{
						for ( int32 i = iClickedIndex; i < iLastSelectedIndex; ++i )
							arRows[ i - iClickedIndex ] = this->arRows[ i ].Get();
					}

					AddSelectedRowsByRef( arRows );
				}
			}
		}

		else
			SetSelectedRowByRef( ctRow );
	}

	ctLastSelected = ctRow;
}


bool UKUIListContainer::IsMultiSelectEnabled() const
{
	return bEnableMultiSelect;
}


void UKUIListContainer::SetMultiSelectEnabled( bool bEnabled )
{
	if ( bEnableMultiSelect == bEnabled )
		return;

	bEnableMultiSelect = bEnabled;

	if ( !bEnableMultiSelect && arSelectedRows.Num() > 1 )
	{
		for ( int32 i = 1; i < arSelectedRows.Num(); ++i )
		{
			if ( arSelectedRows[ i ].Get() == NULL )
				continue;

			arSelectedRows[ i ]->UpdateSelected( false );
		}

		arSelectedRows.SetNum( 1 );

		dgSelectionChange.ExecuteIfBound( this );
	}
}


UKUIListRowContainer* UKUIListContainer::GetSelectedRow() const
{
	return arSelectedRows[ 0 ].Get();
}


const TArray<TWeakObjectPtr<UKUIListRowContainer>>& UKUIListContainer::GetSelectedRows() const
{
	return arSelectedRows;
}


bool UKUIListContainer::IsRowSelectedByRef( UKUIListRowContainer* ctRow ) const
{
	if ( ctRow == NULL )
		return false;

	if ( ctRow->GetContainer() != this )
		return false;

	return ctRow->IsSelected();
}


bool UKUIListContainer::IsRowSelectedByIndex( uint16 iRow ) const
{
	return IsRowSelectedByRef( arRows.IsValidIndex( iRow ) ? arRows[ iRow ].Get() : NULL );
}


bool UKUIListContainer::AreRowsSelectedByRef( const TArray<UKUIListRowContainer*>& arRows ) const
{
	for ( int32 i = 0; i < arRows.Num(); ++i )
	{
		if ( IsRowSelectedByRef( arRows[ i ] ) )
			continue;

		return false;
	}

	return true;
}


bool UKUIListContainer::AreRowsSelectedByIndex( const TArray<uint16>& arRows ) const
{
	for ( int32 i = 0; i < arRows.Num(); ++i )
	{
		if ( IsRowSelectedByIndex( arRows[ i ] ) )
			continue;

		return false;
	}

	return true;
}


void UKUIListContainer::SetSelectedRowByRef( UKUIListRowContainer* ctRow )
{
	for ( int i = 0; i < arSelectedRows.Num(); ++i )
	{
		if ( arSelectedRows[ i ].Get() == NULL )
		{
			arSelectedRows[ i ].Reset();
			continue;
		}

		if ( arSelectedRows[ i ].Get() == ctRow )
			continue;

		arSelectedRows[ i ]->UpdateSelected( false );
	}

	arSelectedRows.SetNum( 1 );
	arSelectedRows[ 0 ] = ctRow;

	if ( ctRow != NULL )
		ctRow->UpdateSelected( true );

	dgSelectionChange.ExecuteIfBound( this );
}


void UKUIListContainer::SetSelectedRowByIndex( uint16 iRow )
{
	SetSelectedRowByRef( arRows.IsValidIndex( iRow ) ? arRows[ iRow ].Get() : NULL );
}


void UKUIListContainer::SetSelectedRowsByRef( const TArray<UKUIListRowContainer*>& arRows )
{
	// Unselect all the previously selected rows... if they aren't in the new array
	for ( int32 i = 0; i < arSelectedRows.Num(); ++i )
	{
		if ( arSelectedRows[ i ].Get() == NULL )
			continue;

		bool bFound = false;

		for ( int32 j = 0; j < arRows.Num(); ++j )
		{
			if ( arRows[ j ] != arSelectedRows[ i ].Get() )
				continue;

			bFound = true;
			break;
		}

		if ( !bFound )
			arSelectedRows[ i ]->UpdateSelected( false );
	}

	arSelectedRows.SetNum( clamp( arRows.Num(), 1, IsMultiSelectEnabled() ? KUI_LIST_INDEX_MAX : 1 ) );
	uint16 iRow = 0;

	// Select all the new rows, if they are valid
	for ( int32 i = 0; i < arRows.Num(); ++i )
	{
		if ( arRows[ i ] == NULL )
			continue;

		if ( arRows[ i ]->GetContainer() != this )
			continue;

		this->arSelectedRows[ iRow ] = arRows[ i ];
		this->arSelectedRows[ iRow ]->UpdateSelected( true );
		
		++iRow;

		if ( iRow >= arSelectedRows.Num() )
			break;
	}

	// Null out the rest
	for ( int32 i = iRow; i < arSelectedRows.Num(); ++i )
		arSelectedRows[ i ].Reset();

	dgSelectionChange.ExecuteIfBound( this );
}


void UKUIListContainer::SetSelectedRowsByIndex( const TArray<uint16>& arRows )
{
	TArray<UKUIListRowContainer*> arRefList;
	arRefList.SetNum( arRows.Num() );

	for ( int32 i = 0; i < arRows.Num(); ++i )
		arRefList[ i ] = ( arRows.IsValidIndex( i ) ? this->arRows[ i ].Get() : NULL );

	SetSelectedRowsByRef( arRefList );
}


bool UKUIListContainer::AddSelectedRowByRef( UKUIListRowContainer* ctRow )
{
	if ( ctRow == NULL )
		return false;

	if ( ctRow->GetContainer() != this )
		return false;
	
	uint16 iSelectedCount = 0;

	for ( int32 i = 0; i < arSelectedRows.Num(); ++i )
	{
		if ( arSelectedRows[ i ].Get() == NULL )
			continue;

		if ( arSelectedRows[ i ].Get() == ctRow )
			return true;

		++iSelectedCount;
	}

	if ( !IsMultiSelectEnabled() && iSelectedCount > 0 )
		return false;

	for ( int32 i = 0; i < arSelectedRows.Num(); ++i )
	{
		if ( arSelectedRows[ i ].Get() != NULL )
			continue;

		arSelectedRows[ i ] = ctRow;
		ctRow->UpdateSelected( true );
		dgSelectionChange.ExecuteIfBound( this );
		return true;
	}

	arSelectedRows.Add( TWeakObjectPtr<UKUIListRowContainer>( ctRow ) );
	ctRow->UpdateSelected( true );
	dgSelectionChange.ExecuteIfBound( this );
	return true;
}


bool UKUIListContainer::AddSelectedRowByIndex( uint16 iRow )
{
	return AddSelectedRowByRef( arRows.IsValidIndex( iRow ) ? this->arRows[ iRow ].Get() : NULL );
}


bool UKUIListContainer::AddSelectedRowsByRef( const TArray<UKUIListRowContainer*>& arRows )
{
	bool bAddedAtLeastOne = false;

	for ( int32 i = 0; i < arRows.Num(); ++i )
	{
		if ( !AddSelectedRowByRef( arRows[ i ] ) )
			continue;

		bAddedAtLeastOne = true;

		if ( !IsMultiSelectEnabled() )
			return true;
	}

	return bAddedAtLeastOne;
}


bool UKUIListContainer::AddSelectedRowsByIndex( const TArray<uint16>& arRows )
{
	TArray<UKUIListRowContainer*> arRefList;
	arRefList.SetNum( arRows.Num() );

	for ( int32 i = 0; i < arRows.Num(); ++i )
		arRefList[ i ] = ( arRows.IsValidIndex( i ) ? this->arRows[ i ].Get() : NULL );

	return AddSelectedRowsByRef( arRefList );
}


void UKUIListContainer::RemoveSelectedRowByRef( UKUIListRowContainer* ctRow )
{
	if ( ctRow == NULL )
		return;

	TArray<TWeakObjectPtr<UKUIListRowContainer>> arNewSelectedRows;
	arNewSelectedRows.SetNum( max( 1, arSelectedRows.Num() ) );
	uint16 iRow = 0;

	for ( int32 i = 0; i < arSelectedRows.Num(); ++i )
	{
		if ( arSelectedRows[ i ].Get() == NULL )
			continue;

		if ( arSelectedRows[ i ].Get() == ctRow )
			continue;

		arNewSelectedRows[ iRow ] = arSelectedRows[ i ].Get();
		++iRow;
	}

	for ( int32 i = iRow; i < arNewSelectedRows.Num(); ++i )
		arNewSelectedRows[ i ].Reset();

	arSelectedRows = arNewSelectedRows;

	dgSelectionChange.ExecuteIfBound( this );
}


void UKUIListContainer::RemoveSelectedRowByIndex( uint16 iRow )
{
	RemoveSelectedRowByRef( arRows.IsValidIndex( iRow ) ? this->arRows[ iRow ].Get() : NULL );
}


void UKUIListContainer::RemoveSelectedRowsByRef( const TArray<UKUIListRowContainer*>& arRows )
{
	TArray<TWeakObjectPtr<UKUIListRowContainer>> arNewSelectedRows;
	arNewSelectedRows.SetNum( max( 1, arSelectedRows.Num() ) );
	uint16 iRow = 0;

	for ( int32 i = 0; i < arSelectedRows.Num(); ++i )
	{
		if ( arSelectedRows[ i ].Get() == NULL )
			continue;

		bool bFound = false;

		for ( int32 j = 0; j < arRows.Num(); ++j )
		{
			if ( arRows[ j ] == NULL )
				continue;

			if ( arSelectedRows[ i ].Get() != arRows[ j ] )
				continue;
				
			bFound = true;
			break;
		}

		if ( bFound )
			continue;

		arNewSelectedRows[ iRow ] = arSelectedRows[ i ].Get();
		++iRow;
	}

	for ( int32 i = iRow; i < arNewSelectedRows.Num(); ++i )
		arNewSelectedRows[ i ].Reset();

	arSelectedRows = arNewSelectedRows;

	dgSelectionChange.ExecuteIfBound( this );
}


void UKUIListContainer::RemoveSelectedRowsByIndex( const TArray<uint16>& arRows )
{
	TArray<UKUIListRowContainer*> arRefList;
	arRefList.SetNum( arRows.Num() );

	for ( int32 i = 0; i < arRows.Num(); ++i )
		arRefList[ i ] = ( arRows.IsValidIndex( i ) ? this->arRows[ i ].Get() : NULL );

	RemoveSelectedRowsByRef( arRefList );
}


void UKUIListContainer::OnClick( UKUISimpleClickWidget* oWidget, const FVector2D& v2CursorOffset )
{
	if ( !IsSelectEnabled() )
		return;

	float fClickHeight = v2CursorOffset.Y;

	if ( GetMinimumRowHeight() == GetMaximumRowHeight() )
	{
		float fRowHeight = GetMinimumRowHeight();
		float fHeightFromBaseOfRow = FMath::Fmod( fClickHeight + GetRowSpacing(), fRowHeight + GetRowSpacing() );

		if ( fHeightFromBaseOfRow > fRowHeight ) // clicked blank space
		{
			OnRowSelectionClick( NULL );
			return;
		}

		int32 iRow = floor( ( fClickHeight + GetRowSpacing() ) / ( GetMinimumRowHeight() + GetRowSpacing() ) );

		if ( iRow < 0 || iRow >= arRows.Num() )
		{
			OnRowSelectionClick( NULL );
			return;
		}

		OnRowSelectionClick( arRows[ iRow ].Get() );
		return;
	}

	float fCurrentHeight = 0.f;

	for ( int32 i = 0; i < arRows.Num(); ++i )
	{
		if ( arRows[ i ].Get() == NULL )
			continue;

		fCurrentHeight = arRows[ i ]->GetLocation().Y;

		// Clicked in the row spacing
		if ( fCurrentHeight > fClickHeight )
		{
			OnRowSelectionClick( NULL );
			return;
		}

		fCurrentHeight += clamp( arRows[ i ]->GetSize().Y, GetMinimumRowHeight(), GetMaximumRowHeight() );

		if ( fCurrentHeight >= fClickHeight )
		{
			OnRowSelectionClick( arRows[ i ].Get() );
			return;
		}
	}

	OnRowSelectionClick( NULL );
}


void UKUIListContainer::SetSelectionChangeDelegate( UObject* oObject, FKUIListRowContainerSelectionChangePrototype fnSelectionChangeDelegate )
{
	if ( oObject == NULL || fnSelectionChangeDelegate == NULL )
		dgSelectionChange.Unbind();

	else
		dgSelectionChange.BindUObject( oObject, fnSelectionChangeDelegate );
}
