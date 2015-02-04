// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KUIInterfaceContainer.h"
#include "KUIMacros.h"
#include "KUIListRowContainer.generated.h"

class UKUITextureInterfaceComponent;


/**
* A list row class. Has select click events and row highlight.
*/
UCLASS( ClassGroup = "KeshUI", BlueprintType, Blueprintable )
class KESHUI_API UKUIListRowContainer : public UKUIInterfaceContainer
{
	GENERATED_UCLASS_BODY()
	KUI_CLASS_HEADER( UKUIListRowContainer )

public:

	/* Gets whether this row is selected or not. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual bool IsSelected() const;

	/* Called when the selection value of this row is changed. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual void UpdateSelected( bool bSelected );

	/* Gets the unselected foreground colour. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual FColor GetUnselectedForegroundColor() const;

	/* Sets the unselected foreground colour. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual void SetUnselectedForegroundColor( const FColor& stColor );

	/* Gets the selected foreground colour. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual FColor GetSelectedForegroundColor() const;

	/* Sets the selected foreground colour. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual void SetSelectedForegroundColor( const FColor& stColor );

	/* Gets the unselected background colour. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual FColor GetUnselectedBackgroundColor() const;

	/* Sets the unselected background colour. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual void SetUnselectedBackgroundColor( const FColor& stColor );

	/* Gets the selected background colour. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual FColor GetSelectedBackgroundColor() const;

	/* Sets the selected background colour. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual void SetSelectedBackgroundColor( const FColor& stColor );

	/* Updates selected status. */
	virtual void OnAddedToContainer( const FKUIInterfaceElementContainerEvent& stEventInfo ) override;

	/* Updates selected status. */
	virtual void OnRemovedFromContainer( const FKUIInterfaceElementContainerEvent& stEventInfo ) override;

	virtual void DoLayout() override;

protected:

	bool bSelectedCache;

	FColor stUnselectedForegroundColor;
	FColor stUnselectedBackgroundColor;
	FColor stSelectedForegroundColor;
	FColor stSelectedBackgroundColor;

	UPROPERTY()
	UKUITextureInterfaceComponent* cmBackground;

	/* Updates all sub text components with the new color. */
	virtual void UpdateComponents();

	virtual void UpdateComponents( UKUIInterfaceContainer* ctContainer );

};
