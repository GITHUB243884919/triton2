
//////////////////////////////////////////////////////////////////////////
// CShortcutList
//////////////////////////////////////////////////////////////////////////
#include "stdafx.hpp"
#include "proclient_svr.h"
#include "coremessage_pb.hxx.pb.h"
#include "promessage_pb.hxx.pb.h"
#include "sceneobj_manager.h"
#include "scenelogic.h"
#include "westserver.h"
#include "servercore.h"
#include "entity.h"
#include "template.h"
#include "property.h"
#include "entity.h"
#include "pro_define.h"
#include "propertymodule.h"
#include "scenecfg_manager.h"
#include "repetionmodule.h"
#include <llimits.h>
#ifdef LEAK_CHK_DEBUG_NEW
#include "debug_new.h"
#endif

CShortcut CShortcutList::GetShortcut( int vIndex )
{
	for ( int i = 0; i < mShortcutNumber; i ++ )
	{
		if ( mShortcut[ i ].mIndex == (unsigned int)vIndex )
			return mShortcut[ i ];
	}

	return CShortcut( );
}
void CShortcutList::SetShortcut( unsigned int vShortcut, unsigned int vType, unsigned int vIndex, unsigned int vLabelType, unsigned int vLabelIndex, int nParam1 )
{
	// 如果快捷栏已满
	if ( vIndex >= 48 )
		return;

	for ( int i = 0; i < mShortcutNumber; i ++ )
	{
		if ( mShortcut[ i ].mIndex == vIndex )
		{
			mShortcut[ i ].mShortcut	= vShortcut;
			mShortcut[ i ].mType		= vType;
			mShortcut[ i ].mLabelType	= vLabelType;
			mShortcut[ i ].mLabelIndex  = vLabelIndex;
			mShortcut[ i ].mParam1 = nParam1;
			return;
		}
	}

	if ( mShortcutNumber == 48 )
		return;

	mShortcut[ mShortcutNumber ++ ] = CShortcut( vShortcut, vType, vIndex, vLabelType, vLabelIndex, nParam1 );
}

void CShortcutList::CreateFromTlv2( PBShortcutList* pBuffer )
{
	mShortcutNumber = pBuffer->shortcutnumber();

	for ( int i = 0; i < mShortcutNumber; i ++ )
	{
		PBShortcut tpPBShortcut = pBuffer->shortcuts( i );

		//if ( tpPBShortcut == NULL )
		//{
		//	continue;
		//}

		mShortcut[ i ].mShortcut = tpPBShortcut.shortcut();
		mShortcut[ i ].mType = tpPBShortcut.mtype();
		mShortcut[ i ].mIndex = tpPBShortcut.mindex();
		mShortcut[ i ].mLabelType = tpPBShortcut.mlabeltype();
		mShortcut[ i ].mLabelIndex = tpPBShortcut.mlabelindex();
		mShortcut[ i ].mParam1 = tpPBShortcut.param1();
	}
	for ( int i = 0; i<MAX_HELPACTON_INT_NUM && i < pBuffer->helpstatus_size(); i++ )
	{
		mHelpActionStatus[i] = pBuffer->helpstatus( i );
	}
}


void CShortcutList::GetTlvBuffer2( PBShortcutList* pBuffer )
{
	pBuffer->set_shortcutnumber( mShortcutNumber );

	for ( int i = 0; i < mShortcutNumber; i ++ )
	{
		PBShortcut* tpPBShortcut = pBuffer->add_shortcuts();

		tpPBShortcut->set_shortcut( mShortcut[ i ].mShortcut );
		tpPBShortcut->set_mtype( mShortcut[ i ].mType );
		tpPBShortcut->set_mindex( mShortcut[ i ].mIndex );
		tpPBShortcut->set_mlabeltype( mShortcut[ i ].mLabelType );
		tpPBShortcut->set_mlabelindex( mShortcut[ i ].mLabelIndex );
		tpPBShortcut->set_param1( mShortcut[ i ].mParam1 );

	}
	for ( int i = 0 ; i<MAX_HELPACTON_INT_NUM; i++ )
	{
		pBuffer->add_helpstatus( mHelpActionStatus[i] );
	}
}

void CShortcutList::SetHelpStatus( int Status, int Index )
{
	if ( Index < 0 || Index >= MAX_HELPACTON_INT_NUM )
	{
		return;
	}
	mHelpActionStatus[ Index ] = Status;
}
int CShortcutList::GetHelpStatus( int Index )
{
	if ( Index < 0 || Index >= MAX_HELPACTON_INT_NUM )
	{
		return -1;
	}
	return mHelpActionStatus[ Index ] ;	
}

