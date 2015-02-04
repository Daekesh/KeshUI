// Copyright 2014-2015 Matt Chapman. All Rights Reserved.


#include "KeshUI.h"
#include "Widget/KUIButtonWidget.h"
#include "Component/KUITextInterfaceComponent.h"
#include "Widget/KUIScrollSelectWidget.h"


UKUIScrollSelectWidget::UKUIScrollSelectWidget( const class FObjectInitializer& oObjectInitializer )
: Super( oObjectInitializer )
{
	cmLeftButton = NULL;
	cmRightButton = NULL;
	v2LargestTextSize = FVector2D::ZeroVector;

	KUICreateDefaultSubobjectAssign( cmText, UKUITextInterfaceComponent, "Text Field" );
	cmText->SetSize( 0, 0 );

	AddChild( cmText.Get() );
}


void UKUIScrollSelectWidget::SetLeftButton( UKUIButtonWidget* cmLeftButton )
{
	if ( this->cmLeftButton.IsValid() )
	{
		this->cmLeftButton->SetClickDelegate( NULL, NULL );
		RemoveChild( this->cmLeftButton.Get() );
	}

	this->cmLeftButton = cmLeftButton;

	if ( this->cmLeftButton.IsValid() )
	{
		this->cmLeftButton->SetClickDelegate( this, ( FKUISimpleClickWidgetClickPrototype ) &UKUIScrollSelectWidget::OnLeftButtonClick );
		this->cmLeftButton->SetHorizontalAlignment( EKUIInterfaceHAlign::HA_Left );
		this->cmLeftButton->SetVerticalAlignment( EKUIInterfaceVAlign::VA_Centre );
		AddChild( this->cmLeftButton.Get() );
	}

	UpdateDisabledStates();
	InvalidateLayout();
}


void UKUIScrollSelectWidget::SetRightButton( UKUIButtonWidget* cmRightButton )
{
	if ( this->cmRightButton.IsValid() )
	{
		this->cmRightButton->SetClickDelegate( NULL, NULL );
		RemoveChild( this->cmRightButton.Get() );
	}

	this->cmRightButton = cmRightButton;

	if ( this->cmRightButton.IsValid() )
	{
		this->cmRightButton->SetClickDelegate( this, ( FKUISimpleClickWidgetClickPrototype ) &UKUIScrollSelectWidget::OnRightButtonClick );
		this->cmRightButton->SetHorizontalAlignment( EKUIInterfaceHAlign::HA_Right );
		this->cmRightButton->SetVerticalAlignment( EKUIInterfaceVAlign::VA_Centre );
		AddChild( this->cmRightButton.Get() );
	}

	UpdateDisabledStates();
	InvalidateLayout();
}


void UKUIScrollSelectWidget::SetDisabled( TEnumAsByte<EKUIBool::SetValue> eValue )
{
	Super::SetDisabled( eValue );

	UpdateDisabledStates();
}


bool UKUIScrollSelectWidget::SetValueByIndex( uint8 iIndex )
{
	if ( Super::SetValueByIndex( iIndex ) )
	{
		UpdateDisabledStates();
		
		if ( iSelectedValue == KUI_SELECT_WIDGET_NO_VALUE )
			cmText->SetText( FText::GetEmpty() );

		else
			cmText->SetText( arValues[ iIndex ] );

		return true;
	}

	return false;	
}


uint8 UKUIScrollSelectWidget::AddValue( const FText& txValue )
{
	uint8 iIndex = Super::AddValue( txValue );

	if ( iIndex != KUI_SELECT_WIDGET_NO_VALUE )
		InvalidateLayout();

	return iIndex;
}


bool UKUIScrollSelectWidget::RemoveValueByIndex( uint8 iIndex )
{
	if ( Super::RemoveValueByIndex( iIndex ) )
	{
		InvalidateLayout();
		return true;
	}

	return false;
}


UKUITextInterfaceComponent* UKUIScrollSelectWidget::GetTextComponent()
{
	return cmText.Get();
}


void UKUIScrollSelectWidget::OnLeftButtonClick( UKUIButtonWidget* cmButton )
{
	PreviousValue();
}


void UKUIScrollSelectWidget::OnRightButtonClick( UKUIButtonWidget* cmButton )
{
	NextValue();
}


void UKUIScrollSelectWidget::DoLayout()
{
	UpdateLargestTextSize();
	cmText->SetSize( v2LargestTextSize.X, v2LargestTextSize.Y );

	if ( cmLeftButton.IsValid() )
		cmText->SetLocation( cmLeftButton->GetSize().X + cmLeftButton->GetMarginSize().X, 0.f );

	FVector2D v2Size = FVector2D::ZeroVector;

	v2Size.X = v2LargestTextSize.X + cmText->GetMarginSize().X;

	if ( cmLeftButton.IsValid() )
		v2Size.X += cmLeftButton->GetSize().X + cmLeftButton->GetMarginSize().X;

	if ( cmRightButton.IsValid() )
		v2Size.X += cmRightButton->GetSize().X + cmRightButton->GetMarginSize().X;

	v2Size.Y = v2LargestTextSize.Y + cmText->GetMarginSize().Y;

	if ( cmLeftButton.IsValid() )
		v2Size.Y = max( v2Size.Y, cmLeftButton->GetSize().Y + cmLeftButton->GetMarginSize().Y );

	if ( cmRightButton.IsValid() )
		v2Size.Y = max( v2Size.Y, cmRightButton->GetSize().Y + cmRightButton->GetMarginSize().Y );

	SetSizeStruct( v2Size );
	UpdateDisabledStates();

	Super::DoLayout();
}


void UKUIScrollSelectWidget::UpdateLargestTextSize()
{
	v2LargestTextSize = FVector2D::ZeroVector;
	FVector2D v2Current;

	for ( uint8 i = 0; i < arValues.Num(); ++i )
	{
		v2Current = cmText->GetSizeText( arValues[ i ] );
		v2LargestTextSize.X = max( v2LargestTextSize.X, v2Current.X );
		v2LargestTextSize.Y = max( v2LargestTextSize.Y, v2Current.Y );
	}
}


void UKUIScrollSelectWidget::UpdateDisabledStates()
{
	if ( cmLeftButton.IsValid() )
	{
		if ( IsDisabled() || iSelectedValue == KUI_SELECT_WIDGET_NO_VALUE || !arValues.IsValidIndex( iSelectedValue - 1 ) )
			cmLeftButton->SetDisabled( EKUIBool::SV_True );

		else
			cmLeftButton->SetDisabled( EKUIBool::SV_False );
	}

	if ( cmRightButton.IsValid() )
	{
		if ( IsDisabled() || arValues.Num() == 0 || ( iSelectedValue < KUI_SELECT_WIDGET_MAX_VALUE && !arValues.IsValidIndex( iSelectedValue + 1 ) ) )
			cmRightButton->SetDisabled( EKUIBool::SV_True );

		else
			cmRightButton->SetDisabled( EKUIBool::SV_False );
	}
}
