// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Widget/KUISimpleClickWidget.h"
#include "KUIMacros.h"
#include "KUIButtonWidget.generated.h"


/* Base class for KeshUI buttons. */
UCLASS(ClassGroup="KeshUI | Widget", BlueprintType, Blueprintable)
class KESHUI_API UKUIButtonWidget : public UKUISimpleClickWidget
{
	GENERATED_UCLASS_BODY()
	KUI_CLASS_HEADER( UKUIButtonWidget )

public:

	/* Overrides to true so space can click the button. */
	virtual bool CanReceieveKeyEvents() const;

};
