// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.


#include "KeshUI.h"
#include "Widget/KUIButtonWidget.h"


UKUIButtonWidget::UKUIButtonWidget( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{

}


bool UKUIButtonWidget::CanReceieveKeyEvents() const
{
	return true;
}
