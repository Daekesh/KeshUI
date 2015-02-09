// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#include "KeshUI.h"
#include "KUIAssetLibrary.h"


UKUIAssetLibrary::UKUIAssetLibrary( const class FObjectInitializer& oObjectInitializer )
	: Super( oObjectInitializer )
{
	KUI_UO_INIT_DEBUG()

	oParentLibrary = NULL;
	oChildLibrary = NULL;
	arAssetLibrary.SetNum( 0 );
}


UObject** UKUIAssetLibrary::GetAsset( const FName& nName ) const
{
	UObject** oAsset = NULL;

	if ( oParentLibrary != NULL )
	{
		oAsset = oParentLibrary->GetAsset( nName );

		if ( oAsset != NULL )
			return oAsset;
	}

	for ( int32 i = 0; i < arAssetLibrary.Num(); ++i )
	{
		if ( arAssetLibrary[ i ].nAssetName != nName )
			continue;

		oAsset = const_cast<UObject**>( &( arAssetLibrary[ i ].oAsset ) );

		if ( oAsset != NULL )
			return oAsset;
	}

	if ( oChildLibrary != NULL )
	{
		oAsset = oChildLibrary->GetAsset( nName );

		if ( oAsset != NULL )
			return oAsset;
	}
	
	return NULL;
}
