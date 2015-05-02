// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KeshUI/KUIInterfaceContainer.h"
#include "KeshUI/KUIMacros.h"
#include "KUIPagedContainer.generated.h"


#define KUI_PAGE_MAX 254
#define KUI_PAGE_NONE 255

USTRUCT( BlueprintType )
struct FKUIInterfaceContainerPageChangeEvent : public FKUIInterfaceEvent
{
	GENERATED_USTRUCT_BODY();

	FKUIInterfaceContainerPageChangeEvent() : FKUIInterfaceEvent() { }

	FKUIInterfaceContainerPageChangeEvent( uint8 iEventID, uint8 iOldPageIndex, uint8 iNewPageIndex )
		: FKUIInterfaceEvent( iEventID )
	{
		this->iOldPageIndex = iOldPageIndex;
		this->iNewPageIndex = iNewPageIndex;
	}

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "Old Page Index" ) )
	uint8 iOldPageIndex;

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "New Page Index" ) )
	uint8 iNewPageIndex;
};


/**
 * General container that can page between sub containers.
 **/
UCLASS(ClassGroup="KeshUI | Container", BlueprintType, Blueprintable)
class KESHUI_API UKUIPagedContainer : public UKUIInterfaceContainer
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUIPagedContainer )

	UKUIPagedContainer( const class FObjectInitializer& oObjectInitializer );

public:

	/* Adds a page. Returns the page index.  May replace old index. */
	UFUNCTION(Category="KeshUI | Container | Paged", BlueprintCallable)
	virtual uint8 AddPage( UKUIInterfaceElement* oPage );

	/* Returns a page by index. */
	UFUNCTION( Category = "KeshUI | Container | Paged", BlueprintCallable )
	virtual UKUIInterfaceElement* GetPage( uint8 iIndex ) const;

	/* Removes a page by reference. */
	UFUNCTION(Category="KeshUI | Container | Paged", BlueprintCallable)
	virtual void RemovePageByReference( UKUIInterfaceElement* oPage, bool bRemoveChild = true );

	/* Removes a page by index. */
	UFUNCTION(Category="KeshUI | Container | Paged", BlueprintCallable)
	virtual void RemovePageByIndex( uint8 iIndex, bool bRemoveChild = true );

	/* Removes a child from this container and from the grid layout. */
	virtual bool RemoveChild( UKUIInterfaceElement* oChild ) override;

	/* Gets the currently active page index. Returns 255 if no active page. */
	UFUNCTION(Category="KeshUI | Container | Paged", BlueprintCallable)
	virtual uint8 GetActivePageIndex() const;

	/* Gets the currently active page. */
	UFUNCTION(Category="KeshUI | Container | Paged", BlueprintCallable)
	virtual UKUIInterfaceElement* GetActivePage() const;

	/* Set active page by reference. */
	UFUNCTION(Category="KeshUI | Container | Paged", BlueprintCallable)
	virtual void SetActivePageByReference( UKUIInterfaceElement* oPage );
	
	/* Sets the active page by index. */
	UFUNCTION(Category="KeshUI | Container | Paged", BlueprintCallable)
	virtual void SetActivePageByIndex( uint8 iIndex );

	virtual bool RespondsToEvent( uint8 iEventID ) const override;

	virtual void SendEvent( FKUIInterfaceEvent& stEventInfo ) override;

protected:

	TArray<TWeakObjectPtr<UKUIInterfaceElement>> arPages;
	uint8 iActivePage;

	/* Triggers when the page is changed. */
	virtual void OnActivePageChange( uint8 iOldIndex, uint8 iNewIndex );

	/* Called before the active page is changed. */
	UFUNCTION(Category="KeshUI | Container | Paged", BlueprintImplementableEvent)
	virtual void OnActivePageChangeBP( uint8 iOldIndex, uint8 iNewIndex );

};
