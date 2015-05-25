// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KeshUI/KUIMacros.h"
#include "KUIInterfaceElement.generated.h"

#define KUISendEventObj( t, o, ... ) \
	t stEventInfo( __VA_ARGS__ ); \
	o->SendEvent( stEventInfo );

#define KUISendEvent( t, ... ) KUISendEventObj( t, this, __VA_ARGS__ )

#define KUISendSubEventObj( t, o, ... ) \
	t stSubEventInfo( __VA_ARGS__ ); \
	o->SendEvent( stSubEventInfo );

#define KUISendSubEvent( t, ... ) KUISendSubEventObj( t, this, __VA_ARGS__ )

class AKUIInterface;
class UKUIInterfaceContainer;
class UKUIRenderCache;

#define KUI_ZINDEX_MAX 65534
#define KUI_ZINDEX_NONE 65535

/* Toggle state values. */
UENUM( BlueprintType )
namespace EKUIBool
{
	enum SetValue
	{
		SV_False   UMETA( DisplayName = "Set To False" ),
		SV_True    UMETA( DisplayName = "Set To True" ),
		SV_Toggle  UMETA( DisplayName = "Toggle Value" ),
		SV_Max     UMETA( Hidden )
	};
}

/* Used to match bools. */
UENUM( BlueprintType )
namespace EKUIBoolMatch
{
	enum Value
	{
		V_True  UMETA( DisplayName = "Match True" ),
		V_False UMETA( DisplayName = "Match False" ),
		V_Any   UMETA( DisplayName = "Match Any" ),
		V_Max   UMETA( Hidden )
	};
}

UENUM( BlueprintType )
namespace EKUIInterfaceHAlign
{
	enum Type
	{
		HA_None        UMETA(DisplayName="Not Aligned"),
		HA_Left        UMETA(DisplayName="Left Edges Aligned"),
		HA_Centre      UMETA(DisplayName="Horizon Centres Aligned"),
		HA_Right       UMETA(DisplayName="Right Edges Aligned"),
		HA_Fill        UMETA(DisplayName="Changed to the Same Width"),
		HA_Left_Outer  UMETA(DisplayName="Right Edge Aligned to Left Edge (Outside)"),
		HA_Right_Outer UMETA(DisplayName="Left Edge Aligned to Right Edge (Outside)"),
		HA_Max         UMETA(Hidden)
	};
}

UENUM( BlueprintType )
namespace EKUIInterfaceVAlign
{
	enum Type
	{
		VA_None         UMETA(DisplayName="Not Aligned"),
		VA_Top          UMETA(DisplayName="Top Edges Aligned"),
		VA_Centre       UMETA(DisplayName="Vertical Centres Aligned"),
		VA_Bottom       UMETA(DisplayName="Bottom Edges Aligned"),
		VA_Fill         UMETA(DisplayName="Changed to the Same Height"),
		VA_Top_Outer    UMETA(DisplayName="Bottom Edge Aligned to Top Edge (Outside)"),
		VA_Bottom_Outer UMETA(DisplayName="Top Edge Aligned to Bottom Edge (Outside)"),
		VA_Max          UMETA(Hidden)
	};
}

UENUM( BlueprintType )
namespace EKUIInterfaceElementEventList
{
	enum Event
	{
		E_AddedToContainer = 0         UMETA( DisplayName = "Added To Container" ),
		E_RemovedFromContainer = 1     UMETA( DisplayName = "Removed From Container" ),
		E_Render = 2                   UMETA( DisplayName = "Render" ),
		E_AlignLocationInvalidated = 3 UMETA( DisplayName = "Align Location Invalidated" ),
		E_AlignLocationCalculated = 4  UMETA( DisplayName = "Align Location Calculated" ),
		E_LocationChange = 5           UMETA( DisplayName = "Location Change" ),
		E_SizeChange = 6               UMETA( DisplayName = "Size Change" ),
	};
}

#define KUI_BASE_EVENT_FIRST EKUIInterfaceElementEventList::E_AddedToContainer
#define KUI_BASE_EVENT_LAST EKUIInterfaceElementEventList::E_SizeChange

USTRUCT( BlueprintType )
struct FKUIInterfaceEvent
{
	GENERATED_USTRUCT_BODY();

	FKUIInterfaceEvent() : FKUIInterfaceEvent( 0 ) { }

	FKUIInterfaceEvent( uint8 iEventID ) 
	{ 
		this->iEventID = iEventID;
	}

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "Event ID" ) )
	uint8 iEventID;
};

USTRUCT( BlueprintType )
struct FKUIInterfaceElementContainerEvent : public FKUIInterfaceEvent
{
	GENERATED_USTRUCT_BODY();

	FKUIInterfaceElementContainerEvent() : FKUIInterfaceEvent() { }

	FKUIInterfaceElementContainerEvent( uint8 iEventID, UKUIInterfaceContainer* ctContainer ) 
		: FKUIInterfaceEvent( iEventID ) 
	{ 
		this->ctContainer = ctContainer; 
	}

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "Container" ) )
	UKUIInterfaceContainer* ctContainer;
};

USTRUCT( BlueprintType )
struct FKUIInterfaceElementRenderEvent : public FKUIInterfaceEvent
{
	GENERATED_USTRUCT_BODY();

	FKUIInterfaceElementRenderEvent() : FKUIInterfaceEvent() { }

	FKUIInterfaceElementRenderEvent( uint8 iEventID, UCanvas* oCanvas, const FVector2D& v2Origin ) 
		: FKUIInterfaceEvent( iEventID ) 
	{ 
		this->oCanvas = oCanvas;
		this->v2Origin = v2Origin; 
	}

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "Canvas" ) )
	UCanvas* oCanvas;

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "Location" ) )
	FVector2D v2Origin;
};

USTRUCT( BlueprintType )
struct FKUIInterfaceContainerLocationChangeEvent : public FKUIInterfaceEvent
{
	GENERATED_USTRUCT_BODY();

	FKUIInterfaceContainerLocationChangeEvent() : FKUIInterfaceEvent() { }

	FKUIInterfaceContainerLocationChangeEvent( uint8 iEventID, const FVector2D& v2OldLocation, const FVector2D& v2NewLocation ) 
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
struct FKUIInterfaceContainerSizeChangeEvent : public FKUIInterfaceEvent
{
	GENERATED_USTRUCT_BODY();

	FKUIInterfaceContainerSizeChangeEvent() : FKUIInterfaceEvent() { }

	FKUIInterfaceContainerSizeChangeEvent( uint8 iEventID, const FVector2D& v2OldSize, const FVector2D& v2NewSize )
		: FKUIInterfaceEvent( iEventID )
	{
		this->v2OldSize = v2OldSize;
		this->v2NewSize = v2NewSize;
	}

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "Old Size" ) )
	FVector2D v2OldSize;

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "New Size" ) )
	FVector2D v2NewSize;
};

/**
 * The base KeshUI class for all ui elements.
 */
UCLASS(Abstract, Category="KeshUI | UI", BlueprintType)
class KESHUI_API UKUIInterfaceElement : public UObject
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUIInterfaceElement )

	UKUIInterfaceElement( const class FObjectInitializer& oObjectInitializer );

public:

	/* Gets the base interface this container is in. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual AKUIInterface* GetInterface() const;

	/* Gets the container this component is in. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual UKUIInterfaceContainer* GetContainer() const;

	/* Sets the container this component is in. Do not call manually. */
	virtual void SetContainer( UKUIInterfaceContainer* ctContainer );

	/* Returns the asset identified by the given FName from the asset libraries. */
	virtual FORCEINLINE UObject** GetAsset( const FName& nName ) const;

	template<class T>
	FORCEINLINE T** GetAsset( const FName& nName ) const
	{
		return reinterpret_cast<T**>( GetAsset( nName ) );
	}

	virtual UWorld* GetWorld() const override;

	/* Returns true if this element is visible. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual bool IsVisible() const;

	/* Sets whether this element is visible. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual void SetVisible( bool bVisible );

	/* Returns true if this and all its containers are visible. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual bool IsVisibleRecursive() const;

	/* Returns true if this element is disabled. */
	UFUNCTION( Category = "KeshUI | Element", BlueprintCallable )
	virtual bool IsDisabled() const;

	/* Returns true if this or any of its containers are disabled. */
	UFUNCTION( Category = "KeshUI | Element", BlueprintCallable )
	virtual bool IsDisabledRecursive() const;

	/* Gets the relative container offset. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual const FVector2D& GetLocation() const;

	/* Sets the relative container offset. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual void SetLocationStruct( const FVector2D& v2Location );

	/* Sets the relative container offset. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual void SetLocation( float fX, float fY );

	/* Gets the border around an element. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual const FVector4& GetMargin() const;

	/* Sets the border around an element. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual void SetMarginStruct( const FVector4& v2Margin );

	/* Sets the border around an element. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual void SetMargin( float fLeft, float fTop, float fRight, float fBottom );

	/* Gets the size of this element. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual const FVector2D& GetSize() const;

	/* Sets the size of this element. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual void SetSize( float fWidth, float fHeight );

	/* Returns the total size of this element, including the margin. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual const FVector2D GetMarginSize() const;

	/* Returns the location of this element, with the margin taken off. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual const FVector2D GetMarginOffset() const;

	/* Gets the element this is aligned to.  If this has a value, then the element location is the offset from the alignment. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual UKUIInterfaceElement* GetAlignedTo() const;

	/* Sets the element this is aligned to. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual void SetAlignedTo( UKUIInterfaceElement* oAlignedTo );

	/* Gets the horizontal alignment. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual TEnumAsByte<EKUIInterfaceHAlign::Type> GetHorizontalAlignment() const;

	/* Sets the horizontal alignment. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual void SetHorizontalAlignment( const TEnumAsByte<EKUIInterfaceHAlign::Type> eHAlign );

	/* Gets the vertical alignment. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual TEnumAsByte<EKUIInterfaceVAlign::Type> GetVerticalAlignment() const;

	/* Sets the vertical alignment. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual void SetVerticalAlignment( const TEnumAsByte<EKUIInterfaceVAlign::Type> eVAlign );

	/* Sets this element to fill its container. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual void FillContainer();

	/* Returns true if this has a valid aligned location. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual bool HasValidAlignLocation() const;

	/* Invalidates the align location. */
	virtual void InvalidateAlignLocation();

	/* Sets the align location and validates it. */
	virtual void SetAlignLocation( const FVector2D& v2AlignLocation );

	/* Gets the render location of this element (alignloc + loc).  Calculates the align location if necessary. */
	virtual const FVector2D GetRenderLocation() const;

	/* Gets the Z-Index order for this element. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual uint16 GetZIndex() const;

	/* Sets the Z-Index order for this element. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual void SetZIndex( uint16 iZIndex );

	/* Returns whether the mouse is over this element. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual bool IsCursorOver() const;

	/* Returns whether the given point is on this element. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual bool IsPointOver( const FVector2D& v2Point ) const;

	/* Returns the screen position of this element. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual const FVector2D GetScreenLocation() const;

	/* Returns the nested location of this element. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual const FVector2D GetNestedLocation( UKUIInterfaceContainer* ctRoot ) const;

	/* Called to render the element on the screen. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual void Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject = NULL );

	/* Returns the last location this element was rendered. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual const FVector2D& GetLastScreenRenderLocation() const;

	/* Returns true if the mouse is currently over the element. Uses cached screen render location. */
	UFUNCTION(Category="KeshUI | Element", BlueprintCallable)
	virtual bool IsMouseOver() const;

	/* Adds an object that should invalidate its location when this object moves. */
	virtual void AddAlignedToThis( UKUIInterfaceElement* oAlignChild );

	/* Removes an object that should invalidate its location when this object moves. */
	virtual void RemoveAlignedToThis( UKUIInterfaceElement* oAlignChild );

	/* Recursive alignment method. */
	virtual void CalculateAlignLocation( TArray<UKUIInterfaceElement*>& arAlignStack );

	/* Works out the align location given some metrics. */
	virtual const FVector2D CalculateAlignLocation( const FVector2D& v2Origin, const FVector2D& v2Extent, const FVector2D& v2Size, const FVector2D& v2Default = FVector2D::ZeroVector );

	/* Returns true if this consume key events when focused. */
	UFUNCTION(Category = "KeshUI | Element", BlueprintCallable)
	virtual bool IsInputEventConsumer() const;

	/* Turns on render caching. */
	UFUNCTION( Category = "KeshUI | Element", BlueprintCallable )
	virtual void EnableRenderCache();

	/* Turns off render caching. */
	UFUNCTION( Category = "KeshUI | Element", BlueprintCallable )
	virtual void DisableRenderCache();

	/* Returns true if render caching is enabled. */
	UFUNCTION( Category = "KeshUI | Element", BlueprintCallable )
	virtual bool IsRenderCaching() const;

	/* Returns true if we respond to this event. */
	virtual bool RespondsToEvent( uint8 iEventID ) const;

	/* Dispatches the event. */
	virtual void SendEvent( FKUIInterfaceEvent& stEventInfo );

	virtual void AddTag( const FString& strTag );

	virtual const FString& GetTag( int32 iIndex ) const;

	virtual bool HasTag( const FString& strTag ) const;

	bool bDebug;

protected:

	bool bVisible;
	FVector2D v2Location;
	FVector4 v4Margin;
	TWeakObjectPtr<UKUIInterfaceContainer> ctContainer;
	uint16 iZIndex;
	bool bValidAlignLocation;
	TWeakObjectPtr<UKUIInterfaceElement> oAlignedTo;
	EKUIInterfaceHAlign::Type eHAlign;
	EKUIInterfaceVAlign::Type eVAlign;
	FVector2D v2AlignLocation;
	FVector2D v2LastScreenRenderLocation;
	TArray<TWeakObjectPtr<UKUIInterfaceElement>> arAlignedToThis;
	TWeakObjectPtr<AKUIInterface> aLastRenderedBy;
	TArray<FString> arTags;

	UPROPERTY()
	UKUIRenderCache* oRenderCache;

	virtual void InvalidateRenderCache();
	virtual void InvalidateContainerRenderCache();

	/* Called when this elements location is changed. */
	virtual void OnLocationChange( const FKUIInterfaceContainerLocationChangeEvent& stEventInfo );

	/* Called when this elements size is changed. */
	virtual void OnSizeChange( const FKUIInterfaceContainerSizeChangeEvent& stEventInfo );

	/* Called when the align location is invalidated. */
	virtual void OnAlignLocationInvalidated( const FKUIInterfaceEvent& stEventInfo );

	/* Called after alignment has been calculated. */
	virtual void OnAlignmentLocationCalculated( const FKUIInterfaceEvent& stEventInfo );

	/* Triggered when this is added to a container. */
	virtual void OnAddedToContainer( const FKUIInterfaceElementContainerEvent& stEventInfo );

	/* Triggered when this is removed from a container. */
	virtual void OnRemovedFromContainer( const FKUIInterfaceElementContainerEvent& stEventInfo );

	/* Called before this element is rendered. */
	virtual void OnRender( const FKUIInterfaceElementRenderEvent& stEventInfo );

	/* Called when this elements location is changed. */
	UFUNCTION(Category="KeshUI | Element", BlueprintImplementableEvent)
	virtual void OnLocationChangeBP( const FKUIInterfaceContainerLocationChangeEvent& stEventInfo );

	/* Called when this elements size is changed. */
	UFUNCTION( Category = "KeshUI | Element", BlueprintImplementableEvent )
	virtual void OnSizeChangeBP( const FKUIInterfaceContainerSizeChangeEvent& stEventInfo );

	/* Called when the align location is invalidated. */
	UFUNCTION(Category="KeshUI | Element", BlueprintImplementableEvent)
	virtual void OnAlignLocationInvalidatedBP( const FKUIInterfaceEvent& stEventInfo );

	/* Called after alignment has been calculated. */
	UFUNCTION(Category="KeshUI | Element", BlueprintImplementableEvent)
	virtual void OnAlignmentLocationCalculatedBP( const FKUIInterfaceEvent& stEventInfo );

	/* Triggered when this is added to a container. */
	UFUNCTION(Category="KeshUI | Element", BlueprintImplementableEvent)
	virtual void OnAddedToContainerBP( const FKUIInterfaceElementContainerEvent& stEventInfo );

	/* Triggered when this is removed from a container. */
	UFUNCTION(Category="KeshUI | Element", BlueprintImplementableEvent)
	virtual void OnRemovedFromContainerBP( const FKUIInterfaceElementContainerEvent& stEventInfo );

	/* Called before this element is rendered. */
	UFUNCTION( Category = "KeshUI | Element", BlueprintImplementableEvent )
	virtual void OnRenderBP( const FKUIInterfaceElementRenderEvent& stEventInfo );

};
