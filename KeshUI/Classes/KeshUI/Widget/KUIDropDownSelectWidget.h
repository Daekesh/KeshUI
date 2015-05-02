// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KeshUI/Widget/KUISelectWidget.h"
#include "KeshUI/KUIMacros.h"
#include "KUIDropDownSelectWidget.generated.h"


/* Combo box widget base class. */
UCLASS( ClassGroup = "KeshUI | Widget", BlueprintType, Blueprintable )
class KESHUI_API UKUIDropDownSelectWidget : public UKUISelectWidget
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUIDropDownSelectWidget )

	UKUIDropDownSelectWidget( const class FObjectInitializer& oObjectInitializer );

};
