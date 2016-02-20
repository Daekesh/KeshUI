// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#include "KeshUI/KeshUI.h"
#include "KeshUI/KUIInterfaceContainer.h"
#include "KeshUI/Component/KUITextInterfaceComponent.h"
#include "KeshUI/Component/KUICanvasItemInterfaceComponent.h"


UKUICanvasItemInterfaceComponent::UKUICanvasItemInterfaceComponent( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	stItem.Reset();
	bItemInvalidated = false;
	eBlendMode = ESimpleElementBlendMode::SE_BLEND_Translucent;
	bRoundPosition = true;
}


UKUICanvasItemInterfaceComponent::~UKUICanvasItemInterfaceComponent()
{
	stItem.Reset();
}


ESimpleElementBlendMode UKUICanvasItemInterfaceComponent::GetBlendMode() const
{
	return eBlendMode;
}


uint8 UKUICanvasItemInterfaceComponent::GetBlendModeBP() const
{
	return static_cast< uint8 >( eBlendMode );
}


void UKUICanvasItemInterfaceComponent::SetBlendMode( uint8 eBlendMode )
{
	this->eBlendMode = static_cast< ESimpleElementBlendMode >( eBlendMode );
}


bool UKUICanvasItemInterfaceComponent::IsRoundingPosition() const
{
	return bRoundPosition;
}


void UKUICanvasItemInterfaceComponent::SetRoundingPosition( bool bEnabled )
{
	if ( bRoundPosition == bEnabled )
		return;

	bRoundPosition = bEnabled;

	InvalidateRenderCache();
}


void UKUICanvasItemInterfaceComponent::SetDrawColor( float fRed, float fGreen, float fBlue, float fAlpha )
{
	Super::SetDrawColor( fRed, fGreen, fBlue, fAlpha );

	Invalidate();
}


bool UKUICanvasItemInterfaceComponent::CanRender() const
{
	return stItem.IsValid();
}


bool UKUICanvasItemInterfaceComponent::HasValidComponents() const
{
	return true;
}


void UKUICanvasItemInterfaceComponent::Invalidate()
{
	bItemInvalidated = true;

	InvalidateRenderCache();
	
	if ( !IsTemplate() )
		OnInvalidated();
}


void UKUICanvasItemInterfaceComponent::Validate()
{
	bItemInvalidated = false;

	if ( !IsTemplate() )
		OnValidated();
}


void UKUICanvasItemInterfaceComponent::ConstructNewItem()
{
	if ( !stItem.IsValid() )
		return;

	stItem->SetColor( GetDrawColor().ReinterpretAsLinear() );
	Validate();

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


void UKUICanvasItemInterfaceComponent::Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject )
{
	if ( bItemInvalidated )
		ConstructNewItem();

	if ( !stItem.IsValid() )
		return;

	Super::Render( aHud, oCanvas, v2Origin, oRenderCacheObject );

	if ( IsRenderCaching() && oRenderCacheObject != this )
		return;

	const FVector2D v2RenderLocation = ( ( IsRenderCaching() || !IsPositionable() ) ? FVector2D::ZeroVector : v2Origin + GetRenderLocation() );

	stItem->Position = v2RenderLocation;
	stItem->Position.X = bRoundPosition ? FMath::RoundToInt( stItem->Position.X ) : stItem->Position.X;
	stItem->Position.Y = bRoundPosition ? FMath::RoundToInt( stItem->Position.Y ) : stItem->Position.Y;

	if ( oRenderCacheObject != NULL && !this->IsA<UKUITextInterfaceComponent>() && ( eBlendMode == SE_BLEND_Translucent || eBlendMode == SE_BLEND_TranslucentAlphaOnly ) )
		stItem->BlendMode = ESimpleElementBlendMode::SE_BLEND_AlphaComposite;

	else
		stItem->BlendMode = eBlendMode;

	//FVector2D v2Size = GetSize();
	//oCanvas->SetClip( stItem->Position.X + v2Size.X, stItem->Position.Y + v2Size.Y );

	//if ( bDebug )
		//UKUILogUO( "%f,%f" ), ExpandV2( stItem->Position ) );
	
	oCanvas->DrawItem( *stItem );
}
