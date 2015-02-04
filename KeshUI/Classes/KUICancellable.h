// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KUICancellable.generated.h"

/* Dummy class because UE4. */
UINTERFACE(MinimalAPI)
class UKUICancellable : public UInterface
{
	GENERATED_UINTERFACE_BODY()

};


class KESHUI_API IKUICancellable
{
	GENERATED_IINTERFACE_BODY()

	virtual void Cancel();

};
