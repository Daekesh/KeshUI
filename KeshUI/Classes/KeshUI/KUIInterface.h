 // Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KeshUI/KUIMacros.h"
#include "KeshUI/KUIInterfaceElement.h"
#include "KeshUI/KUIInterfaceContainer.h"
#include "KUIInterface.generated.h"

#define KUIBroadcastEventObj( o, t, ... ) \
	t stEventInfo( __VA_ARGS__ ); \
	o->BroadcastEvent( stEventInfo );

#define KUIBroadcastEvent( t, ... ) KUIBroadcastEventObj( this, t, __VA_ARGS__ )

#define KUIBroadcastSubEventObj( o, t, ... ) \
	t stSubEventInfo( __VA_ARGS__ ); \
	o->BroadcastEvent( stSubEventInfo );

#define KUIBroadcastSubEvent( t, ... ) KUIBroadcastSubEventObj( this, t, __VA_ARGS__ )

class UKUIInterfaceContainer;
class UKUIRootContainer;
class UKUITooltipContainer;
class UKUICursorContainer;
class UKUIBoxInterfaceComponent;
class IKUICancellable;
class UKUIAssetLibrary;

#define KUI_INTERFACE_FIRST_CURSOR_UPDATE -1.f
#define KUI_INTERFACE_MOUSEOVER_DEBUG 0 // change to 0 to 1

UENUM(BlueprintType)
namespace EKUIInterfaceRoot
{
	enum Type
	{
		R_Root    UMETA(DisplayName="Root Container"),
		R_Overlay UMETA(DisplayName="Overlay Container"),
		R_Tooltip UMETA(DisplayName="Tooltip Container"),
		R_Cursor  UMETA(DisplayName="Cursor Container"),
		R_Max     UMETA(Hidden)
	};
}


/**
 * Root interface class.  Contains all interface components.
 */
UCLASS(ClassGroup="KeshUI", Blueprintable, BlueprintType)
class KESHUI_API AKUIInterface : public AHUD
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( AKUIInterface  )

	AKUIInterface ( const class FObjectInitializer& oObjectInitializer );

public:

	/* Returns the latest created instance of this class. */
	UFUNCTION( Category = "KeshUI | Interface", BlueprintCallable )
	static AKUIInterface* GetLatestInstance();

	/* Returns the asset identified by the given FName from the asset libraries. */
	virtual UObject** GetAsset( const FName& nName ) const;

	template<class T>
	FORCEINLINE T** GetAsset( const FName& nName ) const
	{
		return reinterpret_cast<T**>( GetAsset( nName ) );
	}

	/* Returns the player controller that owns this interface. */
	UFUNCTION( Category = "KeshUI | Interface", BlueprintCallable )
	virtual APlayerController* GetPlayerController() const;

	/* Returns one of the root containers. */
	UFUNCTION( Category = "KeshUI | Interface", BlueprintCallable )
	virtual UKUIInterfaceContainer* GetRootContainer( uint8 iContainerId ) const;

	/* Returns true if the UI is visible (except the cursor.) */
	UFUNCTION(Category = "KeshUI | Interface", BlueprintCallable)
	virtual bool IsVisible() const;

	/* Sets whether the UI is visible (except the cursor.) */
	UFUNCTION(Category = "KeshUI | Interface", BlueprintCallable)
	virtual void SetVisible( bool bVisible );

	/* Returns true if the cursor is visible. */
	UFUNCTION(Category = "KeshUI | Interface", BlueprintCallable)
	virtual bool IsCursorVisible() const;

	/* Sets whether the cursor is visible. */
	UFUNCTION(Category = "KeshUI | Interface", BlueprintCallable)
	virtual void SetCursorVisible( bool bVisible );

	/* Gets the screen resolution. */
	UFUNCTION(Category = "KeshUI | Interface", BlueprintCallable)
	virtual FVector2D GetScreenResolution() const;

	/* Gets the cursor location. */
	UFUNCTION(Category = "KeshUI | Interface", BlueprintCallable)
	virtual FVector2D GetCursorLocation() const;

	/* Gets the cursor vector in the last tick. */
	UFUNCTION(Category = "KeshUI | Interface", BlueprintCallable)
	virtual FVector2D GetCursorVector() const;

	/* Gets the location where the given mouse button was pressed. */
	virtual FVector2D GetMouseButtonDownLocation( EMouseButtons::Type eButton ) const;

	/* Gets the location where the given mouse button was pressed. */
	UFUNCTION(Category = "KeshUI | Interface", BlueprintCallable)
	virtual FVector2D GetMouseButtonDownLocationBP( uint8 eButton ) const;

	/* Gets whether a certain key is pressed. */
	UFUNCTION(Category = "KeshUI | Interface", BlueprintCallable)
	virtual bool IsInputDown( FKey eKey ) const;

	/* Gets the time when the given key was pressed (game time, not real.) */
	UFUNCTION(Category = "KeshUI | Interface", BlueprintCallable)
	virtual float GetInputDownTime( FKey eKey ) const;

	/* Returns the currently focused element. */
	UFUNCTION( Category = "KeshUI | Interface", BlueprintCallable )
	virtual UKUIInterfaceElement* GetFocus() const;

	/* Sets the currently focused element. */
	UFUNCTION(Category = "KeshUI | Interface", BlueprintCallable)
	virtual void SetFocus( UKUIInterfaceContainer* ctNewFocus );

	/* Ticks the UI. */
	virtual void Tick( float fDeltaTime ) override;

	virtual void DrawHUD() override;

	/* Renders the UI. */
	virtual void Render( UCanvas* oCanvas );

	/* Adds an interface element to one of the root containers. */
	UFUNCTION(Category = "KeshUI | Interface", BlueprintCallable)
	virtual void AddElement( uint8 iContainer, UKUIInterfaceElement* oElement );

	/* Adds an object to the cancel stack (pressing escape.) */
	virtual void AddCancellable( UObject* oCancellable );

	/* Removes an object from the cancel stack (pressing escape.) */
	virtual void RemoveCancellable( UObject* oCancellable );

	/* Called when something is cancelled. */
	virtual void OnCancel( UObject* oCancellable );

	/* Cancels the top item on the cancel stack. */
	UFUNCTION(Category = "KeshUI | Interface", BlueprintCallable)
	virtual bool Cancel();

	/* Returns the appropriate mouse button for the key, or invalid. */
	static EMouseButtons::Type GetMouseButtonFromKey( FKey eKey );

	/* Triggers when the mouse is moved. */
	virtual void OnMouseMove( const FVector2D& v2OldLocation, const FVector2D& v2NewLocation );

	/* Triggers when a mouse button is pressed. */
	virtual bool OnMouseButtonDown( EMouseButtons::Type eButton, const FVector2D& v2Location );

	/* Triggers when a mouse button is released. */
	virtual bool OnMouseButtonUp( EMouseButtons::Type eButton, const FVector2D& v2Location );

	/* Triggers when a key is pressed. */
	virtual bool OnKeyDown( FKey eKey );

	/* Triggers when a key is released. */
	virtual bool OnKeyUp( FKey eKey );

	/* Triggers when a key press is repeated. */
	virtual bool OnKeyRepeat( FKey eKey );

	/* Triggers when a char is input. */
	virtual bool OnKeyChar( TCHAR chChar );

	/* Triggers when the match is paused. */
	virtual void OnMatchPaused( uint32 iPlayerId );

	/* Triggers when the match is unpaused. */
	virtual void OnMatchUnpaused( uint32 iPlayerId );

	/* Triggers when a player dies. */
	virtual void OnPlayerDeath( uint32 iPlayerId );

	/* Triggers when the game stage changes. */
	virtual void OnMatchStateChange( FName nMatchState );

	/* Returns true if the currently focused element consumes key events. */
	UFUNCTION( Category = "KeshUI | Interface", BlueprintCallable )
	virtual bool IsConsumingInputEvents() const;

	virtual void PostInitializeComponents() override;

	/* Broadcasts events to components. */
	virtual void BroadcastEvent( FKUIInterfaceEvent& stEventInfo, bool bTopDown = false, bool bIncludeCursor = true );

#if KUI_INTERFACE_MOUSEOVER_DEBUG
	TArray<bool> arDebugMouseOver;
	bool bDebugMouseOver;
	TWeakObjectPtr<UKUIInterfaceElement> cmDebugMouseOver;
	FVector2D v2DebugMouseOverLocation;
	FVector2D v2DebugMouseOverSize;
#endif // KUI_INTERFACE_MOUSEOVER_DEBUG

protected:

	bool bVisible;
	bool bCursorVisible;
	FVector2D v2ScreenResolution;
	FVector2D v2CursorLocation;
	FVector2D v2CursorVector;
	TArray<FVector2D> arMouseButtonDownLocations;
	TArray<TWeakObjectPtr<UObject>> arCancellables;
	TWeakObjectPtr<UKUIInterfaceContainer> ctFocused;
	
	UPROPERTY()
	TArray<UKUIRootContainer*> ctRootContainers;

#if KUI_INTERFACE_MOUSEOVER_DEBUG
	UPROPERTY()
	UKUIBoxInterfaceComponent* cmDebugMouseOverTestBox;

	TWeakObjectPtr<UKUIInterfaceElement> cmDebugMouseOverLastTick;
#endif // KUI_INTERFACE_MOUSEOVER_DEBUG

	/* Triggers when the screen resolution changes. */
	virtual void OnScreenResolutionChange( const FVector2D& v2OldRes, const FVector2D& v2NewRes );

	/* Triggers when the match starts. */
	virtual void OnMatchStart();

	/* Triggers when the match has ended. */
	virtual void OnMatchEnd();

	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual void OnVisibilityChangeBP();

	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual void OnInputKeyBP( FKey eKey, EInputEvent eEventType, float fAmountDepressed, bool bGamepad );

	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual void OnScreenResolutionChangeBP( const FVector2D& v2OldRes, const FVector2D& v2NewRes );

	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual void OnFocusChangeBP( UKUIInterfaceContainer* ctOldFocus, UKUIInterfaceContainer* ctNewFocus );

	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual void OnTickBP( float fDeltaTime );

	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual void OnRenderBP( UCanvas* oCanvas );

	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual void OnElementAddedBP( uint8 iContainer, UKUIInterfaceElement* oElement );

	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual void OnCancellableAddedBP( UObject* oCancellable );

	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual void OnCancellableRemovedBP( UObject* oCancellable );

	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual void OnCancelBP( UObject* oCancellable );

	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual void OnMouseMoveBP( const FVector2D& v2OldLocation, const FVector2D& v2NewLocation );

	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual bool OnMouseButtonDownBP( uint8 eButton, const FVector2D& v2Location );
	
	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual bool OnMouseButtonUpBP( uint8 eButton, const FVector2D& v2Location );

	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual void OnKeyDownBP( FKey eKey );

	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual void OnKeyUpBP( FKey eKey );

	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual void OnKeyRepeatBP( FKey eKey );

	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual void OnKeyCharBP( const FString& strChar );

	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual void OnMatchStartBP();

	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual void OnMatchPausedBP( int32 iPlayerId );

	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual void OnMatchUnpauseBP( int32 iPlayerId );

	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual void OnMatchEndBP();

	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual void OnPlayerDeathBP( int32 iPlayerId );

	UFUNCTION( Category = "KeshUI | Interface", BlueprintImplementableEvent )
	virtual void OnMatchStateChangeBP( FName nMatchState );

};
