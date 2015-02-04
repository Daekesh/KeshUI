// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "KeshUI.h"
#include "KUIInterfaceElement.h"
#include "KUIInterfaceWidget.h"
#include "KUIInterfaceWidgetChildManager.h"


UKUIInterfaceWidgetChildManager::UKUIInterfaceWidgetChildManager( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	KUI_UO_INIT_DEBUG()

	cmWidget = NULL;
	oChild = NULL;
}


UKUIInterfaceWidget* UKUIInterfaceWidgetChildManager::GetWidget() const
{
	return cmWidget.Get();
}


void UKUIInterfaceWidgetChildManager::SetWidget( UKUIInterfaceWidget* cmWidget )
{
	this->cmWidget = cmWidget;
}


UKUIInterfaceElement* UKUIInterfaceWidgetChildManager::GetChild() const
{
	return oChild.Get();
}


void UKUIInterfaceWidgetChildManager::SetChild( UKUIInterfaceElement* oChild )
{
	this->oChild = oChild;
}


void UKUIInterfaceWidgetChildManager::Update()
{

}
