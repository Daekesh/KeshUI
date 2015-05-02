// Copyright 2014-2015 Matt Chapman. All Rights Reserved.


#include "KeshUI/KeshUI.h"
#include "KeshUI/Widget/KUISliderWidget.h"
#include "KeshUI/Container/KUIScrollContainer.h"


UKUIScrollContainer::UKUIScrollContainer( const class FObjectInitializer& oObjectInitializer )
: Super( oObjectInitializer )
{
	KUICreateDefaultSubobjectAssign( ctScrollArea, UKUISubContainer, "Scroll Area" );
	ctScrollArea->FillContainer();
	Super::AddChild( ctScrollArea.Get() );

	v2ScrollBarSize = FVector2D::ZeroVector;
	cmHorizontalScrollBar = NULL;
	cmVerticalScrollBar = NULL;
	oCornerComponent = NULL;
	v2LastScrollAreaSize = FVector2D::ZeroVector;
}


void UKUIScrollContainer::AddChild( UKUIInterfaceElement* oChild )
{
	if ( ctScrollArea.IsValid() )
		ctScrollArea->AddChild( oChild );

	else
	{
		KUIErrorUO( "Null scroll area" );
	}
}


bool UKUIScrollContainer::RemoveChild( UKUIInterfaceElement* oChild )
{
	if ( ctScrollArea.IsValid() )
		return ctScrollArea->RemoveChild( oChild );

	KUIErrorUO( "Null scroll area" );
	return false;
}


void UKUIScrollContainer::SetSize( float fWidth, float fHeight )
{
	if ( v2Size.X == fWidth && v2Size.Y == fHeight )
		return;

	Super::SetSize( fWidth, fHeight );

	UpdateScrollbarMetrics();
}


FVector2D UKUIScrollContainer::GetCornerSize() const
{
	return v2ScrollBarSize;
}


void UKUIScrollContainer::SetCornerSizeStruct( const FVector2D& v2ScrollSize )
{
	SetCornerSize( v2ScrollSize.X, v2ScrollSize.Y );
}


void UKUIScrollContainer::SetCornerSize( float fWidth, float fHeight )
{
	if ( v2ScrollBarSize.X == fWidth && v2ScrollBarSize.Y == fHeight )
		return;

	v2ScrollBarSize.X = fWidth;
	v2ScrollBarSize.Y = fHeight;

	UpdateScrollbarMetrics();
}


void UKUIScrollContainer::UpdateScrollbarMetrics()
{
	if ( !ctScrollArea.IsValid() )
	{
		KUIErrorUO( "Null scroll area" );
		return;
	}

	FVector2D v2ScrollContainerSize = GetSize();
	const FVector2D v2ScrollTotalSize = ctScrollArea->GetTotalSize();

	if ( v2ScrollTotalSize.X > v2ScrollContainerSize.X )
		v2ScrollContainerSize.X -= v2ScrollBarSize.X;

	if ( v2ScrollTotalSize.Y > v2ScrollContainerSize.Y )
		v2ScrollContainerSize.Y -= v2ScrollBarSize.Y;

	// Check again to see if the vertical scrollbar means we need to scroll.
	if ( v2ScrollTotalSize.X > v2ScrollContainerSize.X )
		v2ScrollContainerSize.X -= v2ScrollBarSize.X;

	ctScrollArea->SetSizeStruct( v2ScrollContainerSize );

	const FVector2D v2Size = GetSize();

	if ( v2ScrollContainerSize.X == v2Size.X && v2ScrollContainerSize.Y == v2Size.Y )
	{
		if ( cmHorizontalScrollBar.IsValid() )
			cmHorizontalScrollBar->SetVisible( false );

		if ( cmVerticalScrollBar.IsValid() )
			cmVerticalScrollBar->SetVisible( false );

		if ( oCornerComponent.IsValid() )
			oCornerComponent->SetVisible( false );
	}

	else if ( v2ScrollContainerSize.Y == v2Size.Y )
	{
		if ( cmHorizontalScrollBar.IsValid() )
		{
			cmHorizontalScrollBar->SetVisible( true );
			cmHorizontalScrollBar->SetSize( v2Size.X, v2ScrollBarSize.Y );
		}

		if ( cmVerticalScrollBar.IsValid() )
			cmVerticalScrollBar->SetVisible( false );

		if ( oCornerComponent.IsValid() )
			oCornerComponent->SetVisible( false );
	}

	else if ( v2ScrollContainerSize.X == v2Size.X )
	{
		if ( cmHorizontalScrollBar.IsValid() )
			cmHorizontalScrollBar->SetVisible( false );

		if ( cmVerticalScrollBar.IsValid() )
		{
			cmVerticalScrollBar->SetVisible( true );
			cmVerticalScrollBar->SetSize( v2ScrollBarSize.X, v2Size.Y );
		}

		if ( oCornerComponent.IsValid() )
			oCornerComponent->SetVisible( false );
	}

	else
	{
		if ( cmHorizontalScrollBar.IsValid() )
		{
			cmHorizontalScrollBar->SetVisible( true );
			cmHorizontalScrollBar->SetSize( v2Size.X - v2ScrollBarSize.X, v2ScrollBarSize.Y );
		}

		if ( cmVerticalScrollBar.IsValid() )
		{
			cmVerticalScrollBar->SetVisible( true );
			cmVerticalScrollBar->SetSize( v2ScrollBarSize.X, v2Size.Y - v2ScrollBarSize.Y );
		}

		if ( oCornerComponent.IsValid() )
		{
			oCornerComponent->SetVisible( true );
			oCornerComponent->SetSize( v2Size.X, v2Size.Y );
		}
	}
}


FVector2D UKUIScrollContainer::GetScrollableSize() const
{
	if ( ctScrollArea.IsValid() )
		return ctScrollArea->GetTotalSize();

	KUIErrorUO( "Null scroll area" );
	return FVector2D::ZeroVector;
}


void UKUIScrollContainer::SetScrollableSizeStruct( const FVector2D& v2ScrollableSize )
{
	SetScrollableSize( v2ScrollableSize.X, v2ScrollableSize.Y );
}


void UKUIScrollContainer::SetScrollableSize( float fWidth, float fHeight )
{
	if ( !ctScrollArea.IsValid() )
	{
		KUIErrorUO( "Null scroll area" );
		return;
	}

	ctScrollArea->SetTotalSize( fWidth, fHeight );
	
	UpdateScrollbarMetrics();
}


UKUISliderWidget* UKUIScrollContainer::GetHorizontalScrollbar() const
{
	return cmHorizontalScrollBar.Get();
}


void UKUIScrollContainer::SetHorizontalScrollbar( UKUISliderWidget* cmScrollbar )
{
	if ( cmHorizontalScrollBar.IsValid() )
	{
		Super::RemoveChild( cmHorizontalScrollBar.Get() );
		cmHorizontalScrollBar->SetValueChangeDelegate( NULL, NULL );
	}

	cmHorizontalScrollBar = cmScrollbar;

	if ( cmHorizontalScrollBar.IsValid() )
	{
		Super::AddChild( cmHorizontalScrollBar.Get() );
		cmHorizontalScrollBar->SetValueChangeDelegate( this, static_cast<FKUISliderValueChangePrototype>( &UKUIScrollContainer::OnScrollbarScrolled ) );
		cmHorizontalScrollBar->SetHorizontalAlignment( EKUIInterfaceHAlign::HA_Left );
		cmHorizontalScrollBar->SetVerticalAlignment( EKUIInterfaceVAlign::VA_Bottom );
	}

	UpdateScrollbarMetrics();
}


UKUISliderWidget* UKUIScrollContainer::GetVerticalScrollbar() const
{
	return cmVerticalScrollBar.Get();
}


void UKUIScrollContainer::SetVerticalScrollbar( UKUISliderWidget* cmScrollbar )
{
	if ( cmVerticalScrollBar.IsValid() )
	{
		Super::RemoveChild( cmVerticalScrollBar.Get() );
		cmVerticalScrollBar->SetValueChangeDelegate( NULL, NULL );
	}

	cmVerticalScrollBar = cmScrollbar;

	if ( cmVerticalScrollBar.IsValid() )
	{
		Super::AddChild( cmVerticalScrollBar.Get() );
		cmVerticalScrollBar->SetValueChangeDelegate( this, static_cast<FKUISliderValueChangePrototype>( &UKUIScrollContainer::OnScrollbarScrolled ) );
		cmVerticalScrollBar->SetHorizontalAlignment( EKUIInterfaceHAlign::HA_Right );
		cmVerticalScrollBar->SetVerticalAlignment( EKUIInterfaceVAlign::VA_Top );
	}

	UpdateScrollbarMetrics();
}


UKUIInterfaceElement* UKUIScrollContainer::GetCornerComponent() const
{
	return oCornerComponent.Get();
}


void UKUIScrollContainer::SetCornerComponent( UKUIInterfaceElement* oCorner )
{
	if ( oCornerComponent.IsValid() )
		Super::RemoveChild( oCornerComponent.Get() );

	oCornerComponent = oCorner;

	if ( oCornerComponent.IsValid() )
	{
		Super::AddChild( oCornerComponent.Get() );
		oCornerComponent->SetHorizontalAlignment( EKUIInterfaceHAlign::HA_Right );
		oCornerComponent->SetVerticalAlignment( EKUIInterfaceVAlign::VA_Bottom );
	}

	UpdateScrollbarMetrics();
}


void UKUIScrollContainer::OnScrollbarScrolled( UKUISliderWidget* cmSlider )
{
	if ( !ctScrollArea.IsValid() )
	{
		KUIErrorUO( "Null scroll area" );
		return;
	}

	const FVector2D v2TotalSize = ctScrollArea->GetTotalSize();
	const FVector2D v2VisibleSize = ctScrollArea->GetSize();
	FVector2D v2ScrollAmount = v2TotalSize - v2VisibleSize;

	if ( v2ScrollAmount.X > 0.f && cmHorizontalScrollBar.IsValid() )
		v2ScrollAmount.X *= cmHorizontalScrollBar->GetValue();

	else
		v2ScrollAmount.X = 0.f;

	if ( v2ScrollAmount.Y > 0.f && cmVerticalScrollBar.IsValid() )
		v2ScrollAmount.Y *= cmVerticalScrollBar->GetValue();

	else
		v2ScrollAmount.Y = 0.f;

	ctScrollArea->SetCornerOffsetStruct( v2ScrollAmount );
}


void UKUIScrollContainer::SetVisibleAreaByFraction( float fVertical, float fHorizontal )
{
	if ( !ctScrollArea.IsValid() )
	{
		KUIErrorUO( "Null scroll area" );
		return;
	}

	if ( fVertical == KUI_SCROLL_CONTAINER_NO_SCROLL && fHorizontal == KUI_SCROLL_CONTAINER_NO_SCROLL )
		return;

	FVector2D v2MaxCornerSize = ctScrollArea->GetTotalSize() - ctScrollArea->GetSize();

	v2MaxCornerSize.X = max( 0.f, v2MaxCornerSize.X );
	v2MaxCornerSize.Y = max( 0.f, v2MaxCornerSize.Y );

	if ( fVertical == KUI_SCROLL_CONTAINER_NO_SCROLL )
		ctScrollArea->SetCornerOffset( v2MaxCornerSize.X * fHorizontal, ctScrollArea->GetCornerOffset().Y );

	else if ( fHorizontal == KUI_SCROLL_CONTAINER_NO_SCROLL )
		ctScrollArea->SetCornerOffset( ctScrollArea->GetCornerOffset().X, v2MaxCornerSize.Y * fVertical );

	else
		ctScrollArea->SetCornerOffset( v2MaxCornerSize.X * fHorizontal, v2MaxCornerSize.Y * fVertical );
}


UKUISubContainer* UKUIScrollContainer::GetScrollArea() const
{
	return ctScrollArea.Get();
}


void UKUIScrollContainer::Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject )
{
	if ( ctScrollArea.IsValid() )
	{
		if ( v2LastScrollAreaSize.X != ctScrollArea->GetTotalSize().X || v2LastScrollAreaSize.Y != ctScrollArea->GetTotalSize().Y )
		{
			UpdateScrollbarMetrics();
			v2LastScrollAreaSize = ctScrollArea->GetTotalSize();
		}
	}

	Super::Render( aHud, oCanvas, v2Origin, oRenderCacheObject );
}
