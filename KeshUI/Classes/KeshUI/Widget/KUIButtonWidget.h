// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KeshUI/Widget/KUISimpleClickWidget.h"
#include "KeshUI/KUIMacros.h"
#include "KUIButtonWidget.generated.h"


/* Base class for KeshUI buttons. */
UCLASS(ClassGroup="KeshUI | Widget", BlueprintType, Blueprintable)
class KESHUI_API UKUIButtonWidget : public UKUISimpleClickWidget
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUIButtonWidget )

	UKUIButtonWidget( const class FObjectInitializer& oObjectInitializer );

public:

	/* Overrides to true so space can click the button. */
	virtual bool CanReceieveKeyEvents() const;

};
