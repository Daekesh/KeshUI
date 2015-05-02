// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KeshUI/KUIInterfaceContainer.h"
#include "KeshUI/KUIMacros.h"
#include "KUIListContainer.generated.h"

class UKUIListRowContainer;
class UKUIListRowColumnContainer;
class UKUISimpleClickWidget;

#define KUI_LIST_INDEX_NONE 65535
#define KUI_LIST_INDEX_MIN 0
#define KUI_LIST_INDEX_MAX 65534

class UKUIListContainer;

KUI_DECLARE_DELEGATE_OneParam( FKUIListRowContainerSelectionChange, UKUIListContainer* );

/**
 * General container that has list layout (defined columns with a list of elements of fixed/varying size.)
 **/
UCLASS(ClassGroup="KeshUI | Container", BlueprintType, Blueprintable)
class KESHUI_API UKUIListContainer : public UKUIInterfaceContainer
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUIListContainer )

	UKUIListContainer( const class FObjectInitializer& oObjectInitializer );

public:

	/* Returns true if selection is enable. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual bool IsSelectEnabled() const;

	/* Sets whether selection is enabled. If set to false, all rows are unselected. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual void SetSelectEnabled( bool bEnabled );
	
	/* Gets an element at a specific list location. */
	UFUNCTION(Category = "KeshUI | Container | List", BlueprintCallable)
	virtual UKUIListRowContainer* GetRow( uint16 iRow ) const;

	/* Adds an element at a specific list location.  Adds to container if necessary. */
	UFUNCTION(Category="KeshUI | Container | List", BlueprintCallable)
	virtual void SetRow( uint16 iRow, UKUIListRowContainer* ctRow );

	/* Removes an element from the list slot.  Does not remove it from the container. */
	UFUNCTION(Category="KeshUI | Container | List", BlueprintCallable)
	virtual void RemoveRowByRef( UKUIListRowContainer* ctRow );

	/* Removes an element from the list slot.  Does not remove it from the container. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual void RemoveRowByIndex( uint16 iRow );

	virtual void AddChild( UKUIInterfaceElement* oChild ) override;

	/* Removes a child from this container and from the list layout. */
	virtual bool RemoveChild( UKUIInterfaceElement* oChild ) override;

	/* Gets the number of rows contained in this list. */
	UFUNCTION(Category="KeshUI | Container | List", BlueprintCallable)
	virtual uint16 GetRowCount() const;

	/* Adds 1 to the row count. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual uint8 AddRow( UKUIListRowContainer* ctRow = NULL );

	/* Adds a Column list row with the given widths. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual UKUIListRowColumnContainer* AddRowColumns( const TArray<float>& arWidths );

	/* Sets the number of rows in this list. Deletes the elements in removed rows. */
	UFUNCTION(Category="KeshUI | Container | List", BlueprintCallable)
	virtual void SetRowCount( uint16 iRowCount );

	/* Removes the given number of rows from the start position.  Removes the elements in those rows from the container */
	UFUNCTION(Category="KeshUI | Container | List", BlueprintCallable)
	virtual void RemoveRows( uint16 iStart, uint16 iCount );

	/* Gets the minimum row height. */
	UFUNCTION(Category="KeshUI | Container | List", BlueprintCallable)
	virtual float GetMinimumRowHeight() const;

	/* Sets the minimum row height. */
	UFUNCTION(Category="KeshUI | Container | List", BlueprintCallable)
	virtual void SetMinimumRowHeight( float fHeight );

	/* Gets the maximum row height.  Default is 0.f, which removes the minimum limit.  Rows with no elements will be gaps. */
	UFUNCTION(Category="KeshUI | Container | List", BlueprintCallable)
	virtual float GetMaximumRowHeight() const;

	/* Sets the maximum row height. Default is 0.f, which removes the height limitation. */
	UFUNCTION(Category="KeshUI | Container | List", BlueprintCallable)
	virtual void SetMaximumRowHeight( float fHeight );

	/* Sets both the minimum and maximum row height. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual void SetRowHeight( float fHeight );

	/* Gets the space between rows.  Default is 0.f. */
	UFUNCTION(Category="KeshUI | Container | List", BlueprintCallable)
	virtual float GetRowSpacing() const;

	/* Sets the space between rows. */
	UFUNCTION(Category="KeshUI | Container | List", BlueprintCallable)
	virtual void SetRowSpacing( float fSpacing );

	/* Updates the layout of this container. */
	virtual void SetSize( float fWidth, float fHeight ) override;

	/* Returns true if the given row is selected. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual bool IsRowSelected( UKUIListRowContainer* ctRow ) const;

	/* Incoming click event from a row being selected. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual void OnRowSelectionClick( UKUIListRowContainer* ctRow );

	/* Returns true if multi select is enabled. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual bool IsMultiSelectEnabled() const;

	/* Sets whether multi select is enabled. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual void SetMultiSelectEnabled( bool bEnabled );

	/* Returns the first selected row. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual UKUIListRowContainer* GetSelectedRow() const;

	/* Returns the list of selected rows. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual const TArray<TWeakObjectPtr<UKUIListRowContainer>>& GetSelectedRows() const;

	/* Returns true if the row is selected. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual bool IsRowSelectedByRef( UKUIListRowContainer* ctRow ) const;

	/* Returns true if the row is selected. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual bool IsRowSelectedByIndex( uint16 iRow ) const;

	/* Returns true if the rows are selected. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual bool AreRowsSelectedByRef( const TArray<UKUIListRowContainer*>& arRows ) const;

	/* Returns true if the rows are selected. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual bool AreRowsSelectedByIndex( const TArray<uint16>& arRows ) const;

	/* Sets the selected row. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual void SetSelectedRowByRef( UKUIListRowContainer* ctRow );

	/* Sets the selected row by index. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual void SetSelectedRowByIndex( uint16 iRow );

	/* Sets the selected rows by reference. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual void SetSelectedRowsByRef( const TArray<UKUIListRowContainer*>& arRows );

	/* Sets the selected rows by index. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual void SetSelectedRowsByIndex( const TArray<uint16>& arRows );

	/* Adds row to selection. If multi select not allowed, sets if none selected. Return true if added. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual bool AddSelectedRowByRef( UKUIListRowContainer* ctRow );

	/* Adds row to selection by index. If multi select not allowed, sets if none selected. Return true if added. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual bool AddSelectedRowByIndex( uint16 iRow );

	/* Adds rows to selection. If multi select not allowed, sets to first if none selected. Return true if at least 1 row was added or was already selected. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual bool AddSelectedRowsByRef( const TArray<UKUIListRowContainer*>& arRows );

	/* Adds rows to selection by index. If multi select not allowed, sets to first if none selected. Return true if at least 1 row was added or was already selected. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual bool AddSelectedRowsByIndex( const TArray<uint16>& arRows );

	/* Removes row to selection. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual void RemoveSelectedRowByRef( UKUIListRowContainer* ctRow );

	/* Removes row to selection by index. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual void RemoveSelectedRowByIndex( uint16 iRow );

	/* Removes rows to selection. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual void RemoveSelectedRowsByRef( const TArray<UKUIListRowContainer*>& arRows );

	/* Removes rows to selection by index. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual void RemoveSelectedRowsByIndex( const TArray<uint16>& arRows );

	/* Returns the index of the first selected row. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual uint16 GetSelectedRowIndex() const;

	/* Returns an array of the selected indices. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual TArray<uint16> GetSelectedRowIndices() const;

	/* Triggers when the list container is clicked. */
	UFUNCTION( Category = "KeshUI | Container | List", BlueprintCallable )
	virtual void OnClick( UKUISimpleClickWidget* oWidget, const FVector2D& v2CursorOffset );

	/* Sets the delegate called when a selection change is made. */
	virtual void SetSelectionChangeDelegate( UObject* oObject, FKUIListRowContainerSelectionChangePrototype fnSelectionChangeDelegate );

protected:

	TArray<TWeakObjectPtr<UKUIListRowContainer>> arRows;
	uint16 iRowCount;
	float fSpacing;
	float fMinHeight;
	float fMaxHeight;
	bool bEnableMultiSelect;
	bool bEnableSelect;
	TWeakObjectPtr<UKUIListRowContainer> ctLastSelected;
	TArray<TWeakObjectPtr<UKUIListRowContainer>> arSelectedRows;
	FKUIListRowContainerSelectionChangeDelegate dgSelectionChange;

	UPROPERTY()
	UKUISimpleClickWidget* cmClickArea;

	virtual void OnChildSizeChange( const FKUIInterfaceContainerElementEvent& stEventInfo ) override;

	/* Lays out the list elements. */
	virtual void DoLayout() override;

	virtual uint16 GetRowIndexByRef( UKUIListRowContainer* ctRow ) const;
	virtual uint16 GetSelectedRowIndexByRef( UKUIListRowContainer* ctRow ) const;

};
