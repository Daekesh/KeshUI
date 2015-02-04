// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.


#include "KeshUI.h"
#include "Container/KUIPagedContainer.h"


UKUIPagedContainer::UKUIPagedContainer( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	iActivePage = KUI_PAGE_NONE;
	arPages.SetNum( 0 );
}


uint8 UKUIPagedContainer::AddPage( UKUIInterfaceElement* oPage )
{
	if ( oPage == NULL )
	{
		KUIErrorUO( "Trying to add a null page" );
		return KUI_PAGE_NONE;
	}

	AddChild( oPage );

	for ( uint8 i = 0; i < arPages.Num(); ++i )
	{
		if ( arPages[ i ].Get() != oPage )
			continue;

		return i;
	}

	for ( uint8 i = 0; i < arPages.Num(); ++i )
	{
		if ( arPages[ i ].IsValid() )
			continue;

		arPages[ i ] = oPage;
		oPage->SetVisible( false );
		return i;
	}

	if ( arPages.Num() < KUI_PAGE_MAX )
	{
		arPages.Add( oPage );
		oPage->SetVisible( false );
		return ( arPages.Num() - 1 );
	}

	KUIErrorUO( "Added too many pages to this container" );
	return KUI_PAGE_NONE;
}


void UKUIPagedContainer::RemovePageByReference( UKUIInterfaceElement* oPage, bool bRemoveChild )
{
	if ( oPage == NULL )
	{
		KUIErrorDebugUO( "Trying to remove a null page" );
		return;
	}

	for ( uint8 i = 0; i < arPages.Num(); ++i )
	{
		if ( arPages[ i ].Get() != oPage )
			continue;

		RemovePageByIndex( i, bRemoveChild );
		return;
	}

	KUIErrorUO( "Page not found: %s", *oPage->GetName() );
}


void UKUIPagedContainer::RemovePageByIndex( uint8 iIndex, bool bRemoveChild )
{
	if ( !arPages.IsValidIndex( iIndex ) )
	{
		KUIErrorUO( "Invalid page index" );
		return;
	}

	if ( iActivePage == iIndex )
		SetActivePageByIndex( KUI_PAGE_NONE );

	arPages[ iIndex ] = NULL;

	if ( bRemoveChild && arPages[ iIndex ].IsValid() )
		RemoveChild( arPages[ iIndex ].Get() );
}


bool UKUIPagedContainer::RemoveChild( UKUIInterfaceElement* oChild )
{
	if ( oChild == NULL )
	{
		KUIErrorDebugUO( "Trying to remove a null child" );
		return false;
	}

	RemovePageByReference( oChild, false );
	
	return Super::RemoveChild( oChild );
}


uint8 UKUIPagedContainer::GetActivePageIndex() const
{
	return iActivePage;
}


UKUIInterfaceElement* UKUIPagedContainer::GetActivePage() const
{
	if ( iActivePage == KUI_PAGE_NONE )
		return NULL;

	return arPages[ iActivePage ].Get();
}


void UKUIPagedContainer::SetActivePageByReference( UKUIInterfaceElement* oPage )
{
	//if ( iActivePage != KUI_PAGE_NONE && arPages[ iActivePage ].Get() == oPage )
		//KUILogUO( "Already active." );

	if ( oPage == NULL )
	{
		SetActivePageByIndex( KUI_PAGE_NONE );
		return;
	}
	
	for ( uint8 i = 0; i < arPages.Num(); ++i )
	{
		if ( arPages[ i ].Get() != oPage )
			continue;

		SetActivePageByIndex( i );
		return;
	}

	KUIErrorUO( "Page not found" );
}
	

void UKUIPagedContainer::SetActivePageByIndex( uint8 iIndex )
{
	//KUILogUO( "Page: %d", iIndex );

	if ( iIndex != KUI_PAGE_NONE )
	{
		if ( !arPages.IsValidIndex( iIndex ) )
		{
			KUIErrorUO( "Invalid index: %d", iIndex );
			return;
		}

		if ( !arPages[ iIndex ].IsValid() )
		{
			KUIErrorUO( "Page index is null: %d", iIndex );
			iIndex = KUI_PAGE_NONE;
		}
	}

	if ( iActivePage == iIndex )
		return;

	KUISendEvent( FKUIInterfaceContainerPageChangeEvent, EKUIInterfaceContainerEventList::E_PageChanged, iActivePage, iIndex );

	if ( iActivePage != KUI_PAGE_NONE && arPages[ iActivePage ].IsValid() )
		arPages[ iActivePage ]->SetVisible( false );

	if ( iIndex != KUI_PAGE_NONE && arPages[ iIndex ].IsValid() )
		arPages[ iIndex ]->SetVisible( true );

	iActivePage = iIndex;
}


void UKUIPagedContainer::OnActivePageChange( uint8 iOldIndex, uint8 iNewIndex )
{

}


bool UKUIPagedContainer::RespondsToEvent( uint8 iEventID ) const
{
	if ( Super::RespondsToEvent( iEventID ) )
		return true;

	return ( iEventID == EKUIInterfaceContainerEventList::E_PageChanged );
}


void UKUIPagedContainer::SendEvent( FKUIInterfaceEvent& stEventInfo )
{
	if ( IsTemplate() )
		return;

	Super::SendEvent( stEventInfo );

	if ( stEventInfo.iEventID == EKUIInterfaceContainerEventList::E_PageChanged )
	{
		OnActivePageChange( ( ( FKUIInterfaceContainerPageChangeEvent * ) &stEventInfo )->iOldPageIndex, ( ( FKUIInterfaceContainerPageChangeEvent * ) &stEventInfo )->iNewPageIndex );
		OnActivePageChangeBP( ( ( FKUIInterfaceContainerPageChangeEvent * ) &stEventInfo )->iOldPageIndex, ( ( FKUIInterfaceContainerPageChangeEvent * ) &stEventInfo )->iNewPageIndex );
	}
}
