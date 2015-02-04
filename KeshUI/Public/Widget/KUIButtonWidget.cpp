// Copyright 2014-2015 Matt Chapman. All Rights Reserved.


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
