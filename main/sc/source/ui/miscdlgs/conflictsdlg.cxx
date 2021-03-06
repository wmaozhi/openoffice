/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_sc.hxx"

//-----------------------------------------------------------------------------

#include <vcl/msgbox.hxx>

#include "conflictsdlg.hxx"
#include "conflictsdlg.hrc"
#include "scresid.hxx"
#include "viewdata.hxx"
#include "dbfunc.hxx"


//=============================================================================
// struct ScConflictsListEntry
//=============================================================================

bool ScConflictsListEntry::HasSharedAction( sal_uLong nSharedAction ) const
{
    ScChangeActionList::const_iterator aEnd = maSharedActions.end();
    for ( ScChangeActionList::const_iterator aItr = maSharedActions.begin(); aItr != aEnd; ++aItr )
    {
        if ( *aItr == nSharedAction )
        {
            return true;
        }
    }

    return false;
}

bool ScConflictsListEntry::HasOwnAction( sal_uLong nOwnAction ) const
{
    ScChangeActionList::const_iterator aEnd = maOwnActions.end();
    for ( ScChangeActionList::const_iterator aItr = maOwnActions.begin(); aItr != aEnd; ++aItr )
    {
        if ( *aItr == nOwnAction )
        {
            return true;
        }
    }

    return false;
}


//=============================================================================
// class ScConflictsListHelper
//=============================================================================

//UNUSED2008-05  bool ScConflictsListHelper::HasSharedAction( ScConflictsList& rConflictsList, sal_uLong nSharedAction )
//UNUSED2008-05  {
//UNUSED2008-05      ScConflictsList::const_iterator aEnd = rConflictsList.end();
//UNUSED2008-05      for ( ScConflictsList::const_iterator aItr = rConflictsList.begin(); aItr != aEnd; ++aItr )
//UNUSED2008-05      {
//UNUSED2008-05          if ( aItr->HasSharedAction( nSharedAction ) )
//UNUSED2008-05          {
//UNUSED2008-05              return true;
//UNUSED2008-05          }
//UNUSED2008-05      }
//UNUSED2008-05  
//UNUSED2008-05      return false;
//UNUSED2008-05  }

bool ScConflictsListHelper::HasOwnAction( ScConflictsList& rConflictsList, sal_uLong nOwnAction )
{
    ScConflictsList::const_iterator aEnd = rConflictsList.end();
    for ( ScConflictsList::const_iterator aItr = rConflictsList.begin(); aItr != aEnd; ++aItr )
    {
        if ( aItr->HasOwnAction( nOwnAction ) )
        {
            return true;
        }
    }

    return false;
}

ScConflictsListEntry* ScConflictsListHelper::GetSharedActionEntry( ScConflictsList& rConflictsList, sal_uLong nSharedAction )
{
    ScConflictsList::iterator aEnd = rConflictsList.end();
    for ( ScConflictsList::iterator aItr = rConflictsList.begin(); aItr != aEnd; ++aItr )
    {
        if ( aItr->HasSharedAction( nSharedAction ) )
        {
            return &(*aItr);
        }
    }

    return NULL;
}

ScConflictsListEntry* ScConflictsListHelper::GetOwnActionEntry( ScConflictsList& rConflictsList, sal_uLong nOwnAction )
{
    ScConflictsList::iterator aEnd = rConflictsList.end();
    for ( ScConflictsList::iterator aItr = rConflictsList.begin(); aItr != aEnd; ++aItr )
    {
        if ( aItr->HasOwnAction( nOwnAction ) )
        {
            return &(*aItr);
        }
    }

    return NULL;
}

void ScConflictsListHelper::Transform_Impl( ScChangeActionList& rActionList, ScChangeActionMergeMap* pMergeMap )
{
    if ( !pMergeMap )
    {
        return;
    }

    for ( ScChangeActionList::iterator aItr = rActionList.begin(); aItr != rActionList.end(); )
    {
        ScChangeActionMergeMap::iterator aItrMap = pMergeMap->find( *aItr );
        if ( aItrMap != pMergeMap->end() )
        {
            *aItr = aItrMap->second;
            aItr++;
        }
        else
        {
            aItr = rActionList.erase( aItr );
            DBG_ERROR( "ScConflictsListHelper::Transform_Impl: erased action from conflicts list!" );
        }
    }
}

void ScConflictsListHelper::TransformConflictsList( ScConflictsList& rConflictsList,
    ScChangeActionMergeMap* pSharedMap, ScChangeActionMergeMap* pOwnMap )
{
    ScConflictsList::iterator aEnd = rConflictsList.end();
    for ( ScConflictsList::iterator aItr = rConflictsList.begin(); aItr != aEnd; ++aItr )
    {
        if ( pSharedMap )
        {
            ScConflictsListHelper::Transform_Impl( aItr->maSharedActions, pSharedMap );
        }

        if ( pOwnMap )
        {
            ScConflictsListHelper::Transform_Impl( aItr->maOwnActions, pOwnMap );
        }
    }
}


//=============================================================================
// class ScConflictsFinder
//=============================================================================

ScConflictsFinder::ScConflictsFinder( ScChangeTrack* pTrack, sal_uLong nStartShared, sal_uLong nEndShared,
        sal_uLong nStartOwn, sal_uLong nEndOwn, ScConflictsList& rConflictsList )
    :mpTrack( pTrack )
    ,mnStartShared( nStartShared )
    ,mnEndShared( nEndShared )
    ,mnStartOwn( nStartOwn )
    ,mnEndOwn( nEndOwn )
    ,mrConflictsList( rConflictsList )
{
}
    
ScConflictsFinder::~ScConflictsFinder()
{
}

bool ScConflictsFinder::DoActionsIntersect( const ScChangeAction* pAction1, const ScChangeAction* pAction2 )
{
    if ( pAction1 && pAction2 && pAction1->GetBigRange().Intersects( pAction2->GetBigRange() ) )
    {
        return true;
    }
    return false;
}

ScConflictsListEntry* ScConflictsFinder::GetIntersectingEntry( const ScChangeAction* pAction ) const
{
    ScConflictsList::iterator aEnd = mrConflictsList.end();
    for ( ScConflictsList::iterator aItr = mrConflictsList.begin(); aItr != aEnd; ++aItr )
    {
        ScChangeActionList::const_iterator aEndShared = aItr->maSharedActions.end();
        for ( ScChangeActionList::const_iterator aItrShared = aItr->maSharedActions.begin(); aItrShared != aEndShared; ++aItrShared )
        {
            if ( DoActionsIntersect( mpTrack->GetAction( *aItrShared ), pAction ) )
            {
                return &(*aItr);
            }
        }

        ScChangeActionList::const_iterator aEndOwn = aItr->maOwnActions.end();
        for ( ScChangeActionList::const_iterator aItrOwn = aItr->maOwnActions.begin(); aItrOwn != aEndOwn; ++aItrOwn )
        {
            if ( DoActionsIntersect( mpTrack->GetAction( *aItrOwn ), pAction ) )
            {
                return &(*aItr);
            }
        }
    }

    return NULL;
}

ScConflictsListEntry* ScConflictsFinder::GetEntry( sal_uLong nSharedAction, const ScChangeActionList& rOwnActions )
{
    // try to get a list entry which already contains the shared action
    ScConflictsListEntry* pEntry = ScConflictsListHelper::GetSharedActionEntry( mrConflictsList, nSharedAction );
    if ( pEntry )
    {
        return pEntry;
    }

    // try to get a list entry for which the shared action intersects with any
    // other action of this entry
    pEntry = GetIntersectingEntry( mpTrack->GetAction( nSharedAction ) );
    if ( pEntry )
    {
        pEntry->maSharedActions.push_back( nSharedAction );
        return pEntry;
    }

    // try to get a list entry for which any of the own actions intersects with
    // any other action of this entry
    ScChangeActionList::const_iterator aEnd = rOwnActions.end();
    for ( ScChangeActionList::const_iterator aItr = rOwnActions.begin(); aItr != aEnd; ++aItr )
    {
        pEntry = GetIntersectingEntry( mpTrack->GetAction( *aItr ) );
        if ( pEntry )
        {
            pEntry->maSharedActions.push_back( nSharedAction );
            return pEntry;
        }
    }

    // if no entry was found, create a new one
    ScConflictsListEntry aEntry;
    aEntry.meConflictAction = SC_CONFLICT_ACTION_NONE;
    aEntry.maSharedActions.push_back( nSharedAction );
    mrConflictsList.push_back( aEntry );
    return &(mrConflictsList.back());
}

bool ScConflictsFinder::Find()
{
    if ( !mpTrack )
    {
        return false;
    }

    bool bReturn = false;
    ScChangeAction* pSharedAction = mpTrack->GetAction( mnStartShared );
    while ( pSharedAction && pSharedAction->GetActionNumber() <= mnEndShared )
    {
        ScChangeActionList aOwnActions;
        ScChangeAction* pOwnAction = mpTrack->GetAction( mnStartOwn );
        while ( pOwnAction && pOwnAction->GetActionNumber() <= mnEndOwn )
        {
            if ( DoActionsIntersect( pSharedAction, pOwnAction ) )
            {
                aOwnActions.push_back( pOwnAction->GetActionNumber() );
            }
            pOwnAction = pOwnAction->GetNext();
        }

        if ( aOwnActions.size() )
        {
            ScConflictsListEntry* pEntry = GetEntry( pSharedAction->GetActionNumber(), aOwnActions );;
            ScChangeActionList::iterator aEnd = aOwnActions.end();
            for ( ScChangeActionList::iterator aItr = aOwnActions.begin(); aItr != aEnd; ++aItr )
            {
                if ( pEntry && !ScConflictsListHelper::HasOwnAction( mrConflictsList, *aItr ) )
                {
                    pEntry->maOwnActions.push_back( *aItr );
                }
            }
            bReturn = true;
        }

        pSharedAction = pSharedAction->GetNext();
    }

    return bReturn;
}

//=============================================================================
// class ScConflictsResolver
//=============================================================================

ScConflictsResolver::ScConflictsResolver( ScChangeTrack* pTrack, ScConflictsList& rConflictsList )
    :mpTrack ( pTrack )
    ,mrConflictsList ( rConflictsList )
{
    DBG_ASSERT( mpTrack, "ScConflictsResolver CTOR: mpTrack is null!" );
}

ScConflictsResolver::~ScConflictsResolver()
{
}

void ScConflictsResolver::HandleAction( ScChangeAction* pAction, bool bIsSharedAction,
    bool bHandleContentAction, bool bHandleNonContentAction )
{
    if ( !mpTrack || !pAction )
    {
        return;
    }

    if ( bIsSharedAction )
    {
        ScConflictsListEntry* pConflictEntry = ScConflictsListHelper::GetSharedActionEntry(
            mrConflictsList, pAction->GetActionNumber() );
        if ( pConflictEntry )
        {
            ScConflictAction eConflictAction = pConflictEntry->meConflictAction;
            if ( eConflictAction == SC_CONFLICT_ACTION_KEEP_MINE )
            {
                if ( pAction->GetType() == SC_CAT_CONTENT )
                {
                    if ( bHandleContentAction )
                    {
                        mpTrack->Reject( pAction );
                    }
                }
                else
                {
                    if ( bHandleNonContentAction )
                    {
                        mpTrack->Reject( pAction );
                    }
                }
            }
            else if ( eConflictAction == SC_CONFLICT_ACTION_KEEP_OTHER )
            {
                if ( pAction->GetType() == SC_CAT_CONTENT )
                {
                    if ( bHandleContentAction )
                    {
                        // do nothing
                        //mpTrack->SelectContent( pAction );
                    }
                }
                else
                {
                    if ( bHandleNonContentAction )
                    {
                        // do nothing
                        //mpTrack->Accept( pAction );
                    }
                }
            }
        }
    }
    else
    {
        ScConflictsListEntry* pConflictEntry = ScConflictsListHelper::GetOwnActionEntry(
            mrConflictsList, pAction->GetActionNumber() );
        if ( pConflictEntry )
        {
            ScConflictAction eConflictAction = pConflictEntry->meConflictAction;
            if ( eConflictAction == SC_CONFLICT_ACTION_KEEP_MINE )
            {
                if ( pAction->GetType() == SC_CAT_CONTENT )
                {
                    if ( bHandleContentAction )
                    {
                        // do nothing
                        //mpTrack->SelectContent( pAction );
                    }
                }
                else
                {
                    if ( bHandleNonContentAction )
                    {
                        // do nothing
                        //mpTrack->Accept( pAction );
                    }
                }
            }
            else if ( eConflictAction == SC_CONFLICT_ACTION_KEEP_OTHER )
            {
                if ( pAction->GetType() == SC_CAT_CONTENT )
                {
                    if ( bHandleContentAction )
                    {
                        mpTrack->Reject( pAction );
                    }
                }
                else
                {
                    if ( bHandleNonContentAction )
                    {
                        mpTrack->Reject( pAction );
                    }
                }
            }
        }
    }
}


//=============================================================================
// class ScConflictsListBox
//=============================================================================

//UNUSED2008-05  ScConflictsListBox::ScConflictsListBox( Window* pParent, WinBits nBits )
//UNUSED2008-05      :SvxRedlinTable( pParent, nBits )
//UNUSED2008-05  {
//UNUSED2008-05  }

ScConflictsListBox::ScConflictsListBox( Window* pParent, const ResId& rResId )
    :SvxRedlinTable( pParent, rResId )
{
}

ScConflictsListBox::~ScConflictsListBox()
{
}

//UNUSED2008-05  sal_uLong ScConflictsListBox::GetRootEntryPos( const SvLBoxEntry* pRootEntry ) const
//UNUSED2008-05  {
//UNUSED2008-05      sal_uLong nPos = 0;
//UNUSED2008-05      SvLBoxEntry* pEntry = GetRootLevelParent( First() );
//UNUSED2008-05      while ( pEntry )
//UNUSED2008-05      {
//UNUSED2008-05          if ( pEntry == pRootEntry )
//UNUSED2008-05          {
//UNUSED2008-05              return nPos;
//UNUSED2008-05          }
//UNUSED2008-05          pEntry = NextSibling( pEntry );
//UNUSED2008-05          ++nPos;
//UNUSED2008-05      }
//UNUSED2008-05      return 0xffffffff;
//UNUSED2008-05  }


//=============================================================================
// class ScConflictsDlg
//=============================================================================

ScConflictsDlg::ScConflictsDlg( Window* pParent, ScViewData* pViewData, ScDocument* pSharedDoc, ScConflictsList& rConflictsList )
    :ModalDialog( pParent, ScResId( RID_SCDLG_CONFLICTS ) )
    ,maFtConflicts      ( this, ScResId( FT_CONFLICTS ) )
    ,maLbConflicts      ( this, ScResId( LB_CONFLICTS ) )
    ,maBtnKeepMine      ( this, ScResId( BTN_KEEPMINE ) )
    ,maBtnKeepOther     ( this, ScResId( BTN_KEEPOTHER ) )
    ,maFlConflicts      ( this, ScResId( FL_CONFLICTS ) )
    ,maBtnKeepAllMine   ( this, ScResId( BTN_KEEPALLMINE ) )
    ,maBtnKeepAllOthers ( this, ScResId( BTN_KEEPALLOTHERS ) )
    ,maBtnCancel        ( this, ScResId( BTN_CANCEL ) )
    ,maBtnHelp          ( this, ScResId( BTN_HELP ) )
    ,maStrTitleConflict ( ScResId( STR_TITLE_CONFLICT ) )
    ,maStrTitleAuthor   ( ScResId( STR_TITLE_AUTHOR ) )
    ,maStrTitleDate     ( ScResId( STR_TITLE_DATE ) )
    ,maStrUnknownUser   ( ScResId( STR_UNKNOWN_USER ) )
    ,mpViewData         ( pViewData )
    ,mpOwnDoc           ( NULL )
    ,mpOwnTrack         ( NULL )
    ,mpSharedDoc        ( pSharedDoc )
    ,mpSharedTrack      ( NULL )
    ,mrConflictsList    ( rConflictsList )
    ,maDialogSize       ( GetSizePixel() )
    ,mbInSelectHdl      ( false )
    ,mbInDeselectHdl    ( false )
{
    DBG_ASSERT( mpViewData, "ScConflictsDlg CTOR: mpViewData is null!" );
    mpOwnDoc = ( mpViewData ? mpViewData->GetDocument() : NULL );
    DBG_ASSERT( mpOwnDoc, "ScConflictsDlg CTOR: mpOwnDoc is null!" );
    mpOwnTrack = ( mpOwnDoc ? mpOwnDoc->GetChangeTrack() : NULL );
    DBG_ASSERT( mpOwnTrack, "ScConflictsDlg CTOR: mpOwnTrack is null!" );
    DBG_ASSERT( mpSharedDoc, "ScConflictsDlg CTOR: mpSharedDoc is null!" );
    mpSharedTrack = ( mpSharedDoc ? mpSharedDoc->GetChangeTrack() : NULL );
    DBG_ASSERT( mpSharedTrack, "ScConflictsDlg CTOR: mpSharedTrack is null!" );

    FreeResource();

    SetMinOutputSizePixel( maDialogSize );

    long nTabs[] = { 3, 10, 216, 266 };
    maLbConflicts.SetTabs( nTabs );

    String aTab( sal_Unicode( '\t' ) );
    String aHeader( maStrTitleConflict );
    aHeader += aTab;
    aHeader += maStrTitleAuthor;
    aHeader += aTab;
    aHeader += maStrTitleDate;
    maLbConflicts.InsertHeaderEntry( aHeader, HEADERBAR_APPEND, HIB_LEFT | HIB_LEFTIMAGE | HIB_VCENTER );

    maLbConflicts.SetStyle( maLbConflicts.GetStyle() | WB_HASLINES | WB_CLIPCHILDREN | WB_HASBUTTONS | WB_HASBUTTONSATROOT | WB_HSCROLL );
    maLbConflicts.SetSelectionMode( MULTIPLE_SELECTION );
    maLbConflicts.SetHighlightRange();

    maSelectionTimer.SetTimeout( 100 );
    maSelectionTimer.SetTimeoutHdl( LINK( this, ScConflictsDlg, UpdateSelectionHdl ) );

    maLbConflicts.SetSelectHdl( LINK( this, ScConflictsDlg, SelectHandle ) );
    maLbConflicts.SetDeselectHdl( LINK( this, ScConflictsDlg, DeselectHandle ) );

    maBtnKeepMine.SetClickHdl( LINK( this, ScConflictsDlg, KeepMineHandle ) );
    maBtnKeepOther.SetClickHdl( LINK( this, ScConflictsDlg, KeepOtherHandle ) );
    maBtnKeepAllMine.SetClickHdl( LINK( this, ScConflictsDlg, KeepAllMineHandle ) );
    maBtnKeepAllOthers.SetClickHdl( LINK( this, ScConflictsDlg, KeepAllOthersHandle ) );

    UpdateView();

    SvLBoxEntry* pEntry = maLbConflicts.First();
    if ( pEntry != NULL )
    {
        maLbConflicts.Select( pEntry );
    }
}

ScConflictsDlg::~ScConflictsDlg()
{
}

String ScConflictsDlg::GetConflictString( const ScConflictsListEntry& rConflictEntry )
{
    String aString;
    if ( mpOwnTrack )
    {
        const ScChangeAction* pAction = mpOwnTrack->GetAction( rConflictEntry.maOwnActions[ 0 ] );
        if ( pAction && mpOwnDoc )
        {
            SCTAB nTab = pAction->GetBigRange().MakeRange().aStart.Tab();
            mpOwnDoc->GetName( nTab, aString );
        }
    }
    return aString;
}

String ScConflictsDlg::GetActionString( const ScChangeAction* pAction, ScDocument* pDoc )
{
    String aString;

    DBG_ASSERT( pAction, "ScConflictsDlg::GetActionString(): pAction is null!" );
    DBG_ASSERT( pDoc, "ScConflictsDlg::GetActionString(): pDoc is null!" );
    if ( pAction && pDoc )
    {
        String aDesc;
        pAction->GetDescription( aDesc, pDoc, sal_True, false );
        aString += aDesc;
        aString += '\t';

        String aUser = pAction->GetUser();
        aUser.EraseLeadingAndTrailingChars();
        if ( aUser.Len() == 0 )
        {
            aUser = maStrUnknownUser;
        }
        aString += aUser;
        aString += '\t';

        DateTime aDateTime = pAction->GetDateTime();
        aString += ScGlobal::pLocaleData->getDate( aDateTime );
        aString += ' ';
        aString += ScGlobal::pLocaleData->getTime( aDateTime, sal_False );
        aString += '\t';
    }

    return aString;
}

void ScConflictsDlg::HandleListBoxSelection( bool bSelectHandle )
{
    SvLBoxEntry* pSelEntry = maLbConflicts.GetCurEntry();
    if ( !pSelEntry )
    {
        pSelEntry = maLbConflicts.FirstSelected();
    }
    if ( !pSelEntry )
    {
        return;
    }

    SvLBoxEntry* pRootEntry = maLbConflicts.GetRootLevelParent( pSelEntry );
    if ( pRootEntry )
    {
        if ( bSelectHandle )
        {
            maLbConflicts.SelectAll( sal_False );
        }
        if ( !maLbConflicts.IsSelected( pRootEntry ) )
        {
            maLbConflicts.Select( pRootEntry );
        }
        SvLBoxEntry* pEntry = maLbConflicts.FirstChild( pRootEntry );
        while ( pEntry )
        {
            if ( !maLbConflicts.IsSelected( pEntry ) )
            {
                maLbConflicts.Select( pEntry );
            }
            pEntry = maLbConflicts.NextSibling( pEntry );
        }
    }
}

IMPL_LINK( ScConflictsDlg, SelectHandle, SvxRedlinTable*, EMPTYARG )
{
    if ( mbInSelectHdl || mbInDeselectHdl )
    {
        return 0;
    }

    mbInSelectHdl = true;
    HandleListBoxSelection( true );
    maSelectionTimer.Start();
    mbInSelectHdl = false;

    return 0;
}

IMPL_LINK( ScConflictsDlg, DeselectHandle, SvxRedlinTable*, EMPTYARG )
{
    if ( mbInDeselectHdl || mbInSelectHdl )
    {
        return 0;
    }

    mbInDeselectHdl = true;
    HandleListBoxSelection( false );
    mbInDeselectHdl = false;

    return 0;
}

IMPL_LINK( ScConflictsDlg, UpdateSelectionHdl, Timer*, EMPTYARG )
{
    if ( !mpViewData || !mpOwnDoc )
    {
        return 0;
    }

    ScTabView* pTabView = mpViewData->GetView();
    pTabView->DoneBlockMode();
    sal_Bool bContMark = sal_False;
    SvLBoxEntry* pEntry = maLbConflicts.FirstSelected();
    while ( pEntry )
    {
        if ( pEntry != maLbConflicts.GetRootLevelParent( pEntry ) )
        {
            RedlinData* pUserData = static_cast< RedlinData* >( pEntry->GetUserData() );
            if  ( pUserData )
            {
                ScChangeAction* pAction = static_cast< ScChangeAction* >( pUserData->pData );
                if ( pAction && ( pAction->GetType() != SC_CAT_DELETE_TABS ) &&
                     ( pAction->IsClickable() || pAction->IsVisible() ) )
                {
                    const ScBigRange& rBigRange = ( static_cast< const ScChangeAction* >( pAction ) )->GetBigRange();
                    if ( rBigRange.IsValid( mpOwnDoc ) )
                    {
                        sal_Bool bSetCursor = !maLbConflicts.NextSelected( pEntry );
                        pTabView->MarkRange( rBigRange.MakeRange(), bSetCursor, bContMark );
                        bContMark = sal_True;
                    }
                }
            }
        }
        pEntry = maLbConflicts.NextSelected( pEntry );
    }

    return 0;
}

void ScConflictsDlg::SetConflictAction( SvLBoxEntry* pRootEntry, ScConflictAction eConflictAction )
{
    RedlinData* pUserData = static_cast< RedlinData* >( pRootEntry ? pRootEntry->GetUserData() : NULL );
    ScConflictsListEntry* pConflictEntry = static_cast< ScConflictsListEntry* >( pUserData ? pUserData->pData : NULL );
    if ( pConflictEntry )
    {
        pConflictEntry->meConflictAction = eConflictAction;
    }
}

void ScConflictsDlg::KeepHandler( bool bMine )
{
    SvLBoxEntry* pEntry = maLbConflicts.FirstSelected();
    SvLBoxEntry* pRootEntry = ( pEntry ? maLbConflicts.GetRootLevelParent( pEntry ) : NULL );
    if ( !pRootEntry )
    {
        return;
    }
    SetPointer( Pointer( POINTER_WAIT ) );
    ScConflictAction eConflictAction = ( bMine ? SC_CONFLICT_ACTION_KEEP_MINE : SC_CONFLICT_ACTION_KEEP_OTHER );
    SetConflictAction( pRootEntry, eConflictAction );
    maLbConflicts.RemoveEntry( pRootEntry );
    SetPointer( Pointer( POINTER_ARROW ) );
    if ( maLbConflicts.GetEntryCount() == 0 )
    {
        EndDialog( RET_OK );
    }
}

void ScConflictsDlg::KeepAllHandler( bool bMine )
{
    SvLBoxEntry* pEntry = maLbConflicts.First();
    SvLBoxEntry* pRootEntry = ( pEntry ? maLbConflicts.GetRootLevelParent( pEntry ) : NULL );
    if ( !pRootEntry )
    {
        return;
    }
    SetPointer( Pointer( POINTER_WAIT ) );
    ScConflictAction eConflictAction = ( bMine ? SC_CONFLICT_ACTION_KEEP_MINE : SC_CONFLICT_ACTION_KEEP_OTHER );
    while ( pRootEntry )
    {
        SetConflictAction( pRootEntry, eConflictAction );
        pRootEntry = maLbConflicts.NextSibling( pRootEntry );
    }
    maLbConflicts.SetUpdateMode( sal_False );
    maLbConflicts.Clear();
    maLbConflicts.SetUpdateMode( sal_True );
    SetPointer( Pointer( POINTER_ARROW ) );
    EndDialog( RET_OK );
}

IMPL_LINK( ScConflictsDlg, KeepMineHandle, void*, EMPTYARG )
{
    KeepHandler( true );

    return 0;
}

IMPL_LINK( ScConflictsDlg, KeepOtherHandle, void*, EMPTYARG )
{
    KeepHandler( false );

    return 0;
}

IMPL_LINK( ScConflictsDlg, KeepAllMineHandle, void*, EMPTYARG )
{
    KeepAllHandler( true );

    return 0;
}

IMPL_LINK( ScConflictsDlg, KeepAllOthersHandle, void*, EMPTYARG )
{
    KeepAllHandler( false );

    return 0;
}

void lcl_MoveControlX( Window& rWindow, long nDelta )
{
    Point aPos( rWindow.GetPosPixel() );
    aPos.X() += nDelta;
    rWindow.SetPosPixel( aPos );
}

void lcl_MoveControlY( Window& rWindow, long nDelta )
{
    Point aPos( rWindow.GetPosPixel() );
    aPos.Y() += nDelta;
    rWindow.SetPosPixel( aPos );
}

void lcl_ChangeControlWidth( Window& rWindow, long nDelta )
{
    Size aSize( rWindow.GetSizePixel() );
    aSize.Width() += nDelta;
    rWindow.SetSizePixel( aSize );
}

void lcl_ChangeControlHeight( Window& rWindow, long nDelta )
{
    Size aSize( rWindow.GetSizePixel() );
    aSize.Height() += nDelta;
    rWindow.SetSizePixel( aSize );
}

void ScConflictsDlg::Resize()
{
    Size aSize( GetSizePixel() );
    long nDeltaWidth = aSize.Width() - maDialogSize.Width();
    long nDeltaHeight = aSize.Height() - maDialogSize.Height();
    maDialogSize = aSize;

    lcl_ChangeControlWidth( maFtConflicts, nDeltaWidth );

    lcl_ChangeControlWidth( maLbConflicts, nDeltaWidth );
    lcl_ChangeControlHeight( maLbConflicts, nDeltaHeight );

    lcl_MoveControlX( maBtnKeepMine, nDeltaWidth / 2 );
    lcl_MoveControlY( maBtnKeepMine, nDeltaHeight );

    lcl_MoveControlX( maBtnKeepOther, nDeltaWidth / 2 );
    lcl_MoveControlY( maBtnKeepOther, nDeltaHeight );

    lcl_MoveControlY( maFlConflicts, nDeltaHeight );
    lcl_ChangeControlWidth( maFlConflicts, nDeltaWidth );

    lcl_MoveControlX( maBtnKeepAllMine, nDeltaWidth );
    lcl_MoveControlY( maBtnKeepAllMine, nDeltaHeight );

    lcl_MoveControlX( maBtnKeepAllOthers, nDeltaWidth );
    lcl_MoveControlY( maBtnKeepAllOthers, nDeltaHeight );

    lcl_MoveControlX( maBtnCancel, nDeltaWidth );
    lcl_MoveControlY( maBtnCancel, nDeltaHeight );

    lcl_MoveControlX( maBtnHelp, nDeltaWidth );
    lcl_MoveControlY( maBtnHelp, nDeltaHeight );
}

void ScConflictsDlg::UpdateView()
{
    ScConflictsList::iterator aEndItr = mrConflictsList.end();
    for ( ScConflictsList::iterator aItr = mrConflictsList.begin(); aItr != aEndItr; ++aItr )
    {
        ScConflictsListEntry* pConflictEntry = &(*aItr);
        if ( pConflictEntry && pConflictEntry->meConflictAction == SC_CONFLICT_ACTION_NONE )
        {
            RedlinData* pRootUserData = new RedlinData();
            pRootUserData->pData = static_cast< void* >( pConflictEntry );
            SvLBoxEntry* pRootEntry = maLbConflicts.InsertEntry( GetConflictString( *aItr ), pRootUserData );

            ScChangeActionList::const_iterator aEndShared = aItr->maSharedActions.end();
            for ( ScChangeActionList::const_iterator aItrShared = aItr->maSharedActions.begin(); aItrShared != aEndShared; ++aItrShared )
            {
                ScChangeAction* pAction = mpSharedTrack->GetAction( *aItrShared );
                if ( pAction )
                {
                    // only display shared top content entries
                    if ( pAction->GetType() == SC_CAT_CONTENT )
                    {
                        ScChangeActionContent* pNextContent = ( dynamic_cast< ScChangeActionContent* >( pAction ) )->GetNextContent();
                        if ( pNextContent && aItr->HasSharedAction( pNextContent->GetActionNumber() ) )
                        {
                            continue;
                        }
                    }

                    String aString( GetActionString( pAction, mpSharedDoc ) );
                    maLbConflicts.InsertEntry( aString, static_cast< RedlinData* >( NULL ), pRootEntry );
                }
            }

            ScChangeActionList::const_iterator aEndOwn = aItr->maOwnActions.end();
            for ( ScChangeActionList::const_iterator aItrOwn = aItr->maOwnActions.begin(); aItrOwn != aEndOwn; ++aItrOwn )
            {
                ScChangeAction* pAction = mpOwnTrack->GetAction( *aItrOwn );
                if ( pAction )
                {
                    // only display own top content entries
                    if ( pAction->GetType() == SC_CAT_CONTENT )
                    {
                        ScChangeActionContent* pNextContent = ( dynamic_cast< ScChangeActionContent* >( pAction ) )->GetNextContent();
                        if ( pNextContent && aItr->HasOwnAction( pNextContent->GetActionNumber() ) )
                        {
                            continue;
                        }
                    }

                    String aString( GetActionString( pAction, mpOwnDoc ) );
                    RedlinData* pUserData = new RedlinData();
                    pUserData->pData = static_cast< void* >( pAction );
                    maLbConflicts.InsertEntry( aString, pUserData, pRootEntry );
                }
            }

            maLbConflicts.Expand( pRootEntry );
        }
    }
}
