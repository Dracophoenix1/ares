/*
The MIT License

Copyright (c) 2012 by Jorrit Tyberghein

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

#ifndef __appares_entityparameters_h
#define __appares_entityparameters_h

#include <crystalspace.h>

#include "edcommon/model.h"
#include "edcommon/inspect.h"
#include "edcommon/smartpicker.h"

#include <wx/wx.h>
#include <wx/imaglist.h>
#include <wx/listctrl.h>
#include <wx/xrc/xmlres.h>

class UIManager;
struct iDynamicObject;

using namespace Ares;

class ParameterListValue;

class EntityParameterDialog : public wxDialog, public View
{
private:
  UIManager* uiManager;
  iCelPlLayer* pl;
  wxPanel* parameterPanel;
  csHash<wxWindow*,csStringID> parameterToComponent;

  SmartPickerLogic spl;

  void OnOkButton (wxCommandEvent& event);
  void OnCancelButton (wxCommandEvent& event);
  void OnResetButton (wxCommandEvent& event);
  void OnUpdateTemplate (wxCommandEvent& event);

  csRef<ParameterListValue> parameters;
  iDynamicObject* object;

  wxBoxSizer* AddRow (wxBoxSizer* sizer);

  void AddEntityParameter (csStringID parID, ParameterDomain& par, wxBoxSizer* sizer);
  void AddTemplateParameter (csStringID parID, ParameterDomain& par, wxBoxSizer* sizer);
  void AddGenericParameter (csStringID parID, ParameterDomain& par, wxBoxSizer* sizer,
      const char* label1, const char* label2);
  void AddLinkedParameters (csStringID parID, ParameterDomain& par,
    wxBoxSizer* sizer, wxBoxSizer* rowSizer, const char* label1, const char* label2);

  void FillSemanticParameters (iDynamicObject* object);
  void UpdateParameters (iDynamicObject* object);

  iCelEntityTemplate* GetTemplate ();

public:
  EntityParameterDialog (wxWindow* parent, UIManager* uiManager);
  ~EntityParameterDialog ();

  void SuggestParameters ();

  void Show (iDynamicObject* object);

  DECLARE_EVENT_TABLE ();
};

#endif // __appares_entityparameters_h

