// Copyright 2014-2015 Matt Chapman. All Rights Reserved.


#include "KeshUI/KeshUI.h"
#include "KeshUI/Component/KUITextureInterfaceComponent.h"
#include "KeshUI/Container/KUIScrollContainer.h"
#include "KeshUI/Widget/KUIScrollBarWidget.h"


UKUIScrollBarWidget::UKUIScrollBarWidget( const class FObjectInitializer& oObjectInitializer )
: Super( oObjectInitializer )
{
	oEndLeftTop = NULL;
	oEndRightBottom = NULL;
	oCentral = NULL;
	bTileInsteadOfStretchTrack = false;
	ctScrollContainer = NULL;
	bScaleHandle = true;
	fMinimumHandleSize = 8.f;
}


void UKUIScrollBarWidget::SetHorizontal( bool bHorizontal )
{
	if ( this->bHorizontal == bHorizontal )
		return;

	Super::SetHorizontal( bHorizontal );

	UpdateScrollbarComponents();
}


UKUIInterfaceElement* UKUIScrollBarWidget::GetLeftTopTrackComponent() const
{
	return oEndLeftTop.Get();
}


void UKUIScrollBarWidget::SetLeftTopTrackComponent( UKUIInterfaceElement* oComponent )
{
	if ( oEndLeftTop.IsValid() )
		RemoveChild( oEndLeftTop.Get() );

	oEndLeftTop = oComponent;

	if ( oEndLeftTop.IsValid() )
		AddChild( oEndLeftTop.Get() );

	UpdateScrollbarComponents();
}


UKUIInterfaceElement* UKUIScrollBarWidget::GetRightBottomTrackComponent() const
{
	return oEndRightBottom.Get();
}


void UKUIScrollBarWidget::SetRightBottomTrackComponent( UKUIInterfaceElement* oComponent )
{
	if ( oEndRightBottom.IsValid() )
		RemoveChild( oEndRightBottom.Get() );

	oEndRightBottom = oComponent;

	if ( oEndRightBottom.IsValid() )
		AddChild( oEndRightBottom.Get() );

	UpdateScrollbarComponents();
}


UKUIInterfaceElement* UKUIScrollBarWidget::GetCentralTrackComponent() const
{
	return oEndRightBottom.Get();
}


void UKUIScrollBarWidget::SetCentralTrackComponent( UKUIInterfaceElement* oComponent )
{
	if ( oCentral.IsValid() )
		RemoveChild( oCentral.Get() );

	oCentral = oComponent;

	if ( oCentral.IsValid() )
		AddChild( oCentral.Get() );

	UpdateScrollbarComponents();
}


bool UKUIScrollBarWidget::ShouldTileInsteadOfStretchTrack() const
{
	return bTileInsteadOfStretchTrack;
}


void UKUIScrollBarWidget::SetTileInsteadOfStretchTrack( bool bValue )
{
	if ( bTileInsteadOfStretchTrack == bValue )
		return;

	bTileInsteadOfStretchTrack = bValue;

	UpdateScrollbarComponents();
}


UKUIScrollContainer* UKUIScrollBarWidget::GetScrollContainer() const
{
	return ctScrollContainer.Get();
}


void UKUIScrollBarWidget::SetScrollContainer( UKUIScrollContainer* ctScrollContainer )
{
	this->ctScrollContainer = ctScrollContainer;
}


void UKUIScrollBarWidget::UpdateScrollbarComponents()
{
	if ( oHandle.IsValid() )
		oHandle->SetZIndex( 2 );

	if ( bHorizontal )
	{
		float fTotalLength = GetSize().X;

		if ( oEndLeftTop.IsValid() )
		{
			oEndLeftTop->SetHorizontalAlignment( EKUIInterfaceHAlign::HA_Left );
			oEndLeftTop->SetVerticalAlignment( EKUIInterfaceVAlign::VA_Centre );
			oEndLeftTop->SetZIndex( 1 );

			fTotalLength -= oEndLeftTop->GetSize().X;

			if ( oCentral.IsValid() )
				oCentral->SetLocation( oEndLeftTop->GetSize().X, 0.f );
		}

		else
			oCentral->SetLocation( 0.f, 0.f );

		if ( oEndRightBottom.IsValid() )
		{
			oEndRightBottom->SetHorizontalAlignment( EKUIInterfaceHAlign::HA_Right );
			oEndRightBottom->SetVerticalAlignment( EKUIInterfaceVAlign::VA_Centre );
			oEndRightBottom->SetZIndex( 1 );

			fTotalLength -= oEndRightBottom->GetSize().X;
		}

		if ( oCentral.IsValid() )
		{
			oCentral->SetSize( fTotalLength, GetSize().Y );
			oCentral->SetHorizontalAlignment( EKUIInterfaceHAlign::HA_None );
			oCentral->SetVerticalAlignment( EKUIInterfaceVAlign::VA_Centre );
			oCentral->SetZIndex( 0 );

			if ( oCentral->IsA<UKUITextureInterfaceComponent>() )
			{
				UKUITextureInterfaceComponent* const cmTexture = Cast<UKUITextureInterfaceComponent>( oCentral.Get() );

				if ( cmTexture->GetTexture() != NULL )
				{
					if ( bTileInsteadOfStretchTrack )
						cmTexture->SetTextureSize( fTotalLength / cmTexture->GetTexture()->GetSurfaceWidth(), 1.f );

					else
						cmTexture->SetTextureSize( 1.f, 1.f );
				}
			}
		}

		if ( bScaleHandle && ctScrollContainer.IsValid() && ctScrollContainer->GetScrollArea() != NULL && oHandle.IsValid() )
		{
			float fFraction = ctScrollContainer->GetScrollArea()->GetSize().X / max( 1.f, ctScrollContainer->GetScrollArea()->GetTotalSize().X );
			float fSize = max( 1.f, ( fTotalLength - ( fEndSize * 2.f ) ) * fFraction );

			if ( fSize < fMinimumHandleSize )
				fSize = fMinimumHandleSize;

			oHandle->SetHorizontalAlignment( EKUIInterfaceHAlign::HA_None );
			oHandle->SetSize( fSize, oHandle->GetSize().Y );
		}
	}

	else
	{
		float fTotalLength = GetSize().Y;

		if ( oEndLeftTop.IsValid() )
		{
			oEndLeftTop->SetHorizontalAlignment( EKUIInterfaceHAlign::HA_Centre );
			oEndLeftTop->SetVerticalAlignment( EKUIInterfaceVAlign::VA_Top );
			oEndLeftTop->SetZIndex( 1 );

			fTotalLength -= oEndLeftTop->GetSize().Y;

			if ( oCentral.IsValid() )
				oCentral->SetLocation( 0.f, oEndLeftTop->GetSize().Y );
		}

		else
			oCentral->SetLocation( 0.f, 0.f );

		if ( oEndRightBottom.IsValid() )
		{
			oEndRightBottom->SetHorizontalAlignment( EKUIInterfaceHAlign::HA_Centre );
			oEndRightBottom->SetVerticalAlignment( EKUIInterfaceVAlign::VA_Bottom );
			oEndRightBottom->SetZIndex( 1 );

			fTotalLength -= oEndRightBottom->GetSize().Y;
		}

		if ( oCentral.IsValid() )
		{
			oCentral->SetSize( GetSize().X, fTotalLength );
			oCentral->SetHorizontalAlignment( EKUIInterfaceHAlign::HA_Centre );
			oCentral->SetVerticalAlignment( EKUIInterfaceVAlign::VA_None );
			oCentral->SetZIndex( 0 );

			if ( oCentral->IsA<UKUITextureInterfaceComponent>() )
			{
				UKUITextureInterfaceComponent* const cmTexture = Cast<UKUITextureInterfaceComponent>( oCentral.Get() );

				if ( cmTexture->GetTexture() != NULL )
				{
					if ( bTileInsteadOfStretchTrack )
						cmTexture->SetTextureSize( 1.f, fTotalLength / cmTexture->GetTexture()->GetSurfaceWidth() );

					else
						cmTexture->SetTextureSize( 1.f, 1.f );
				}
			}
		}

		if ( bScaleHandle && ctScrollContainer.IsValid() && ctScrollContainer->GetScrollArea() != NULL && oHandle.IsValid() )
		{
			float fFraction = ctScrollContainer->GetScrollArea()->GetSize().Y / max( 1.f, ctScrollContainer->GetScrollArea()->GetTotalSize().Y );
			float fSize = max( 1.f, ( fTotalLength - ( fEndSize * 2.f ) ) * fFraction );

			if ( fSize < fMinimumHandleSize )
				fSize = fMinimumHandleSize;

			oHandle->SetVerticalAlignment( EKUIInterfaceVAlign::VA_None );
			oHandle->SetSize( oHandle->GetSize().X, fSize );
		}
	}
}


void UKUIScrollBarWidget::OnWidgetStateChange()
{
	Super::OnWidgetStateChange();

	if ( !ctScrollContainer.IsValid() )
		return;

	if ( bHorizontal )
		ctScrollContainer->SetVisibleAreaByFraction( KUI_SCROLL_CONTAINER_NO_SCROLL, GetValue() );

	else
		ctScrollContainer->SetVisibleAreaByFraction( GetValue(), KUI_SCROLL_CONTAINER_NO_SCROLL );
		
}


void UKUIScrollBarWidget::SetSize( float fWidth, float fHeight )
{
	if ( v2Size.X == fWidth && v2Size.Y == fHeight )
		return;

	Super::SetSize( fWidth, fHeight );

	UpdateScrollbarComponents();
}


bool UKUIScrollBarWidget::IsHandleScaling() const
{
	return bScaleHandle;
}


void UKUIScrollBarWidget::SetHandleScaling( bool bScaling )
{
	if ( bScaleHandle == bScaling )
		return;

	bScaleHandle = bScaling;

	UpdateScrollbarComponents();
}


float UKUIScrollBarWidget::GetMinimumHandleSize() const
{
	return fMinimumHandleSize;
}


void UKUIScrollBarWidget::SetMinimumHandleSize( float fSize )
{
	fSize = min( fSize, 1.f );

	if ( fMinimumHandleSize == fSize )
		return;

	fMinimumHandleSize = fSize;

	UpdateScrollbarComponents();
}
