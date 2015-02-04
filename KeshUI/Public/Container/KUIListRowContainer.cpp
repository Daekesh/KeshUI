// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#include "KeshUI.h"
#include "Component/KUITextureInterfaceComponent.h"
#include "Component/KUITextInterfaceComponent.h"
#include "Container/KUIListContainer.h"
#include "Container/KUIListRowContainer.h"


UKUIListRowContainer::UKUIListRowContainer( const class FObjectInitializer& oObjectInitializer )
	: Super( oObjectInitializer )
{
	bSelectedCache = false;

	stUnselectedForegroundColor = FColor::White;
	stUnselectedBackgroundColor = FColor::Black;
	stUnselectedBackgroundColor.A = 0.f;

	stSelectedForegroundColor = FColor::Black;
	stSelectedBackgroundColor = FColor::White;

	SetHorizontalAlignment( EKUIInterfaceHAlign::HA_Fill );

	static KUIFindObj( tBackground, UTexture2D, "Texture2D'/Engine/EngineResources/WhiteSquareTexture.WhiteSquareTexture'" );

	KUICreateDefaultSubobjectAssign( cmBackground, UKUITextureInterfaceComponent, "Selection Background" );
	cmBackground->SetTexture( tBackground.Object );
	cmBackground->FillContainer();
	cmBackground->SetDrawColorStruct( stUnselectedBackgroundColor );
	cmBackground->SetZIndex( -1 );
	cmBackground->SetVisible( false );

	AddChild( cmBackground );
}


bool UKUIListRowContainer::IsSelected() const
{
	return bSelectedCache;
}


void UKUIListRowContainer::UpdateSelected( bool bSelected )
{
	if ( bSelected == bSelectedCache )
		return;

	bSelectedCache = bSelected;
	UpdateComponents();
}


FColor UKUIListRowContainer::GetUnselectedForegroundColor() const
{
	return stUnselectedForegroundColor;
}


void UKUIListRowContainer::SetUnselectedForegroundColor( const FColor& stColor )
{
	if ( stUnselectedForegroundColor == stColor )
		return;

	stUnselectedBackgroundColor = stColor;
	UpdateComponents();
}


FColor UKUIListRowContainer::GetSelectedForegroundColor() const
{
	return stSelectedForegroundColor;
}


void UKUIListRowContainer::SetSelectedForegroundColor( const FColor& stColor )
{
	if ( stSelectedForegroundColor == stColor )
		return;

	stSelectedForegroundColor = stColor;
	UpdateComponents();
}


FColor UKUIListRowContainer::GetUnselectedBackgroundColor() const
{
	return stUnselectedBackgroundColor;
}


void UKUIListRowContainer::SetUnselectedBackgroundColor( const FColor& stColor )
{
	if ( stUnselectedBackgroundColor == stColor )
		return;

	stUnselectedBackgroundColor = stColor;
	UpdateComponents();
}


FColor UKUIListRowContainer::GetSelectedBackgroundColor() const
{
	return stSelectedBackgroundColor;
}


void UKUIListRowContainer::SetSelectedBackgroundColor( const FColor& stColor )
{
	if ( stSelectedBackgroundColor == stColor )
		return;

	stSelectedBackgroundColor = stColor;
	UpdateComponents();
}


void UKUIListRowContainer::OnAddedToContainer( const FKUIInterfaceElementContainerEvent& stEventInfo )
{
	Super::OnAddedToContainer( stEventInfo );

	UKUIListContainer* const ctList = Cast<UKUIListContainer>( stEventInfo.ctContainer );

	if ( ctList == NULL )
	{
		if ( !bSelectedCache )
			return;
		
		bSelectedCache = false;
	}		

	else
	{
		bool bSelected = ctList->IsRowSelected( this );

		if ( bSelected == bSelectedCache )
			return;

		bSelectedCache = bSelected;
	}

	UpdateComponents();
}


void UKUIListRowContainer::OnRemovedFromContainer( const FKUIInterfaceElementContainerEvent& stEventInfo )
{
	Super::OnRemovedFromContainer( stEventInfo );

	if ( !bSelectedCache )
		return;

	bSelectedCache = false;
	UpdateComponents();
}


void UKUIListRowContainer::UpdateComponents()
{
	cmBackground->SetDrawColorStruct( IsSelected() ? stSelectedBackgroundColor : stUnselectedBackgroundColor );
	cmBackground->SetVisible( cmBackground->GetDrawColor().A > 0.f );

	UpdateComponents( this );
}


void UKUIListRowContainer::UpdateComponents( UKUIInterfaceContainer* ctContainer )
{
	if ( ctContainer == NULL )
		return;

	for ( auto itChildren = ctContainer->GetChildIterator(); itChildren; ++itChildren )
	{
		UKUITextInterfaceComponent* const cmText = Cast<UKUITextInterfaceComponent>( *itChildren );

		if ( cmText != NULL )
		{
			cmText->SetDrawColorStruct( IsSelected() ? stSelectedForegroundColor : stUnselectedForegroundColor );

			if ( cmText->IsOutlined() )
				cmText->SetOutlineColor( IsSelected() ? stUnselectedForegroundColor : stSelectedForegroundColor );

			continue;
		}

		UKUIInterfaceContainer* const ctContainer = Cast<UKUIInterfaceContainer>( *itChildren );

		if ( ctContainer != NULL )
			UpdateComponents( ctContainer );
	}
}


void UKUIListRowContainer::DoLayout()
{
	float fMaxHeight = 0.f;

	UKUIListContainer* ctList = Cast<UKUIListContainer>( GetContainer() );

	if ( ctList != NULL )
		fMaxHeight = ctList->GetMinimumRowHeight();

	for ( UKUIInterfaceElement* oElement : arChildren )
	{
		if ( oElement->GetAlignedTo() == this || oElement->GetAlignedTo() == NULL )
		{
			switch ( oElement->GetVerticalAlignment() )
			{
				case EKUIInterfaceVAlign::VA_Bottom_Outer:
				case EKUIInterfaceVAlign::VA_Top_Outer:
					continue;

				case EKUIInterfaceVAlign::VA_Bottom:
					fMaxHeight = max( oElement->GetSize().Y - oElement->GetLocation().Y, fMaxHeight );
					continue;

				case EKUIInterfaceVAlign::VA_Centre:
					fMaxHeight = max( oElement->GetSize().Y + oElement->GetLocation().Y, fMaxHeight );
					continue;

				case EKUIInterfaceVAlign::VA_Fill:
					fMaxHeight = max( oElement->GetSize().Y, fMaxHeight );
					continue;
			}
		}

		fMaxHeight = max( oElement->GetRenderLocation().Y + oElement->GetSize().Y, fMaxHeight );
	}

	if ( ctList != NULL )
		fMaxHeight = min( fMaxHeight, ctList->GetMaximumRowHeight() );

	SetSize( GetSize().X, fMaxHeight );

	Super::DoLayout();
}
