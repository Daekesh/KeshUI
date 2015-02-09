// Copyright 2014-2015 Matt Chapman. All Rights Reserved.


#include "KeshUI.h"
#include "Container/KUISubContainerRenderCache.h"
#include "Container/KUISubContainer.h"


UKUISubContainer::UKUISubContainer( const class FObjectInitializer& oObjectInitializer )
: Super( oObjectInitializer )
{
	v2CornerOffset = FVector2D::ZeroVector;
	v2TotalSize = FVector2D::ZeroVector;
}


FVector2D UKUISubContainer::GetCornerOffset() const
{
	return v2CornerOffset;
}


void UKUISubContainer::SetCornerOffsetStruct( const FVector2D& v2CornerOffset )
{
	SetCornerOffset( v2CornerOffset.X, v2CornerOffset.Y );
}


void UKUISubContainer::SetCornerOffset( float fX, float fY )
{
	if ( v2CornerOffset.X == fX && v2CornerOffset.Y == fY )
		return;

	v2CornerOffset.X = floor( fX );
	v2CornerOffset.Y = floor( fY );

	UpdateRenderCacheSize();
}


FVector2D UKUISubContainer::GetTotalSize() const
{
	return v2TotalSize;
}


void UKUISubContainer::SetTotalSizeStruct( const FVector2D& v2TotalSize )
{
	SetTotalSize( v2TotalSize.X, v2TotalSize.Y );
}


void UKUISubContainer::SetTotalSize( float fWidth, float fHeight )
{
	if ( v2TotalSize.X == fWidth && v2TotalSize.Y == fHeight )
		return;

	v2TotalSize.X = fWidth;
	v2TotalSize.Y = fHeight;

	const FVector2D v2Size = GetSize();
	const FVector2D v2CornerOffset = GetCornerOffset();

	if ( v2Size.X <= 0.f || v2Size.Y <= 0.f )
	{
		DisableRenderCache();
		return;
	}

	if ( v2TotalSize.X <= 0.f || v2TotalSize.Y <= 0.f )
	{
		DisableRenderCache();
		return;
	}

	if ( v2CornerOffset.X >= v2TotalSize.X && v2CornerOffset.Y >= v2TotalSize.Y )
	{
		DisableRenderCache();
		return;
	}

	EnableRenderCache();
	oRenderCache->InvalidateRenderCache();
}


void UKUISubContainer::SetSize( float fWidth, float fHeight )
{
	if ( v2Size.X == fWidth && v2Size.Y == fHeight )
		return;

	Super::SetSize( fWidth, fHeight );

	UpdateRenderCacheSize();
}


void UKUISubContainer::UpdateRenderCacheSize()
{
	if ( !IsRenderCaching() )
		return;

	FVector2D v2Size = GetSize();
	const FVector2D v2TotalSize = GetTotalSize();

	v2Size.X = min( v2Size.X, v2TotalSize.X );
	v2Size.Y = min( v2Size.Y, v2TotalSize.Y );

	if ( v2Size.X > 0.9f && v2Size.Y > 0.9f )
	{
		oRenderCache->SetSizeStruct( v2Size );
		oRenderCache->SetTextureCoordsStruct( GetCornerOffset() / v2TotalSize );
		oRenderCache->SetTextureSizeStruct( ( v2Size - GetCornerOffset() ) / v2TotalSize );
	}
}


UKUIInterfaceElement* UKUISubContainer::GetContainerFor() const
{
	return oContainerFor;
}


void UKUISubContainer::SetContainerFor( UKUIInterfaceElement* oContainerFor )
{
	this->oContainerFor = oContainerFor;
}


void UKUISubContainer::Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject )
{
	if ( oContainerFor != NULL )
		SetTotalSizeStruct( oContainerFor->GetSize() );

	if ( !HasValidLayout() )
		DoLayout();

	if ( IsRenderCaching() )
	{
		// Redraw the contents (using the canvas and origin supplied by the render cache.)
		if ( oRenderCacheObject == this )
		{
			oCanvas->Reset();

			KUISendEvent( FKUIInterfaceElementRenderEvent, EKUIInterfaceElementEventList::E_Render, oCanvas, v2Origin );

			for ( int32 i = 0; i < arChildren.Num(); ++i )
			{
				if ( arChildren[ i ] == NULL )
					continue;

				if ( !arChildren[ i ]->IsVisible() )
					continue;

				arChildren[ i ]->Render( aHud, oCanvas, FVector2D::ZeroVector, oRenderCacheObject );
			}
		}

		// Draw the cache to the output
		else
		{
			if ( !HasValidAlignLocation() )
			{
				TArray<UKUIInterfaceElement*> arAlignStack;

				CalculateAlignLocation( arAlignStack );
			}

			if ( !oRenderCache->IsRenderCacheValid() )
				oRenderCache->UpdateRenderCache( this );

			v2LastScreenRenderLocation = v2Origin + GetRenderLocation();
			oCanvas->Reset();
			oRenderCache->Render( aHud, oCanvas, v2LastScreenRenderLocation, oRenderCacheObject );

			KUISendEvent( FKUIInterfaceElementRenderEvent, EKUIInterfaceElementEventList::E_Render, oCanvas, v2Origin );
		}
	}
}


void UKUISubContainer::EnableRenderCache()
{
	if ( !IsRenderCaching() )
		oRenderCache = NewObject<UKUISubContainerRenderCache>( this );

	UpdateRenderCacheSize();
}


void UKUISubContainer::BroadcastEvent( FKUIInterfaceEvent& stEventInfo, bool bTopDown )
{
	if ( stEventInfo.iEventID == EKUIInterfaceContainerEventList::E_MouseButtonDown || stEventInfo.iEventID == EKUIInterfaceContainerEventList::E_MouseButtonUp )
	{
		// Can't use UStruct pointers... so this.
#pragma warning( disable : 4946 )

		FKUIInterfaceContainerMouseButtonEvent& stNewEventInfo = *reinterpret_cast< FKUIInterfaceContainerMouseButtonEvent*>( &stEventInfo );
		stNewEventInfo.v2Location -= GetCornerOffset();

#pragma warning( default : 4946 )

		Super::BroadcastEvent( stNewEventInfo, bTopDown );
		return;
	}

	Super::BroadcastEvent( stEventInfo, bTopDown );
}


FVector2D UKUISubContainer::GetNestedLocation( UKUIInterfaceContainer* ctRoot ) const
{
	if ( this == ctRoot )
		return -GetCornerOffset();

	if ( this->GetContainer() == NULL )
		return GetLocation() - GetCornerOffset();

	return ( this->GetContainer()->GetNestedLocation( ctRoot ) - GetCornerOffset() + GetLocation() );
}
