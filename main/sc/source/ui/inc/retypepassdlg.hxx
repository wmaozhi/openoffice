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



#ifndef SC_UI_RETYPEPASS_DLG_HXX
#define SC_UI_RETYPEPASS_DLG_HXX

#include <vcl/dialog.hxx>
#include <vcl/button.hxx>
#include <vcl/fixed.hxx>
#include <vcl/edit.hxx>
#include <vcl/scrbar.hxx>
#include <svx/checklbx.hxx>
#include <svtools/stdctrl.hxx>

#include "tabprotection.hxx"

#include <boost/shared_ptr.hpp>

class Window;
class ScDocProtection;
class ScTableProtection;
class ScDocument;

class ScRetypePassDlg : public ModalDialog
{
public:
    typedef ::boost::shared_ptr<ScDocProtection>    DocProtectionPtr;
    typedef ::boost::shared_ptr<ScTableProtection>  TabProtectionPtr;

    explicit ScRetypePassDlg(Window* pParent);
    virtual ~ScRetypePassDlg();

    virtual short Execute();

    void SetDataFromDocument(const ScDocument& rDoc);
    void SetDesiredHash(ScPasswordHash eHash);

    /** Write the new set of password data to the document instance to
        overwrite the current ones. */
    void WriteNewDataToDocument(ScDocument& rDoc) const;

private:
    ScRetypePassDlg(); // disabled

    void Init();
    void PopulateDialog();
    void SetDocData();
    void SetTableData(size_t nRowPos, SCTAB nTab);
    void ResetTableRows();

    /** Check the status of all hash values to see if it's okay to enable
        the OK button. */
    void CheckHashStatus();

private:
    OKButton        maBtnOk;
    CancelButton    maBtnCancel;
    HelpButton      maBtnHelp;

    FixedInfo       maTextDescription;

    FixedLine       maLineDocument;
    FixedText       maTextDocStatus;
    PushButton      maBtnRetypeDoc;

    FixedLine       maLineSheet;
    FixedText       maTextSheetName1;
    FixedText       maTextSheetStatus1;
    PushButton      maBtnRetypeSheet1;

    FixedText       maTextSheetName2;
    FixedText       maTextSheetStatus2;
    PushButton      maBtnRetypeSheet2;

    FixedText       maTextSheetName3;
    FixedText       maTextSheetStatus3;
    PushButton      maBtnRetypeSheet3;

    FixedText       maTextSheetName4;
    FixedText       maTextSheetStatus4;
    PushButton      maBtnRetypeSheet4;

    ScrollBar       maScrollBar;

    String          maTextNotProtected;
    String          maTextNotPassProtected;
    String          maTextHashBad;
    String          maTextHashGood;
    String          maTextHashRegen;

    DECL_LINK( OKHdl, OKButton* );
    DECL_LINK( RetypeBtnHdl, PushButton* );
    DECL_LINK( ScrollHdl, ScrollBar* );

    struct TableItem
    {
        String              maName;
        TabProtectionPtr    mpProtect;
    };
    ::std::vector<TableItem> maTableItems;

    DocProtectionPtr    mpDocItem;
    size_t              mnCurScrollPos;
    ScPasswordHash      meDesiredHash;
};

// ============================================================================

class ScRetypePassInputDlg : public ModalDialog
{
public:
    explicit ScRetypePassInputDlg(Window* pParent, ScPassHashProtectable* pProtected);
    virtual ~ScRetypePassInputDlg();

    virtual short Execute();

    bool IsRemovePassword() const;
    String GetNewPassword() const;

private:
    ScRetypePassInputDlg(); // disabled

    void Init();
    void CheckPasswordInput();

private:
    OKButton        maBtnOk;
    CancelButton    maBtnCancel;
    HelpButton      maBtnHelp;

    RadioButton     maBtnRetypePassword;

    FixedText       maPassword1Text;
    Edit            maPassword1Edit;
    FixedText       maPassword2Text;
    Edit            maPassword2Edit;

    CheckBox        maBtnMatchOldPass;

    RadioButton     maBtnRemovePassword;

    DECL_LINK( OKHdl, OKButton* );
    DECL_LINK( RadioBtnHdl, RadioButton* );
    DECL_LINK( CheckBoxHdl, CheckBox* );
    DECL_LINK( PasswordModifyHdl, Edit* );

    ScPassHashProtectable* mpProtected;
};

#endif
