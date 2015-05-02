// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KeshUI/KUIInterfaceElement.h"
#include "KeshUI/KUIMacros.h"
#include "GenericApplicationMessageHandler.h"
#include "KUIInterfaceContainer.generated.h"


class UKUIInterfaceWidgetChildManager;


UENUM( BlueprintType )
namespace EKUIInterfaceContainerEventList
{
	enum Event
	{
		E_Tick = 16                    UMETA( DisplayName = "Tick" ),
		E_MouseMove = 17               UMETA( DisplayName = "Mouse Move" ),
		E_MouseButtonDown = 18         UMETA( DisplayName = "Mouse Button Down" ),
		E_MouseButtonUp = 19           UMETA( DisplayName = "Mouse Button Up" ),
		E_KeyDown = 20                 UMETA( DisplayName = "Key Down" ),
		E_KeyUp = 21                   UMETA( DisplayName = "Key Up" ),
		E_KeyRepeat = 22               UMETA( DisplayName = "Key Repeat" ),
		E_KeyChar = 23                 UMETA( DisplayName = "Char Input" ),
		E_ScreenResolutionChange = 24  UMETA( DisplayName = "Screen Resolution Change" ),
		E_MatchStart = 25              UMETA( DisplayName = "Match Start" ),
		E_MatchEnd = 26                UMETA( DisplayName = "Match End" ),
		E_MatchPaused = 27             UMETA( DisplayName = "Match Paused" ),
		E_MatchUnpaused = 28           UMETA( DisplayName = "Match Unpaused" ),
		E_PlayerDeath = 29             UMETA( DisplayName = "Player Death" ),
		E_VisibilityChange = 30        UMETA( DisplayName = "Visibility Change" ),
		E_Focus = 31                   UMETA( DisplayName = "Focus Gained" ),
		E_Blur = 32                    UMETA( DisplayName = "Focus Lost (Blur)" ),
		E_FocusChange = 33             UMETA( DisplayName = "Focus Changed" ),
		E_ChildSizeChange = 34         UMETA( DisplayName = "Child Size Change" ),
		E_ChildLocationChange = 35     UMETA( DisplayName = "Child Location Change" ),
		E_MatchStateChange = 36        UMETA( DisplayName = "Match State Change" ),
		E_LayoutInvalidated = 37       UMETA( DisplayName = "Layout Invalidated" ),
		E_LayoutComplete = 38          UMETA( DisplayName = "Layout Complete" ),
		E_ChildManagersUpdated = 39    UMETA( DisplayName = "Child Managers Updated" ),
		E_ChildAdded = 40              UMETA( DisplayName = "Child Added" ),
		E_ChildRemoved = 41            UMETA( DisplayName = "Child Removed" ),
		E_PageChanged = 42             UMETA( DisplayName = "Page Changed" )
	};
}

#define KUI_CONTAINER_EVENT_FIRST EKUIInterfaceContainerEventList::E_Tick
#define KUI_CONTAINER_EVENT_LAST EKUIInterfaceContainerEventList::E_ChildRemoved

USTRUCT( BlueprintType )
struct FKUIInterfaceContainerTickEvent : public FKUIInterfaceEvent
{
	GENERATED_USTRUCT_BODY();

	FKUIInterfaceContainerTickEvent() : FKUIInterfaceEvent() { }

	FKUIInterfaceContainerTickEvent( uint8 iEventID, float fDeltaTime )
		: FKUIInterfaceEvent( iEventID )
	{
		this->fDeltaTime = fDeltaTime;
	}

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "Delta Time" ) )
	float fDeltaTime;
};

USTRUCT( BlueprintType )
struct FKUIInterfaceContainerHandleableEvent : public FKUIInterfaceEvent
{
	GENERATED_USTRUCT_BODY();

	FKUIInterfaceContainerHandleableEvent() : FKUIInterfaceEvent() { }

	FKUIInterfaceContainerHandleableEvent( uint8 iEventID, bool bHandled )
		: FKUIInterfaceEvent( iEventID )
	{
		this->bHandled = bHandled;
	}

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "Handled" ) )
	bool bHandled;
};

USTRUCT( BlueprintType )
struct FKUIInterfaceContainerMouseLocationEvent : public FKUIInterfaceEvent
{
	GENERATED_USTRUCT_BODY();

	FKUIInterfaceContainerMouseLocationEvent() : FKUIInterfaceEvent() { }

	FKUIInterfaceContainerMouseLocationEvent( uint8 iEventID, const FVector2D& v2OldLocation, const FVector2D& v2NewLocation )
		: FKUIInterfaceEvent( iEventID )
	{
		this->v2OldLocation = v2OldLocation;
		this->v2NewLocation = v2NewLocation;
	}

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "Old Location" ) )
	FVector2D v2OldLocation;

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "New Location" ) )
	FVector2D v2NewLocation;
};

USTRUCT( BlueprintType )
struct FKUIInterfaceContainerMouseButtonEvent : public FKUIInterfaceContainerHandleableEvent
{
	GENERATED_USTRUCT_BODY();

	FKUIInterfaceContainerMouseButtonEvent() : FKUIInterfaceContainerHandleableEvent() { }

	FKUIInterfaceContainerMouseButtonEvent( uint8 iEventID, bool bHandled, uint8 eButton, const FVector2D& v2Location )
		: FKUIInterfaceContainerHandleableEvent( iEventID, bHandled )
	{
		this->eButton = eButton;
		this->v2Location = v2Location;
	}

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "Mouse Button" ) )
	uint8 eButton;

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "Location" ) )
	FVector2D v2Location;
};

USTRUCT( BlueprintType )
struct FKUIInterfaceContainerKeyEvent : public FKUIInterfaceContainerHandleableEvent
{
	GENERATED_USTRUCT_BODY();

	FKUIInterfaceContainerKeyEvent() : FKUIInterfaceContainerHandleableEvent() { }

	FKUIInterfaceContainerKeyEvent( uint8 iEventID, bool bHandled, FKey eKey )
		: FKUIInterfaceContainerHandleableEvent( iEventID, bHandled )
	{
		this->eKey = eKey;
	}

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "Key" ) )
	FKey eKey;
};

USTRUCT( BlueprintType )
struct FKUIInterfaceContainerCharEvent : public FKUIInterfaceContainerHandleableEvent
{
	GENERATED_USTRUCT_BODY();

	FKUIInterfaceContainerCharEvent() : FKUIInterfaceContainerHandleableEvent() { }

	FKUIInterfaceContainerCharEvent( uint8 iEventID, bool bHandled, TCHAR chChar )
		: FKUIInterfaceContainerHandleableEvent( iEventID, bHandled )
	{
		this->chChar = chChar;

		strChar = FString::Chr( chChar );
	}

	TCHAR chChar;

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "Char" ) )
	FString strChar;
};

USTRUCT( BlueprintType )
struct FKUIInterfaceContainerScreenResolutionEvent : public FKUIInterfaceEvent
{
	GENERATED_USTRUCT_BODY();

	FKUIInterfaceContainerScreenResolutionEvent() : FKUIInterfaceEvent() { }

	FKUIInterfaceContainerScreenResolutionEvent( uint8 iEventID, const FVector2D& v2OldResolution, const FVector2D& v2NewResolution )
		: FKUIInterfaceEvent( iEventID )
	{
		this->v2OldResolution = v2OldResolution;
		this->v2NewResolution = v2NewResolution;
	}

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "Old Screen Resolution" ) )
	FVector2D v2OldResolution;

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "New Screen Resolution" ) )
	FVector2D v2NewResolution;
};

USTRUCT( BlueprintType )
struct FKUIInterfaceContainerPlayerEvent : public FKUIInterfaceEvent
{
	GENERATED_USTRUCT_BODY();

	FKUIInterfaceContainerPlayerEvent() : FKUIInterfaceEvent() { }

	FKUIInterfaceContainerPlayerEvent( uint8 iEventID, uint32 iPlayerId )
		: FKUIInterfaceEvent( iEventID )
	{
		this->iPlayerId = iPlayerId;
	}

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "Player ID" ) )
	uint32 iPlayerId;
};

USTRUCT( BlueprintType )
struct FKUIInterfaceContainerVisibilityEvent : public FKUIInterfaceEvent
{
	GENERATED_USTRUCT_BODY();

	FKUIInterfaceContainerVisibilityEvent() : FKUIInterfaceEvent() { }

	FKUIInterfaceContainerVisibilityEvent( uint8 iEventID, bool bVisible )
		: FKUIInterfaceEvent( iEventID )
	{
		this->bVisible = bVisible;
	}

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "Is Visible" ) )
	bool bVisible;
};

USTRUCT( BlueprintType )
struct FKUIInterfaceContainerElementEvent : public FKUIInterfaceEvent
{
	GENERATED_USTRUCT_BODY();

	FKUIInterfaceContainerElementEvent() : FKUIInterfaceEvent() { }

	FKUIInterfaceContainerElementEvent( uint8 iEventID, UKUIInterfaceElement* oElement )
		: FKUIInterfaceEvent( iEventID )
	{
		this->oElement = oElement;
	}

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "Child Element" ) )
		UKUIInterfaceElement* oElement;
};

USTRUCT( BlueprintType )
struct FKUIInterfaceContainerMatchStateEvent : public FKUIInterfaceEvent
{
	GENERATED_USTRUCT_BODY();

	FKUIInterfaceContainerMatchStateEvent() : FKUIInterfaceEvent() { }

	FKUIInterfaceContainerMatchStateEvent( uint8 iEventID, const FName& nMatchState )
		: FKUIInterfaceEvent( iEventID )
	{
		this->nMatchState = nMatchState;
	}

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "Match State" ) )
	FName nMatchState;
};

/**
 * The base KeshUI class for all ui widgets and panels.
 */
UCLASS(ClassGroup="KeshUI", BlueprintType, Blueprintable)
class KESHUI_API UKUIInterfaceContainer : public UKUIInterfaceElement
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUIInterfaceContainer )

	UKUIInterfaceContainer( const class FObjectInitializer& oObjectInitializer );

public:

	/* Gets the size of this element. */
	virtual const FVector2D& GetSize() const override;

	/* Sets the size of the component. */
	UFUNCTION(Category="KeshUI | Container", BlueprintCallable)
	virtual void SetSizeStruct( const FVector2D& v2Size );

	/* Sets the size of the component. */
	virtual void SetSize( float fWidth, float fHeight ) override;

	/* Adds a child to this container. */
	UFUNCTION(Category="KeshUI | Container", BlueprintCallable)
	virtual void AddChild( UKUIInterfaceElement* oChild );

	/* Removes a child from this container. */
	UFUNCTION(Category="KeshUI | Container", BlueprintCallable)
	virtual bool RemoveChild( UKUIInterfaceElement* oChild );

	/* Returns the child at the given index. */
	UFUNCTION( Category = "KeshUI | Container", BlueprintCallable )
	virtual UKUIInterfaceElement* GetChildAtIndex( int32 iIndex ) const;

	/* Returns an iterator for the child components. */
	virtual TArray<UKUIInterfaceElement*>::TIterator GetChildIterator();

	/* Re-order the components according to Z-Index. */
	UFUNCTION(Category="KeshUI | Container", BlueprintCallable)
	virtual void SortChildren();

	/* Returns whether this container can receive mouse events. */
	UFUNCTION(Category="KeshUI | Container", BlueprintCallable)
	virtual bool CanReceieveMouseEvents() const;

	/* Returns whether this container can receive key events. */
	UFUNCTION(Category="KeshUI | Container", BlueprintCallable)
	virtual bool CanReceieveKeyEvents() const;

	/* Sets whether this element is visible. */
	virtual void SetVisible( bool bVisible ) override;

	/* Called to render the container on the screen. */
	virtual void Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject = NULL ) override;

	/* Returns the number of tick requests. */
	virtual int16 GetTickRequests() const;

	/* Returns the number of mouse input requests. */
	virtual int16 GetMouseInputRequests() const;

	/* Returns the number of key input events. */
	virtual int16 GetKeyInputRequests() const;

	/* Adds a child Manager. */
	UFUNCTION(Category="KeshUI | Container", BlueprintCallable)
	virtual void AddChildManager( UKUIInterfaceWidgetChildManager* oChildManager );

	/* Removes a child Manager. */
	UFUNCTION(Category="KeshUI | Container", BlueprintCallable)
	virtual void RemoveChildManager( UKUIInterfaceWidgetChildManager* oChildManager );
		
	/* Calls update on all child Managers. */
	UFUNCTION(Category="KeshUI | Container", BlueprintCallable)
	virtual void UpdateChildManagers();

	/* Returns true if this manages the given child's layout. */
	UFUNCTION(Category="KeshUI | Container", BlueprintCallable)
	virtual bool IsChildsLayoutManaged( UKUIInterfaceElement* oChild ) const;

	/* Returns true if the layout is valid and up to date. */
	UFUNCTION( Category = "KeshUI | Container", BlueprintCallable )
	virtual bool HasValidLayout() const;

	/* Invalidates the layout, so it can be reconstructed at the next render. */
	UFUNCTION( Category = "KeshUI | Container", BlueprintCallable )
	virtual void InvalidateLayout();

	/* Lays out the elements in this container. */
	UFUNCTION( Category = "KeshUI | Container", BlueprintCallable )
	virtual void DoLayout();

	/* Returns true if we respond to this event. */
	virtual bool RespondsToEvent( uint8 iEventID ) const override;

	/* Dispatches the event. */
	virtual void SendEvent( FKUIInterfaceEvent& stEventInfo ) override;

	/* Broadcasts events to children. */
	virtual void BroadcastEvent( FKUIInterfaceEvent& stEventInfo, bool bTopDown = false );

protected:

	UPROPERTY() // Ensures reference counts.
	TArray<UKUIInterfaceElement*> arChildren;

	FVector2D v2Size;
	bool bValidLayout;
	int16 iTickRequests;
	int16 iMouseInputRequests;
	int16 iKeyInputRequests;
	bool bFocused;

	UPROPERTY()
	TArray<UKUIInterfaceWidgetChildManager*> arChildManagers;

	/* Returns true if this container can tick. */
	virtual bool CanTick() const;

	/* Adds one to the number of tick requests.  And parents. */
	virtual void AddTickRequests( int16 iCount );

	/* Removes one from the number of tick requests.  And parents. */
	virtual void RemoveTickRequests( int16 iCount );

	/* Adds one to the number of mouse input requests.  And parents. */
	virtual void AddMouseInputRequests( int16 iCount );

	/* Removes one from the number of mouse input requests.  And parents. */
	virtual void RemoveMouseInputRequests( int16 iCount );

	/* Adds one to the number of key input requests.  And parents. */
	virtual void AddKeyInputRequests( int16 iCount );

	/* Removes one from the number of key input request.  And parents. */
	virtual void RemoveKeyInputRequests( int16 iCount );

	/* Ticks the container. */
	virtual void OnTick( const FKUIInterfaceContainerTickEvent& stEventInfo );

	/* Triggers when the mouse is moved. */
	virtual void OnMouseMove( const FKUIInterfaceContainerMouseLocationEvent& stEventInfo );

	/* Triggers when a mouse button is pressed. */
	virtual bool OnMouseButtonDown( const FKUIInterfaceContainerMouseButtonEvent& stEventInfo );

	/* Triggers when a mouse button is released. */
	virtual bool OnMouseButtonUp( const FKUIInterfaceContainerMouseButtonEvent& stEventInfo );

	/* Triggers when a key is pressed. */
	virtual bool OnKeyDown( const FKUIInterfaceContainerKeyEvent& stEventInfo );

	/* Triggers when a key is released. */
	virtual bool OnKeyUp( const FKUIInterfaceContainerKeyEvent& stEventInfo );

	/* Triggers when a key press is repeated. */
	virtual bool OnKeyRepeat( const FKUIInterfaceContainerKeyEvent& stEventInfo );

	/* Triggers when a char is pressed on the keyboard. */
	virtual bool OnKeyChar( const FKUIInterfaceContainerCharEvent& stEventInfo );

	/* Triggers when the screen resolution changes. */
	virtual void OnScreenResolutionChange( const FKUIInterfaceContainerScreenResolutionEvent& stEventInfo );

	/* Triggers when the match starts. */
	virtual void OnMatchStart( const FKUIInterfaceEvent& stEventInfo );

	/* Triggers when the match ends. */
	virtual void OnMatchEnd( const FKUIInterfaceEvent& stEventInfo );

	/* Triggers when the match is paused. */
	virtual void OnMatchPaused( const FKUIInterfaceContainerPlayerEvent& stEventInfo );

	/* Triggers when the match is unpaused. */
	virtual void OnMatchUnpaused( const FKUIInterfaceContainerPlayerEvent& stEventInfo );

	/* Triggers when a player dies. */
	virtual void OnPlayerDeath( const FKUIInterfaceContainerPlayerEvent& stEventInfo );

	/* Triggers when the visibility of this element changes. */
	virtual void OnVisibilityChange( const FKUIInterfaceContainerVisibilityEvent& stEventInfo );

	/* Triggers when this element is focused. */
	virtual void OnFocus( const FKUIInterfaceEvent& stEventInfo );

	/* Triggers when this element loses focus.*/
	virtual void OnBlur( const FKUIInterfaceEvent& stEventInfo );

	/* Triggers when the interface focus changes.*/
	virtual void OnFocusChange( const FKUIInterfaceContainerElementEvent& stEventInfo );

	/* Triggers when a child element changes size. */
	virtual void OnChildSizeChange( const FKUIInterfaceContainerElementEvent& stEventInfo );

	/* Triggers when a child element changes location. */
	virtual void OnChildLocationChange( const FKUIInterfaceContainerElementEvent& stEventInfo );

	/* Called when the layout is invalidated. */
	virtual void OnLayoutInvalidated( const FKUIInterfaceEvent& stEventInfo );

	/* Called when the layout is complete. */
	virtual void OnLayoutComplete( const FKUIInterfaceEvent& stEventInfo );

	/* Calls after the child managers are updated. */
	virtual void OnChildManagersUpdated( const FKUIInterfaceEvent& stEventInfo );

	/* Called when a child is added. */
	virtual void OnChildAdded( const FKUIInterfaceContainerElementEvent& stEventInfo );

	/* Called when a child is removed. */
	virtual void OnChildRemoved( const FKUIInterfaceContainerElementEvent& stEventInfo );

	/* Triggers when the game stage changes. */
	virtual void OnMatchStateChange( const FKUIInterfaceContainerMatchStateEvent& stEventInfo );

	/* Called when the layout is invalidated. */
	UFUNCTION(Category="KeshUI | Container", BlueprintImplementableEvent)
	virtual void OnLayoutInvalidatedBP( const FKUIInterfaceEvent& stEventInfo );

	/* Called when the layout is complete. */
	UFUNCTION(Category="KeshUI | Container", BlueprintImplementableEvent)
	virtual void OnLayoutCompleteBP( const FKUIInterfaceEvent& stEventInfo );

	/* Calls after the child managers are updated. */
	UFUNCTION( Category = "KeshUI | Container", BlueprintImplementableEvent )
	virtual void OnChildManagersUpdatedBP( const FKUIInterfaceEvent& stEventInfo );

	/* Called when a child is added. */
	UFUNCTION(Category="KeshUI | Container", BlueprintImplementableEvent)
	virtual void OnChildAddedBP( const FKUIInterfaceContainerElementEvent& stEventInfo );

	/* Called when a child is removed. */
	UFUNCTION(Category="KeshUI | Container", BlueprintImplementableEvent)
	virtual void OnChildRemovedBP( const FKUIInterfaceContainerElementEvent& stEventInfo );

	/* Triggers when the screen resolution changes. */
	UFUNCTION(Category="KeshUI | Container", BlueprintImplementableEvent)
	virtual void OnScreenResolutionChangeBP( const FKUIInterfaceContainerScreenResolutionEvent& stEventInfo );

	/* Triggers when the match starts. */
	UFUNCTION(Category="KeshUI | Container", BlueprintImplementableEvent)
	virtual void OnMatchStartBP( const FKUIInterfaceEvent& stEventInfo );

	/* Triggers when the match starts. */
	UFUNCTION( Category = "KeshUI | Container", BlueprintImplementableEvent )
	virtual void OnMatchEndBP( const FKUIInterfaceEvent& stEventInfo );

	/* Triggers when the match is paused. */
	UFUNCTION(Category="KeshUI | Container", BlueprintImplementableEvent)
	virtual void OnMatchPausedBP( const FKUIInterfaceContainerPlayerEvent& stEventInfo );

	/* Triggers when the match is unpaused. */
	UFUNCTION(Category="KeshUI | Container", BlueprintImplementableEvent)
	virtual void OnMatchUnpausedBP( const FKUIInterfaceContainerPlayerEvent& stEventInfo );

	/* Triggers when a player dies. */
	UFUNCTION(Category="KeshUI | Container", BlueprintImplementableEvent)
	virtual void OnPlayerDeathBP( const FKUIInterfaceContainerPlayerEvent& stEventInfo );

	/* Triggers when the visibility of this element changes. */
	UFUNCTION(Category="KeshUI | Container", BlueprintImplementableEvent)
	virtual void OnVisibilityChangeBP( const FKUIInterfaceContainerVisibilityEvent& stEventInfo );

	/* Triggers when this element is focused. */
	UFUNCTION(Category="KeshUI | Container", BlueprintImplementableEvent)
	virtual void OnFocusBP( const FKUIInterfaceEvent& stEventInfo );
	
	/* Triggers when this element loses focus.*/
	UFUNCTION(Category="KeshUI | Container", BlueprintImplementableEvent)
	virtual void OnBlurBP( const FKUIInterfaceEvent& stEventInfo );

	/* Triggers when the interface focus changes.*/
	UFUNCTION( Category = "KeshUI | Container", BlueprintImplementableEvent )
	virtual void OnFocusChangeBP( const FKUIInterfaceContainerElementEvent& stEventInfo );

	/* Triggers when a child element changes size. */
	UFUNCTION(Category="KeshUI | Container", BlueprintImplementableEvent)
	virtual void OnChildSizeChangeBP( const FKUIInterfaceContainerElementEvent& stEventInfo );

	/* Triggers when a child element changes location. */
	UFUNCTION(Category="KeshUI | Container", BlueprintImplementableEvent)
	virtual void OnChildLocationChangeBP( const FKUIInterfaceContainerElementEvent& stEventInfo );

	/* Triggers when the game stage changes. */
	UFUNCTION( Category = "KeshUI | Container", BlueprintImplementableEvent )
	virtual void OnMatchStateChangeBP( const FKUIInterfaceContainerMatchStateEvent& stEventInfo );

};
