// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#include "KeshUI/KeshUI.h"
#include "KeshUI/Container/KUICursorContainer.h"


UKUICursorContainer::UKUICursorContainer( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{

}


bool UKUICursorContainer::CanReceieveKeyEvents() const
{
	return false;
}
