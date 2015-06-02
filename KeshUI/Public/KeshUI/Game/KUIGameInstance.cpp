
#include "KeshUI.h"
#include "KeshUI/KUIAssetLibrary.h"
#include "KeshUI/Game/KUIGameInstance.h"


UKUIGameInstance::UKUIGameInstance( const class FObjectInitializer& oObjectInitializer )
	: Super( oObjectInitializer )
{
	KUI_UO_INIT_DEBUG()
		
	arAssetLibraries.SetNum( 0 );
}


UObject** UKUIGameInstance::GetAsset( const FName& nName ) const
{
	if ( IsTemplate() )
		return NULL;

	UObject** oAsset = NULL;

	for ( int32 i = 0; i < arAssetLibraries.Num(); ++i )
	{
		if ( arAssetLibraries[ i ] == NULL )
			continue;

		oAsset = arAssetLibraries[ i ]->GetAsset( nName );

		if ( oAsset != NULL )
			return oAsset;
	}

	KUIErrorUO( "Failed to load asset: %s", *( nName.ToString() ) );

	return NULL;
}
