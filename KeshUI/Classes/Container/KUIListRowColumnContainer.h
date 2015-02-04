// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Container/KUIListRowContainer.h"
#include "KUIMacros.h"
#include "KUIListRowColumnContainer.generated.h"


/**
 * List column class that implements a column layout.
 **/
UCLASS(ClassGroup="KeshUI | Container", BlueprintType, Blueprintable)
class KESHUI_API UKUIListRowColumnContainer : public UKUIListRowContainer
{
	GENERATED_UCLASS_BODY()
	KUI_CLASS_HEADER( UKUIListRowColumnContainer )

public:
	
	/* Gets an element at a specific list location. */
	UFUNCTION(Category = "KeshUI | Container | List", BlueprintCallable)
	virtual UKUIInterfaceElement* GetColumnElement( uint16 iColumn ) const;

	/* Adds an element at a specific list location.  Adds to container if necessary. */
	UFUNCTION(Category="KeshUI | Container | List", BlueprintCallable)
	virtual void SetColumnElement( uint16 iColumn, UKUIInterfaceElement* oElement );

	/* Removes an element from the list slot.  Does not remove it from the container. */
	UFUNCTION(Category="KeshUI | Container | List", BlueprintCallable)
	virtual void RemoveColumnElementByRef( UKUIInterfaceElement* oElement );

	/* Removes an element from the list slot.  Does not remove it from the container. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual void RemoveColumnElementByIndex( uint16 iColumn );

	/* Removes a child from this container and from the list layout. */
	virtual bool RemoveChild( UKUIInterfaceElement* oChild ) override;

	/* Returns true for everything added to a list location. */
	virtual bool IsChildsLayoutManaged( UKUIInterfaceElement* oChild ) const override;

	/* Gets the number of columns contained in this list. */
	UFUNCTION(Category="KeshUI | Container | List", BlueprintCallable)
	virtual uint16 GetColumnCount() const;

	/* Adds 1 to the column count. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual uint8 AddColumn( float fWidth = 0.f, UKUIInterfaceElement* oElement = NULL );

	/* Sets the number of columns in this list. Deletes the elements in removed columns. */
	UFUNCTION(Category="KeshUI | Container | List", BlueprintCallable)
	virtual void SetColumnCount( uint16 iColumnCount );

	/* Removes the given number of columns from the start position.  Removes the elements in those columns from the container */
	UFUNCTION(Category="KeshUI | Container | List", BlueprintCallable)
	virtual void RemoveColumns( uint16 iStart, uint16 iCount );

	/* Returns a column's width. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual float GetColumnWidth( uint16 iColumn ) const;
	
	/* Sets a column's width. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual void SetColumnWidth( uint16 iColumn, float fHeight );

	/* Gets the space between columns.  Default is 0.f. */
	UFUNCTION(Category="KeshUI | Container | List", BlueprintCallable)
	virtual float GetColumnSpacing() const;

	/* Sets the space between columns. */
	UFUNCTION(Category="KeshUI | Container | List", BlueprintCallable)
	virtual void SetColumnSpacing( float fSpacing );

	virtual void DoLayout() override;

protected:

	TArray<TWeakObjectPtr<UKUIInterfaceElement>> arColumnElements;
	TArray<float> arColumnWidths;
	uint16 iColumnCount;
	float fSpacing;

	virtual void OnChildSizeChange( const FKUIInterfaceContainerElementEvent& stEventInfo ) override;

	virtual uint16 GetColumnElementIndexByRef( UKUIInterfaceElement* oElement ) const;

};
