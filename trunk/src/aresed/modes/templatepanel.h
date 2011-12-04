/*
The MIT License

Copyright (c) 2011 by Jorrit Tyberghein

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 */

#ifndef __appares_templatepanel_h
#define __appares_templatepanel_h

#include <crystalspace.h>

#include <wx/wx.h>
#include <wx/imaglist.h>
#include <wx/choicebk.h>
#include <wx/listctrl.h>
#include <wx/xrc/xmlres.h>

class UIManager;
class EntityMode;
struct iCelEntityTemplate;

enum
{
  ID_TplPar_Add = wxID_HIGHEST + 10000,
  ID_TplPar_Delete,
  ID_Char_Add,
  ID_Char_Edit,
  ID_Char_Delete,
  ID_Class_Add,
  ID_Class_Delete,
};

class EntityTemplatePanel : public wxPanel
{
private:
  UIManager* uiManager;
  EntityMode* emode;
  wxSizer* parentSizer;
  iCelEntityTemplate* tpl;

  bool CheckHitList (const char* listname, bool& hasItem, const wxPoint& pos);
  void OnContextMenu (wxContextMenuEvent& event);

  void OnTemplateParentAdd (wxCommandEvent& event);
  void OnTemplateParentDelete (wxCommandEvent& event);
  void OnCharacteristicsAdd (wxCommandEvent& event);
  void OnCharacteristicsEdit (wxCommandEvent& event);
  void OnCharacteristicsDelete (wxCommandEvent& event);
  void OnClassAdd (wxCommandEvent& event);
  void OnClassDelete (wxCommandEvent& event);

  void UpdateTemplate ();

public:
  EntityTemplatePanel (wxWindow* parent, UIManager* uiManager, EntityMode* emode);
  ~EntityTemplatePanel();

  // Switch this dialog to editing of a template.
  void SwitchToTpl (iCelEntityTemplate* tpl);

  void OnTemplateRMB (bool hasItem);
  void OnCharacteristicsRMB (bool hasItem);
  void OnClassesRMB (bool hasItem);

  void Show () { wxPanel::Show (); parentSizer->Layout (); }
  void Hide () { wxPanel::Hide (); parentSizer->Layout (); }
  bool IsVisible () const { return IsShown (); }

  DECLARE_EVENT_TABLE ();
};

#endif // __appares_templatepanel_h
