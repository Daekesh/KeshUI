// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

// Generic project-wide macros

//#define KUI_LOG_DEBUG
#define KUI_LOG_NIX "/Source/KeshUI/"
#define KUI_LOG_WIN "\\Source\\KeshUI\\"
#define KUI_LOG_FILE ( strstr( __FILE__, KUI_LOG_NIX ) ? ( strstr( __FILE__, KUI_LOG_NIX ) + strlen( KUI_LOG_NIX ) - 1 ) : ( strstr( __FILE__, KUI_LOG_WIN ) ? ( strstr( __FILE__, KUI_LOG_WIN ) + strlen( KUI_LOG_WIN ) - 1 ) : __FILE__ ) )

// Log
#define KUILogActual( w, t, s, ... ) UE_LOG( LogKeshUI, t, L"%s: %s:%d[%s]: " s, *( FString( HasServerCW( w ) ? "S" : "" ) + FString( HasClientCW( w ) ? "C" : "" ) ), *FString( KUI_LOG_FILE ), __LINE__, *FString( __FUNCTION__ ), __VA_ARGS__ )
#define KUILogCW( w, s, ... ) KUILogActual( w, Log, TEXT( s ), __VA_ARGS__ )
#define KUIWarnCW( w, s, ... ) KUILogActual( w, Warning, TEXT( s ), __VA_ARGS__ )
#define KUIErrorCW( w, s, ... ) KUILogActual( w, Error, TEXT( s ), __VA_ARGS__ )
#define KUIFatalCW( w, s, ... ) KUILogActual( w, Fatal, TEXT( s ), __VA_ARGS__ )
#define KUILogUOCW( w, s, ... ) KUILogActual( w, Log, L"%s: " L##s, *GetName(), __VA_ARGS__ )
#define KUIWarnUOCW( w, s, ... ) KUILogActual( w, Warning, L"%s: " L##s, *GetName(), __VA_ARGS__ )
#define KUIErrorUOCW( w, s, ... ) KUILogActual( w, Error, L"%s: " L##s, *GetName(), __VA_ARGS__ )
#define KUIFatalUOCW( w, s, ... ) KUILogActual( w, Fatal, L"%s: " L##s, *GetName(), __VA_ARGS__ )
#define KUILog( s, ... ) KUILogCW( GetWorld(), s, __VA_ARGS__ )
#define KUIWarn( s, ... ) KUIWarnCW( GetWorld(), s, __VA_ARGS__ )
#define KUIError( s, ... ) KUIErrorCW( GetWorld(), s, __VA_ARGS__ )
#define KUIFatal( s, ... ) KUIFatalCW( GetWorld(), s, __VA_ARGS__ )
#define KUILogUO( s, ... ) KUILogUOCW( GetWorld(), s, __VA_ARGS__ )
#define KUIWarnUO( s, ... ) KUIWarnUOCW( GetWorld(), s, __VA_ARGS__ )
#define KUIErrorUO( s, ... ) KUIErrorUOCW( GetWorld(), s, __VA_ARGS__ )
#define KUIFatalUO( s, ... ) KUIFatalUOCW( GetWorld(), s, __VA_ARGS__ )

#define ExpandV3( v ) (v).X, (v).Y, (v).Z
#define ExpandV2( v2 ) (v2).X, (v2).Y
#define ExpandR3( r ) (r).Yaw, (r).Pitch, (r).Roll
#define ExpandR2( r2 ) (r2).Yaw, (r2).Pitch

#ifdef KUI_LOG_DEBUG

#define KUILogDebug( s, ... ) KUILog( s, __VA_ARGS__ )
#define KUIWarnDebug( s, ... ) KUIWarn( s, __VA_ARGS__ )
#define KUIErrorDebug( s, ... ) KUIError( s, __VA_ARGS__ )
#define KUIFatalDebug( s, ... ) KUIFatal( s, __VA_ARGS__ )
#define KUILogDebugUO( s, ... ) KUILogUO( s, __VA_ARGS__ )
#define KUIWarnDebugUO( s, ... ) KUIWarnUO( s, __VA_ARGS__ )
#define KUIErrorDebugUO( s, ... ) KUIErrorUO( s, __VA_ARGS__ )
#define KUIFatalDebugUO( s, ... ) KUIFatalUO( s, __VA_ARGS__ )

#else // ifdef KUI_LOG_DEBUG

#define KUILogDebug( s, ... )
#define KUIWarnDebug( s, ... )
#define KUIErrorDebug( s, ... )
#define KUIFatalDebug( s, ... )
#define KUILogDebugUO( s, ... )
#define KUIWarnDebugUO( s, ... )
#define KUIErrorDebugUO( s, ... )
#define KUIFatalDebugUO( s, ... )

#endif // ifdef KUI_LOG_DEBUG

//#define KUI_UO_INIT_DEBUG()
#define KUI_UO_INIT_DEBUG() KUILogDebug( "Init class: %s", *GetClass()->GetName() );

// New objects
#define KUICreateDefaultSubobjectAssign( V, T, N ) V = oObjectInitializer.CreateDefaultSubobject<T>( this, N, false )
#define KUICreateDefaultSubobject( V, T, N ) T* KUICreateDefaultSubobjectAssign( V, T, N )
#define KUINewNamedObjectAssign( V, T, N ) V = NewNamedObject<T>( this, N, this->GetFlags(), NULL )
#define KUINewNamedObject( V, T, N ) T* KUINewNamedObjectAssign( V, T, N )
#define KUINewObjectAssign( V, T ) V = NewObject<T>( this )
#define KUINewObject( V, T ) T* KUINewObjectAssign( V, T )

// Construction helpers
#define KUIFindObjText( N, T, P ) \
	ConstructorHelpers::FObjectFinder<T> N( P ); \
	if ( N.Object == NULL ) \
	{ \
		KUIWarnUO( "Null resource %s* %s %s", *( T::StaticClass()->GetName() ), *FString( #N ), P ); \
	}
#define KUIFindObj( N, T, P ) KUIFindObjText( N, T, TEXT( P ) )

// Local player
#define KUILocalPlayerCW( w ) ( ( GEngine && w && w->GetNetMode() != ENetMode::NM_DedicatedServer ) ? GEngine->GetFirstLocalPlayerController( w ) : NULL )
#define KUILocalPlayer() KUILocalPlayerCW( GetWorld() )

// Network (custom world)
#define HasServerCW( w ) ( w == NULL ? true : w->GetNetMode() != ENetMode::NM_Client )
#define HasClientCW( w ) ( w == NULL ? true : w->GetNetMode() != ENetMode::NM_DedicatedServer )
#define HasNetworkCW( w ) ( w == NULL ? true : w->GetNetMode() != ENetMode::NM_Standalone )
#define IsClientOnlyCW( w ) ( w == NULL ? false : w->GetNetMode() == ENetMode::NM_Client )
#define IsServerOnlyCW( w ) ( w == NULL ? false : w->GetNetMode() == ENetMode::NM_DedicatedServer )
#define IsNetworkClientCW( w ) ( w == NULL ? true : ( w->GetNetMode() != ENetMode::NM_DedicatedServer && w->GetNetMode() != ENetMode::NM_Standalone ) )
#define IsNetworkServerCW( w ) ( w == NULL ? true : ( w->GetNetMode() != ENetMode::NM_Client && w->GetNetMode() != ENetMode::NM_Standalone ) )

// Network
#define HasServer() HasServerCW( GetWorld() )
#define HasClient() HasClientCW( GetWorld() )
#define HasNetwork() HasNetworkCW( GetWorld() )
#define IsClientOnly() IsClientOnlyCW( GetWorld() )
#define IsServerOnly() IsServerOnlyCW( GetWorld() )
#define IsNetworkClient() IsNetworkClientCW( GetWorld() )
#define IsNetworkServer() IsNetworkServerCW( GetWorld() )

// World
#define IsMapPreviewCW( w ) ( w == NULL ? false : ( w->WorldType != EWorldType::Game && w->WorldType != EWorldType::PIE ) )
#define IsMapPreview() IsMapPreviewCW( GetWorld() )

// Math stuff
#define round( x ) FMath::RoundToInt( x )
#define floor( x ) FMath::FloorToInt( x )
#define max( a, b ) ( b > a ? b : a )
#define min( a, b ) ( a > b ? b : a )
#define abs( a ) ( a < 0 ? -a : a )
#define clamp( x, mi, ma ) FMath::Clamp( x, mi, ma )
#define sqrt( x ) FMath::Pow( x, 0.5f )
#define sqrd( x ) ( ( x ) * ( x ) )
#define pow( x, y ) FMath::Pow( x, y )
#define sin( x ) FMath::Sin( x )
#define cos( x ) FMath::Cos( x )
#define tan( x ) FMath::Tan( x )
#define asin( x ) FMath::Asin( x )
#define acos( x ) FMath::Acos( x )
#define atan( x ) FMath::Atan( x )
#define atan2( x, y ) FMath::Atan2( x, y )
#define mod( x, y ) FMath::Fmod( x, y )

// Delegates
#define KUI_DECLARE_DELEGATE_OneParam( n, p1 ) \
	DECLARE_DELEGATE_OneParam( n ## Delegate, p1 ); \
	typedef void( __cdecl UObject::* ## n ## Prototype )( p1 param1 );

#define KUI_DECLARE_DELEGATE_TwoParams( n, p1, p2 ) \
	DECLARE_DELEGATE_TwoParams( n ## Delegate, p1, p2 ); \
	typedef void( __cdecl UObject::* ## n ## Prototype )( p1 param1, p2 param2 );

#define KUI_DECLARE_DELEGATE_ThreeParams( n, p1, p2, p3 ) \
	DECLARE_DELEGATE_ThreeParams( n ## Delegate, p1, p2, p3 ); \
	typedef void( __cdecl UObject::* ## n ## Prototype )( p1 param1, p2 param2, p3 param3 );

#define KUI_DECLARE_DELEGATE_FourParams( n, p1, p2, p3, p4 ) \
	DECLARE_DELEGATE_FourParams( n ## Delegate, p1, p2, p3, p4 ); \
	typedef void( __cdecl UObject::* ## n ## Prototype )( p1 param1, p2 param2, p3 param3, p4 param4 );

#define KUI_DECLARE_DELEGATE_FiveParams( n, p1, p2, p3, p4, p5 ) \
	DECLARE_DELEGATE_FiveParams( n ## Delegate, p1, p2, p3, p4, p5 ); \
	typedef void( __cdecl UObject::* ## n ## Prototype )( p1 param1, p2 param2, p3 param3, p4 param4, p5 param5 );

#define KUI_DECLARE_DELEGATE_SixParams( n, p1, p2, p3, p4, p5, p6 ) \
	DECLARE_DELEGATE_SixParams( n ## Delegate, p1, p2, p3, p4, p5, p6 ); \
	typedef void( __cdecl UObject::* ## n ## Prototype )( p1 param1, p2 param2, p3 param3, p4 param4, p5 param5, p6 param6 );

#define KUI_DECLARE_DELEGATE_SevenParams( n, p1, p2, p3, p4, p5, p6, p7 ) \
	DECLARE_DELEGATE_SevenParams( n ## Delegate, p1, p2, p3, p4, p5, p6, p7 ); \
	typedef void( __cdecl UObject::* ## n ## Prototype )( p1 param1, p2 param2, p3 param3, p4 param4, p5 param5, p6 param6, p7 param7 );

#define KUI_DECLARE_DELEGATE_EightParams( n, p1, p2, p3, p4, p5, p6, p7, p8 ) \
	DECLARE_DELEGATE_EightParams( n ## Delegate, p1, p2, p3, p4, p5, p6, p7, p8 ); \
	typedef void( __cdecl UObject::* ## n ## Prototype )( p1 param1, p2 param2, p3 param3, p4 param4, p5 param5, p6 param6, p7 param7, p8 param8 );

// Constructors
#define KUI_CLASS_HEADER_ACCESS( c, a ) //\
//	a : \
//	c ( const class FObjectInitializer& oObjectInitializer );
#define KUI_CLASS_HEADER( c ) KUI_CLASS_HEADER_ACCESS( c, public )
