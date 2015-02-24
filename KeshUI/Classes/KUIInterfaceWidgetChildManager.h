// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KUIMacros.h"
#include "KUIInterfaceWidgetChildManager.generated.h"

class UKUIInterfaceElement;
class UKUIInterfaceWidget;


/**
 * The base widget child info class.  Used by widgets to control child elements.  Location, visibility, etc.
 */
UCLASS(ClassGroup="KeshUI | Widget | Child Manager", BlueprintType, Blueprintable)
class KESHUI_API UKUIInterfaceWidgetChildManager : public UObject
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUIInterfaceWidgetChildManager )

	UKUIInterfaceWidgetChildManager( const class FObjectInitializer& oObjectInitializer );

public:

	/* Returns the widget this is a component of. */
	UFUNCTION(Category="KeshUI | Widget | Child Manager", BlueprintCallable)
	virtual UKUIInterfaceWidget* GetWidget() const;

	/* Sets the widget this is a component of. */
	UFUNCTION(Category="KeshUI | Widget | Child Manager", BlueprintCallable)
	virtual void SetWidget( UKUIInterfaceWidget* cmWidget );

	/* Gets the component in this element. */
	UFUNCTION(Category="KeshUI | Widget | Child Manager", BlueprintCallable)
	virtual UKUIInterfaceElement* GetChild() const;

	/* Sets the component in this element. */
	UFUNCTION(Category="KeshUI | Widget | Child Manager", BlueprintCallable)
	virtual void SetChild( UKUIInterfaceElement* oChild );

	/* Called to update the widget component settings. */
	virtual void Update();

protected:

	TWeakObjectPtr<UKUIInterfaceWidget> cmWidget;
	TWeakObjectPtr<UKUIInterfaceElement> oChild;

};
