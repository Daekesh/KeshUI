// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Container/KUIRootContainer.h"
#include "KUIMacros.h"
#include "KUITooltipContainer.generated.h"

class AKUIInterface;

/**
 * The container used for cursors.
 */
UCLASS(ClassGroup="KeshUI | Container", BlueprintType, Blueprintable)
class KESHUI_API UKUITooltipContainer : public UKUIRootContainer
{
	GENERATED_UCLASS_BODY()
	KUI_CLASS_HEADER( UKUITooltipContainer )

public:

	/* Overriden to false. */
	virtual bool CanReceieveKeyEvents() const;

};
