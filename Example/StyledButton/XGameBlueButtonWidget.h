// Copyright 2015 Matt Chapman, Inc. All Rights Reserved.

#pragma once

#include <Widget/KUIComponentButtonWidget.h>
#include "XGameBlueButtonWidget.generated.h"


/* Button component using blue textures. */
UCLASS( Category = "X | UI | Widget", BlueprintType, Blueprintable )
class UXGameBlueButtonWidget : public UKUIComponentButtonWidget
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UXGameBlueButtonWidget )

	UXGameBlueButtonWidget( const class FObjectInitializer& oObjectInitializer );

};
