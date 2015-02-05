// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#include "KeshUI.h"
#include "KUIInterface.h"
#include "KUIInterfaceContainer.h"
#include "KUIRenderCache.h"
#include "KUIInterfaceElement.h"

#define _XTGMATH
#include <functional>


UKUIInterfaceElement::UKUIInterfaceElement( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	KUI_UO_INIT_DEBUG()

	bVisible = true;
	v2Location = FVector2D::ZeroVector;
	v4Margin = FVector4( 0.f, 0.f, 0.f, 0.f );
	ctContainer = NULL;
	iZIndex = 0;
	oAlignedTo = NULL;
	eHAlign = EKUIInterfaceHAlign::HA_None;
	eVAlign = EKUIInterfaceVAlign::VA_None;
	v2AlignLocation = FVector2D::ZeroVector;
	bValidAlignLocation = false;
	v2LastScreenRenderLocation = FVector2D( -1.f, -1.f ); // Invalid
	arAlignedToThis.SetNum( 0 );
	oRenderCache = NULL;
	aLastRenderedBy = NULL;

	bDebug = false;
}


AKUIInterface* UKUIInterfaceElement::GetInterface() const
{
	if ( ctContainer.IsValid() )
	{
		AKUIInterface* const aInterface = ctContainer->GetInterface();

		if ( aInterface != NULL )
			return aInterface;
	}

	if ( aLastRenderedBy != NULL )
		return aLastRenderedBy.Get();

	return AKUIInterface::GetLatestInstance();
}


UKUIInterfaceContainer* UKUIInterfaceElement::GetContainer() const
{
	return ctContainer.Get();
}


UObject** UKUIInterfaceElement::GetAsset( const FName& nName ) const
{
	if ( IsTemplate() )
		return NULL;

	AKUIInterface* aInterface = GetInterface();

	if ( aInterface == NULL )
		aInterface = AKUIInterface::GetLatestInstance();

	if ( aInterface == NULL )
		return NULL;

	return aInterface->GetAsset( nName );
}


template<class T>
T** UKUIInterfaceElement::GetAsset( const FName& nName ) const
{
	return ( T** ) GetAsset( nName );
}


bool UKUIInterfaceElement::IsVisible() const
{
	return bVisible;
}


UWorld* UKUIInterfaceElement::GetWorld() const
{
	AKUIInterface* const aInterface = GetInterface();

	if ( aInterface == NULL )
		return NULL;

	return aInterface->GetWorld();
}


void UKUIInterfaceElement::SetVisible( bool bVisible )
{
	this->bVisible = bVisible;

	InvalidateContainerRenderCache();
}


bool UKUIInterfaceElement::IsVisibleRecursive() const
{
	if ( !IsVisible() )
		return false;

	if ( GetContainer() == NULL )
		return false;

	return GetContainer()->IsVisibleRecursive();
}


bool UKUIInterfaceElement::IsDisabled() const
{
	return false;
}


bool UKUIInterfaceElement::IsDisabledRecursive() const
{
	if ( IsDisabled() )
		return true;

	if ( GetContainer() == NULL )
		return false;

	return GetContainer()->IsDisabledRecursive();
}


FVector2D UKUIInterfaceElement::GetLocation() const
{
	return v2Location;
}


void UKUIInterfaceElement::SetLocationStruct( const FVector2D& v2Location )
{
	SetLocation( v2Location.X, v2Location.Y );
}


void UKUIInterfaceElement::SetLocation( float fX, float fY )
{
	if ( v2Location.X == fX && v2Location.Y == fY )
		return;

	FKUIInterfaceContainerLocationChangeEvent stEventInfo( EKUIInterfaceElementEventList::E_LocationChange, GetLocation(), FVector2D( fX, fY ) );
	SendEvent( stEventInfo );

	v2Location.X = fX;
	v2Location.Y = fY;

	if ( GetContainer() != NULL )
	{
		FKUIInterfaceContainerElementEvent stContainerEventInfo( EKUIInterfaceContainerEventList::E_ChildLocationChange, this );
		GetContainer()->SendEvent( stContainerEventInfo );
	}

	for ( int32 i = 0; i < arAlignedToThis.Num(); ++i )
		if ( arAlignedToThis[ i ].IsValid() )
			arAlignedToThis[ i ]->InvalidateAlignLocation();

	InvalidateContainerRenderCache();
}


FVector4 UKUIInterfaceElement::GetMargin() const
{
	return v4Margin;
}


void UKUIInterfaceElement::SetMarginStruct( const FVector4& v4Margin )
{
	SetMargin( v4Margin.X, v4Margin.Y, v4Margin.Z, v4Margin.W );
}


void UKUIInterfaceElement::SetMargin( float fLeft, float fTop, float fRight, float fBottom )
{
	if ( v4Margin.X == fLeft && v4Margin.Y == fTop && v4Margin.Z == fRight && v4Margin.W == fBottom )
		return;

	v4Margin.X = fLeft;
	v4Margin.Y = fTop;
	v4Margin.Z = fRight;
	v4Margin.W = fBottom;

	if ( GetContainer() != NULL )
	{
		FKUIInterfaceContainerElementEvent stEventInfo( EKUIInterfaceContainerEventList::E_ChildLocationChange, this );
		GetContainer()->SendEvent( stEventInfo );
	}

	for ( int32 i = 0; i < arAlignedToThis.Num(); ++i )
		if ( arAlignedToThis[ i ].IsValid() )
			arAlignedToThis[ i ]->InvalidateAlignLocation();

	// Moves the child... possibly.
	InvalidateContainerRenderCache();
}


FVector2D UKUIInterfaceElement::GetSize() const
{
	return FVector2D::ZeroVector;
}


void UKUIInterfaceElement::SetSize( float fWidth, float fHeight )
{
	for ( int32 i = 0; i < arAlignedToThis.Num(); ++i )
		if ( arAlignedToThis[ i ].IsValid() )
			arAlignedToThis[ i ]->InvalidateAlignLocation();

	if ( GetContainer() != NULL )
	{
		FKUIInterfaceContainerElementEvent stEventInfo( EKUIInterfaceContainerEventList::E_ChildSizeChange, this );
		GetContainer()->SendEvent( stEventInfo );
	}

	InvalidateRenderCache();
}


FVector2D UKUIInterfaceElement::GetMarginSize() const
{
	return FVector2D(
		v4Margin.X + v4Margin.Z,
		v4Margin.Y + v4Margin.W
	);
}


FVector2D UKUIInterfaceElement::GetMarginOffset() const
{
	return FVector2D(
		v4Margin.X,
		v4Margin.Y
	);
}


UKUIInterfaceElement* UKUIInterfaceElement::GetAlignedTo() const
{
	return oAlignedTo.Get();
}


void UKUIInterfaceElement::SetAlignedTo( UKUIInterfaceElement* oAlignedTo )
{
	if ( this->oAlignedTo.Get() == oAlignedTo )
		return;

	if ( this->oAlignedTo.IsValid() )
		this->oAlignedTo->RemoveAlignedToThis( this );

	this->oAlignedTo = oAlignedTo;

	if ( this->oAlignedTo.IsValid() )
		this->oAlignedTo->AddAlignedToThis( this );

	InvalidateAlignLocation();

	if ( GetContainer() != NULL )
	{
		FKUIInterfaceContainerElementEvent stEventInfo( EKUIInterfaceContainerEventList::E_ChildLocationChange, this );
		GetContainer()->SendEvent( stEventInfo );
	}
}


TEnumAsByte<EKUIInterfaceHAlign::Type> UKUIInterfaceElement::GetHorizontalAlignment() const
{
	return eHAlign;
}


void UKUIInterfaceElement::SetHorizontalAlignment( TEnumAsByte<EKUIInterfaceHAlign::Type> eHAlign )
{
	if ( this->eHAlign == eHAlign )
		return;

	this->eHAlign = eHAlign;

	InvalidateAlignLocation();

	if ( GetContainer() != NULL )
	{
		FKUIInterfaceContainerElementEvent stEventInfo( EKUIInterfaceContainerEventList::E_ChildLocationChange, this );
		GetContainer()->SendEvent( stEventInfo );
	}
}


TEnumAsByte<EKUIInterfaceVAlign::Type> UKUIInterfaceElement::GetVerticalAlignment() const
{
	return eVAlign;
}


void UKUIInterfaceElement::SetVerticalAlignment( TEnumAsByte<EKUIInterfaceVAlign::Type> eVAlign )
{
	if ( this->eVAlign == eVAlign )
		return;

	this->eVAlign = eVAlign;

	InvalidateAlignLocation();

	if ( GetContainer() != NULL )
	{
		FKUIInterfaceContainerElementEvent stEventInfo( EKUIInterfaceContainerEventList::E_ChildLocationChange, this );
		GetContainer()->SendEvent( stEventInfo );
	}
}


void UKUIInterfaceElement::FillContainer()
{
	SetHorizontalAlignment( EKUIInterfaceHAlign::HA_Fill );
	SetVerticalAlignment( EKUIInterfaceVAlign::VA_Fill );
}


bool UKUIInterfaceElement::HasValidAlignLocation() const
{
	return bValidAlignLocation;
}


void UKUIInterfaceElement::InvalidateAlignLocation()
{
	bValidAlignLocation = false;

	for ( int32 i = 0; i < arAlignedToThis.Num(); ++i )
		if ( arAlignedToThis[ i ].IsValid() )
			arAlignedToThis[ i ]->InvalidateAlignLocation();

	FKUIInterfaceEvent stEventInfo( EKUIInterfaceElementEventList::E_AlignLocationInvalidated );
	SendEvent( stEventInfo );
}


void UKUIInterfaceElement::SetAlignLocation( const FVector2D& v2AlignLocation )
{
	this->v2AlignLocation = v2AlignLocation;
	bValidAlignLocation = true;

	FKUIInterfaceEvent stEventInfo( EKUIInterfaceElementEventList::E_AlignLocationCalculated );
	SendEvent( stEventInfo );
}


FVector2D UKUIInterfaceElement::GetRenderLocation()
{
	if ( !bValidAlignLocation )
	{
		TArray<UKUIInterfaceElement*> arAlignStack;

		CalculateAlignLocation( arAlignStack );
	}

	return ( v2AlignLocation + v2Location );
}


uint16 UKUIInterfaceElement::GetZIndex() const
{
	return iZIndex;
}


void UKUIInterfaceElement::SetZIndex( uint16 iZIndex )
{
	if ( this->iZIndex == iZIndex )
		return;

	this->iZIndex = iZIndex;

	if ( ctContainer.IsValid() )
		ctContainer->SortChildren();
}


bool UKUIInterfaceElement::IsCursorOver() const
{
	if ( GetInterface() == NULL )
	{
		KUIErrorUO( "Null interface" );
		return false;
	}

	return IsPointOver( GetInterface()->GetCursorLocation() );
}


bool UKUIInterfaceElement::IsPointOver( FVector2D v2Point ) const
{
	const FVector2D v2ScreenLocation = GetScreenLocation();
	const FVector2D v2Size = GetSize();

	if ( v2Point.X < v2ScreenLocation.X || v2Point.Y < v2ScreenLocation.Y )
		return false;

	if ( v2Point.X >= ( v2ScreenLocation.X + v2Size.X ) || v2Point.Y >= ( v2ScreenLocation.Y + v2Size.Y ) )
		return false;

	return true;
}


FVector2D UKUIInterfaceElement::GetScreenLocation() const
{
	return GetNestedLocation( NULL );
}


FVector2D UKUIInterfaceElement::GetNestedLocation( UKUIInterfaceContainer* ctRoot ) const
{
	if ( this == ctRoot )
		return FVector2D::ZeroVector;

	if ( this->GetContainer() == NULL )
		return v2AlignLocation + v2Location;

	return ( this->GetContainer()->GetNestedLocation( ctRoot ) + v2AlignLocation + v2Location );
}


void UKUIInterfaceElement::Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject )
{
	aLastRenderedBy = aHud;

	if ( !HasValidAlignLocation() )
	{
		TArray<UKUIInterfaceElement*> arAlignStack;
		CalculateAlignLocation( arAlignStack );
	}

	if ( IsRenderCaching() )
	{
		if ( oRenderCacheObject != this )
		{
			if ( !oRenderCache->IsRenderCacheValid() )
				oRenderCache->UpdateRenderCache( this );

			//v2LastScreenRenderLocation = v2Origin + GetRenderLocation()/* + oRenderCacheObject->GetLastScreenRenderLocation()*/;
			oCanvas->Reset();
			oRenderCache->Render( aHud, oCanvas, GetScreenLocation() );

			KUISendEvent( FKUIInterfaceElementRenderEvent, EKUIInterfaceElementEventList::E_Render, oCanvas, v2Origin );
		}
	}

	else
	{
		v2LastScreenRenderLocation = v2Origin + GetRenderLocation() + ( oRenderCacheObject != NULL ? oRenderCacheObject->GetLastScreenRenderLocation() : FVector2D::ZeroVector );
		oCanvas->Reset();

		KUISendEvent( FKUIInterfaceElementRenderEvent, EKUIInterfaceElementEventList::E_Render, oCanvas, v2Origin );
	}

#if KUI_INTERFACE_MOUSEOVER_DEBUG
	AKUIInterface* aInterface = GetInterface();

	if ( aInterface && aInterface->bDebugMouseOver && IsMouseOver() )
	{
		aInterface->cmDebugMouseOver = this;
		aInterface->v2DebugMouseOverLocation = GetLastScreenRenderLocation();
		aInterface->v2DebugMouseOverSize = GetSize();
	}
#endif // KUI_INTERFACE_MOUSEOVER_DEBUG
}


FVector2D UKUIInterfaceElement::GetLastScreenRenderLocation() const
{
	return v2LastScreenRenderLocation;
}


bool UKUIInterfaceElement::IsMouseOver() const
{
	if ( v2LastScreenRenderLocation.X == -1.f )
		return false;

	if ( GetInterface() == NULL )
	{
		KUIErrorUO( "Null interface" );
		return false;
	}

	if ( GetContainer() != NULL )
	{
		if ( !GetContainer()->IsMouseOver() )
			return false;
	}

	const FVector2D v2CursorLocation = GetInterface()->GetCursorLocation();
	const FVector2D v2Size = GetSize();
	const FVector2D v2TopLeftLocation = GetScreenLocation();

	if ( v2CursorLocation.X >= v2TopLeftLocation.X &&
		 v2CursorLocation.Y >= v2TopLeftLocation.Y &&
		 v2CursorLocation.X < ( v2TopLeftLocation.X + v2Size.X ) &&
		 v2CursorLocation.Y < ( v2TopLeftLocation.Y + v2Size.Y ) )
		 return true;

	return false;
}


void UKUIInterfaceElement::AddAlignedToThis( UKUIInterfaceElement* oAlignChild )
{
	// Can't remove a null pointer.
	if ( oAlignChild == NULL )
	{
		KUIErrorUO( "Trying to add a null align child" );
		return;
	}

	if ( oAlignChild == this )
	{
		KUIErrorUO( "Trying to align to self" );
		return;
	}

	if ( oAlignChild->GetAlignedTo() != this )
	{
		KUIErrorUO( "Trying to add an align child that isn't aligned to this" );
		return;
	}

	// Check if we're in the child array already.
	for ( int32 i = 0; i < arAlignedToThis.Num(); ++i )
		if ( arAlignedToThis[ i ] == oAlignChild )
			return;

	// If this child already has a container, remove it from it.
	if ( oAlignChild->GetAlignedTo() != NULL )
		oAlignChild->GetAlignedTo()->RemoveAlignedToThis( oAlignChild );

	// Add the child to our array.
	arAlignedToThis.Add( oAlignChild );
}


void UKUIInterfaceElement::RemoveAlignedToThis( UKUIInterfaceElement* oAlignChild )
{
	// Can't remove a null pointer
	if ( oAlignChild == NULL )
	{
		KUIErrorUO( "Trying to remove null align child" );
		return;
	}

	// Check if we're actually in the children array.
	int32 iIndex = INDEX_NONE;

	for ( int32 i = 0; i < arAlignedToThis.Num(); ++i )
	{
		if ( arAlignedToThis[ i ] != oAlignChild )
			continue;

		iIndex = i;
		break;
	}

	// It wasn't
	if ( iIndex == INDEX_NONE )
	{
		KUIErrorDebugUO( "Trying to remove an align child not aligned to this element" );
		return;
	}

	// Swap out the highest index with this child, to maintain contiguity.
	int32 iHighestIndex = INDEX_NONE;

	for ( int32 i = arAlignedToThis.Num() - 1; i > iIndex; --i )
	{
		if ( !arAlignedToThis[ i ].IsValid() )
			continue;

		iHighestIndex = i;
		break;
	}

	// We found a highest child, so swap.  Otherwise, we were the highest child and no sort is necessary.
	if ( iHighestIndex != INDEX_NONE )
		arAlignedToThis[ iIndex ] = arAlignedToThis[ iHighestIndex ];

	// Reduce the size of the array by 1.
	arAlignedToThis.SetNum( iIndex );
}


void UKUIInterfaceElement::CalculateAlignLocation( TArray<UKUIInterfaceElement*>& arAlignStack )
{
	if ( HasValidAlignLocation() )
		return;

	// Avoid recursive layout dependencies.
	if ( arAlignStack.Contains( this ) )
		return;
	
	if ( GetContainer() != NULL && GetContainer()->IsChildsLayoutManaged( this ) )
	{
		SetAlignLocation( FVector2D::ZeroVector );

		// Caused circular layout dependencies.
		//if ( !GetContainer()->HasValidLayout() )
			//GetContainer()->DoLayout();

		return;
	}

	UKUIInterfaceElement* oAlignedTo = GetAlignedTo();

	if ( oAlignedTo == NULL )
		oAlignedTo = GetContainer();

	if ( oAlignedTo == NULL )
	{
		SetAlignLocation( FVector2D::ZeroVector );
		return;
	}

	arAlignStack.Add( this );

	if ( !oAlignedTo->HasValidAlignLocation() )
		oAlignedTo->CalculateAlignLocation( arAlignStack );

	const FVector2D v2Size = GetSize() + GetMarginSize();
	FVector2D v2AlignLocation = FVector2D::ZeroVector;
	FVector2D v2AlignSize = FVector2D::ZeroVector;
	FVector2D v2AlignRenderLocation = FVector2D::ZeroVector;

	// If we're aligned to something in this container, use local space
	if ( oAlignedTo->GetContainer() == GetContainer() )	
	{
		v2AlignRenderLocation = oAlignedTo->GetRenderLocation() - oAlignedTo->GetMarginOffset();
		v2AlignSize = oAlignedTo->GetSize() + oAlignedTo->GetMarginSize();
	}

	// If we're aligned to something outside our container, use its screen location
	else if ( oAlignedTo != GetContainer() )
	{
		v2AlignRenderLocation = oAlignedTo->GetScreenLocation() - oAlignedTo->GetMarginOffset();
		v2AlignSize = oAlignedTo->GetSize() + oAlignedTo->GetMarginSize();
	}

	// We're aligned to the container, so use 0, 0
	else
	{
		v2AlignRenderLocation = FVector2D::ZeroVector;
		v2AlignSize = oAlignedTo->GetSize();
	}

	v2AlignLocation = CalculateAlignLocation( 
		v2AlignRenderLocation, 
		v2AlignSize, 
		v2Size
	);

	// If we're aligned to something outside of our container, move the align location back into local space.
	if ( oAlignedTo != GetContainer() && oAlignedTo->GetContainer() != GetContainer() )
		v2AlignLocation += oAlignedTo->GetScreenLocation() - GetContainer()->GetScreenLocation();

	arAlignStack.Remove( this );
	SetAlignLocation( v2AlignLocation );
}


FVector2D UKUIInterfaceElement::CalculateAlignLocation( const FVector2D& v2Origin, const FVector2D& v2Extent, const FVector2D& v2Size, const FVector2D& v2Default )
{
	FVector2D v2AlignLocation;
	const FVector2D v2MarginSize = GetMarginSize(); // X+Z, Y+W

	if ( bDebug )
		KUILogUO( "(%f,%f) (%f,%f) (%f,%f) (%f,%f)", ExpandV2( v2Origin ), ExpandV2( v2Extent ), ExpandV2( v2Size ), ExpandV2( v2Default ) );

	switch ( GetHorizontalAlignment() )
	{
		case EKUIInterfaceHAlign::HA_Left: 
			v2AlignLocation.X = v2Origin.X;
			break;

		case EKUIInterfaceHAlign::HA_Centre: 
			v2AlignLocation.X = v2Origin.X + ( v2Extent.X / 2.f ) - ( v2Size.X / 2.f ); 
			break;

		case EKUIInterfaceHAlign::HA_Right:
			v2AlignLocation.X = v2Origin.X + v2Extent.X - v2Size.X;
			break;

		case EKUIInterfaceHAlign::HA_Left_Outer:
			v2AlignLocation.X = v2Origin.X - v2Size.X;
			break;

		case EKUIInterfaceHAlign::HA_Right_Outer:
			v2AlignLocation.X = v2Origin.X + v2Extent.X;
			break;

		case EKUIInterfaceHAlign::HA_Fill:
			SetSize( v2Extent.X - v2MarginSize.X, GetSize().Y );
			v2AlignLocation.X = v2Origin.X;
			break;

		case EKUIInterfaceHAlign::HA_None:
		default:
			v2AlignLocation.X = v2Default.X;
			break;
	}

	switch ( GetVerticalAlignment() )
	{
		case EKUIInterfaceVAlign::VA_Top: 
			v2AlignLocation.Y = v2Origin.Y; 
			break;

		case EKUIInterfaceVAlign::VA_Centre: 
			v2AlignLocation.Y = v2Origin.Y + ( v2Extent.Y / 2.f ) - ( v2Size.Y / 2.f ); 
			break;

		case EKUIInterfaceVAlign::VA_Bottom:
			v2AlignLocation.Y = v2Origin.Y + v2Extent.Y - v2Size.Y;
			break;

		case EKUIInterfaceVAlign::VA_Top_Outer:
			v2AlignLocation.Y = v2Origin.Y - v2Size.Y;
			break;

		case EKUIInterfaceVAlign::VA_Bottom_Outer:
			v2AlignLocation.Y = v2Origin.Y + v2Extent.Y;
			break;

		case EKUIInterfaceVAlign::VA_Fill:
			SetSize( GetSize().X, v2Extent.Y - v2MarginSize.Y );
			v2AlignLocation.Y = v2Origin.Y;
			break;

		case EKUIInterfaceVAlign::VA_None:
		default:
			v2AlignLocation.Y = v2Default.Y;
			break;
	}

	v2AlignLocation += GetMarginOffset();

	if ( bDebug )
		KUILogUO( "%f,%f", ExpandV2( v2AlignLocation ) );

	return v2AlignLocation;
}


bool UKUIInterfaceElement::IsKeyEventConsumer() const
{
	return false;
}


void UKUIInterfaceElement::InvalidateRenderCache()
{
	if ( oRenderCache )
		oRenderCache->InvalidateRenderCache();

	InvalidateContainerRenderCache();
}


void UKUIInterfaceElement::InvalidateContainerRenderCache()
{
	if ( GetContainer() )
		GetContainer()->InvalidateRenderCache();
}


void UKUIInterfaceElement::EnableRenderCache()
{
	if ( oRenderCache )
		return;

	oRenderCache = NewObject<UKUIRenderCache>( this );
}


void UKUIInterfaceElement::DisableRenderCache()
{
	oRenderCache = NULL;
}


bool UKUIInterfaceElement::IsRenderCaching() const
{
	return ( oRenderCache != NULL );
}


bool UKUIInterfaceElement::RespondsToEvent( uint8 iEventID ) const
{
	return ( iEventID >= KUI_BASE_EVENT_FIRST && iEventID <= KUI_BASE_EVENT_LAST );
}


void UKUIInterfaceElement::SendEvent( FKUIInterfaceEvent& stEventInfo )
{
	if ( IsTemplate() )
		return;

	static TArray<std::function<void( UKUIInterfaceElement*, FKUIInterfaceEvent& )>> arDispatchers;

	if ( arDispatchers.Num() == 0 )
	{
		arDispatchers.SetNum( KUI_BASE_EVENT_LAST - KUI_BASE_EVENT_FIRST + 1 );

		arDispatchers[ EKUIInterfaceElementEventList::E_AddedToContainer - KUI_BASE_EVENT_FIRST ] = 
			[] ( UKUIInterfaceElement* oElement, FKUIInterfaceEvent& stEventInfo )
		{
			oElement->OnAddedToContainer( *( ( FKUIInterfaceElementContainerEvent* ) &stEventInfo ) );
			oElement->OnAddedToContainerBP( *( ( FKUIInterfaceElementContainerEvent* ) &stEventInfo ) );
		};

		arDispatchers[ EKUIInterfaceElementEventList::E_RemovedFromContainer - KUI_BASE_EVENT_FIRST ] = 
			[] ( UKUIInterfaceElement* oElement, FKUIInterfaceEvent& stEventInfo )
		{
			oElement->OnRemovedFromContainer( *( ( FKUIInterfaceElementContainerEvent* ) &stEventInfo ) );
			oElement->OnRemovedFromContainerBP( *( ( FKUIInterfaceElementContainerEvent* ) &stEventInfo ) );
		};

		arDispatchers[ EKUIInterfaceElementEventList::E_Render - KUI_BASE_EVENT_FIRST ] = 
			[] ( UKUIInterfaceElement* oElement, FKUIInterfaceEvent& stEventInfo )
		{
			oElement->OnRender( *( ( FKUIInterfaceElementRenderEvent* ) &stEventInfo ) );
			oElement->OnRenderBP( *( ( FKUIInterfaceElementRenderEvent* ) &stEventInfo ) );
		};

		arDispatchers[ EKUIInterfaceElementEventList::E_AlignLocationCalculated - KUI_BASE_EVENT_FIRST ] = 
			[] ( UKUIInterfaceElement* oElement, FKUIInterfaceEvent& stEventInfo )
		{
			oElement->OnAlignmentLocationCalculated( stEventInfo );
			oElement->OnAlignmentLocationCalculatedBP( stEventInfo );
		};

		arDispatchers[ EKUIInterfaceElementEventList::E_AlignLocationInvalidated - KUI_BASE_EVENT_FIRST ] = 
			[] ( UKUIInterfaceElement* oElement, FKUIInterfaceEvent& stEventInfo )
		{
			oElement->OnAlignLocationInvalidated( stEventInfo );
			oElement->OnAlignLocationInvalidatedBP( stEventInfo );
		};

		arDispatchers[ EKUIInterfaceElementEventList::E_LocationChange - KUI_BASE_EVENT_FIRST ] = 
			[] ( UKUIInterfaceElement* oElement, FKUIInterfaceEvent& stEventInfo )
		{
			oElement->OnLocationChange( *( ( FKUIInterfaceContainerLocationChangeEvent* ) &stEventInfo ) );
			oElement->OnLocationChangeBP( *( ( FKUIInterfaceContainerLocationChangeEvent* ) &stEventInfo ) );
		};

		arDispatchers[ EKUIInterfaceElementEventList::E_SizeChange - KUI_BASE_EVENT_FIRST ] = 
			[] ( UKUIInterfaceElement* oElement, FKUIInterfaceEvent& stEventInfo )
		{
			oElement->OnSizeChange( *( ( FKUIInterfaceContainerSizeChangeEvent* ) &stEventInfo ) );
			oElement->OnSizeChangeBP( *( ( FKUIInterfaceContainerSizeChangeEvent* ) &stEventInfo ) );
		};
	}

	if ( stEventInfo.iEventID >= KUI_BASE_EVENT_FIRST && stEventInfo.iEventID <= KUI_BASE_EVENT_LAST )
		arDispatchers[ stEventInfo.iEventID - KUI_BASE_EVENT_FIRST ]( this, stEventInfo );
}


void UKUIInterfaceElement::OnLocationChange( const FKUIInterfaceContainerLocationChangeEvent& stEventInfo )
{

}


void UKUIInterfaceElement::OnSizeChange( const FKUIInterfaceContainerSizeChangeEvent& stEventInfo )
{

}


void UKUIInterfaceElement::OnAlignLocationInvalidated( const FKUIInterfaceEvent& stEventInfo )
{

}


void UKUIInterfaceElement::OnAlignmentLocationCalculated( const FKUIInterfaceEvent& stEventInfo )
{

}


void UKUIInterfaceElement::OnAddedToContainer( const FKUIInterfaceElementContainerEvent& stEventInfo )
{
	this->ctContainer = stEventInfo.ctContainer;

	InvalidateAlignLocation();
	InvalidateContainerRenderCache();
}


void UKUIInterfaceElement::OnRemovedFromContainer( const FKUIInterfaceElementContainerEvent& stEventInfo )
{
	this->ctContainer = NULL;

	InvalidateAlignLocation();
	InvalidateContainerRenderCache();
}

void UKUIInterfaceElement::OnRender( const FKUIInterfaceElementRenderEvent& stEventInfo )
{

}
