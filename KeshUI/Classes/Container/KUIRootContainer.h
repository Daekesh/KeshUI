// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KUIInterfaceContainer.h"
#include "KUIMacros.h"
#include "KUIRootContainer.generated.h"

class AKUIInterface;

/**
 * The root container used in the interface.
 */
UCLASS(ClassGroup="KeshUI | Container", BlueprintType, Blueprintable)
class KESHUI_API UKUIRootContainer : public UKUIInterfaceContainer
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUIRootContainer )

	UKUIRootContainer( const class FObjectInitializer& oObjectInitializer );

public:

	/* Gets the base interface this container is in. */
	virtual AKUIInterface* GetInterface() const override;

	/* Sets the base interface this container is in. */
	UFUNCTION(Category="KeshUI | Container | Root", BlueprintCallable)
	virtual void SetInterface( AKUIInterface* aInterface );

	/* Returns true if this is visible. */
	virtual bool IsVisibleRecursive() const override;

protected:

	TWeakObjectPtr<AKUIInterface> aInterface;

	/* Sets the root container to the size of the screen. */
	virtual void OnScreenResolutionChange( const FKUIInterfaceContainerScreenResolutionEvent& stEventInfo );

};
