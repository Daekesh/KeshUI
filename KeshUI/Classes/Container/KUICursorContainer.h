// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "Container/KUIRootContainer.h"
#include "KUIMacros.h"
#include "KUICursorContainer.generated.h"

class AKUIInterface;

/**
 * The container used for cursors.
 */
UCLASS(ClassGroup="KeshUI | Container", BlueprintType, Blueprintable)
class KESHUI_API UKUICursorContainer : public UKUIRootContainer
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUICursorContainer )

	UKUICursorContainer( const class FObjectInitializer& oObjectInitializer );

public:

	/* Overriden to false. */
	virtual bool CanReceieveKeyEvents() const;

};
