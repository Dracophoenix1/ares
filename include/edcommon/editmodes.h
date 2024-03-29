/*
The MIT License

Copyright (c) 2010 by Jorrit Tyberghein

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

#ifndef __aresed_editmodes_h
#define __aresed_editmodes_h

#include "csutil/csstring.h"
#include "csutil/scfstr.h"
#include "imarker.h"
#include "propclass/dynworld.h"

#include "edcommon/model.h"
#include "editor/imode.h"

struct i3DView;

class EditingMode;

class ARES_EDCOMMON_EXPORT MarkerCallback : public scfImplementation1<MarkerCallback,iMarkerCallback>
{
private:
  EditingMode* editmode;

public:
  MarkerCallback (EditingMode* editmode) : scfImplementationType (this),
    editmode (editmode) { }
  virtual ~MarkerCallback () { }
  virtual void StartDragging (iMarker* marker, iMarkerHitArea* area,
      const csVector3& pos, uint button, uint32 modifiers);
  virtual void MarkerWantsMove (iMarker* marker, iMarkerHitArea* area,
      const csVector3& pos);
  virtual void MarkerWantsRotate (iMarker* marker, iMarkerHitArea* area,
      const csReversibleTransform& transform);
  virtual void StopDragging (iMarker* marker, iMarkerHitArea* area);
};

class ARES_EDCOMMON_EXPORT EditingMode : public scfImplementation3<EditingMode, iEditingMode,
  iEditorPlugin, iCommandHandler>
{
protected:
  Ares::View view;
  iObjectRegistry* object_reg;
  iAresEditor* app;
  i3DView* view3d;
  csRef<iMarkerManager> markerMgr;
  csRef<iGraphics3D> g3d;
  csRef<iKeyboardDriver> kbd;
  csString name;
  int mouseX, mouseY;
  int colorWhite;
  bool started;

public:
  EditingMode (i3DView* view, iObjectRegistry* object_reg, const char* name);
  EditingMode (iBase* parent);
  virtual ~EditingMode () { }

  virtual void SetApplication (iAresEditor* app);
  i3DView* Get3DView () const { return view3d; }

  virtual void SetTopLevelParent (wxWindow* toplevel) { }
  virtual bool HasMainPanel () const { return false; }
  virtual void BuildMainPanel (wxWindow* parent) { }
  virtual void ReadConfig () { }

  virtual bool Initialize (iObjectRegistry* object_reg);

  virtual bool Command (csStringID id, const csString& args, bool checked)
  {
    return false;
  }
  virtual bool IsCommandValid (csStringID id, const csString& args,
      iSelection* selection, size_t pastesize)
  {
    return false;
    //if (name == "Copy" || name == "Paste" || name == "Delete") return false;
    //return true;
  }
  virtual csPtr<iString> GetAlternativeLabel (csStringID id,
      iSelection* selection, size_t pastesize)
  {
    return 0;
  }

  virtual void AllocContextHandlers (wxFrame* frame) { }
  virtual void AddContextMenu (wxMenu* contextMenu, int mouseX, int mouseY) { }
  virtual bool IsContextMenuAllowed () { return true; }

  virtual void Start () { started = true; }
  virtual void Stop () { started = false; }
  virtual void Refresh () { }

  virtual void SelectResource (iObject* resource) { }

  virtual csRef<iString> GetStatusLine ()
  {
    csRef<iString> str;
    str.AttachNew (new scfString (""));
    return str;
  }

  virtual void CurrentObjectsChanged (const csArray<iDynamicObject*>& current) { }

  virtual const char* GetPluginName () const { return name; }
  virtual void FramePre() { }
  virtual void Frame3D() { }
  virtual void Frame2D() { }
  virtual bool OnKeyboard (iEvent& ev, utf32_char code) { return false; }
  virtual bool OnMouseDown (iEvent& ev, uint but, int mouseX, int mouseY) { return false; }
  virtual bool OnMouseUp (iEvent& ev, uint but, int mouseX, int mouseY) { return false; }
  virtual bool OnMouseMove (iEvent& ev, int mouseX, int mouseY)
  {
    EditingMode::mouseX = mouseX;
    EditingMode::mouseY = mouseY;
    return false;
  }
  virtual void OnFocusLost () { }

  virtual void MarkerStartDragging (iMarker* marker, iMarkerHitArea* area,
      const csVector3& pos, uint button, uint32 modifiers) { }
  virtual void MarkerWantsMove (iMarker* marker, iMarkerHitArea* area,
      const csVector3& pos) { }
  virtual void MarkerWantsRotate (iMarker* marker, iMarkerHitArea* area,
      const csReversibleTransform& transform) { }
  virtual void MarkerStopDragging (iMarker* marker, iMarkerHitArea* area) { }
};

#endif // __aresed_editmodes_h
