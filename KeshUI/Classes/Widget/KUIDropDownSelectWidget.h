// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "Widget/KUISelectWidget.h"
#include "KUIMacros.h"
#include "KUIDropDownSelectWidget.generated.h"


/* Combo box widget base class. */
UCLASS( ClassGroup = "KeshUI | Widget", BlueprintType, Blueprintable )
class KESHUI_API UKUIDropDownSelectWidget : public UKUISelectWidget
{
	GENERATED_UCLASS_BODY()
	KUI_CLASS_HEADER( UKUIDropDownSelectWidget )

};
