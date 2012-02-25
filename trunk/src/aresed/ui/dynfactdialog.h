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

#ifndef __appares_dynfactdialog_h
#define __appares_dynfactdialog_h

#include <crystalspace.h>

#include "../models/model.h"
#include "meshview.h"

#include <wx/wx.h>
#include <wx/imaglist.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/xrc/xmlres.h>

#include "imesh/bodymesh.h"

class TreeCtrlView;
class ListCtrlView;
class FactoryEditorModel;

class DynfactDialog;
class ColliderCollectionValue;

class UIManager;

using namespace Ares;

/**
 * A mesh view that knows how to setup the colliders
 * for the given mesh.
 */
class DynfactMeshView : public MeshView
{
private:
  DynfactDialog* dialog;
  size_t normalPen;
  size_t hilightPen;
  size_t originXPen;
  size_t originYPen;
  size_t originZPen;
  size_t bonePen;
  size_t boneActivePen;
  size_t boneHiPen;

  void SetupColliderGeometry ();

public:
  DynfactMeshView (DynfactDialog* dialog, iObjectRegistry* object_reg, wxWindow* parent);
  virtual ~DynfactMeshView () { }

  virtual void SyncValue (Ares::Value* value);
  void Refresh ();
};

/**
 * Action to edit the category of an item.
 */
class EditCategoryAction : public Action
{
private:
  DynfactDialog* dialog;

public:
  EditCategoryAction (DynfactDialog* dialog) : dialog (dialog) { }
  virtual ~EditCategoryAction () { }
  virtual const char* GetName () const { return "Edit category..."; }
  virtual bool Do (View* view, wxWindow* component);
};

/**
 * Action to create a body skeleton.
 */
class CreateBodySkeletonAction : public Action
{
private:
  DynfactDialog* dialog;

public:
  CreateBodySkeletonAction (DynfactDialog* dialog) : dialog (dialog) { }
  virtual ~CreateBodySkeletonAction () { }
  virtual const char* GetName () const { return "Create skeleton"; }
  virtual bool Do (View* view, wxWindow* component);
};

/**
 * The composite value representing the selected dynamic factory.
 */
class DynfactValue : public CompositeValue
{
private:
  DynfactDialog* dialog;

protected:
  virtual void ChildChanged (Value* child);

public:
  DynfactValue (DynfactDialog* dialog);
  virtual ~DynfactValue () { }
};

/**
 * The composite value representing the selected bone.
 */
class BoneValue : public CompositeValue
{
private:
  DynfactDialog* dialog;

protected:
  virtual void ChildChanged (Value* child);

public:
  BoneValue (DynfactDialog* dialog);
  virtual ~BoneValue () { }
};

/**
 * A float value that calculates v = offset +- size/2 in both directions.
 */
class Offset2MinMaxValue : public FloatValue
{
private:
  csRef<StandardChangeListener> listener;
  csRef<Value> offset;
  csRef<Value> size;
  bool operatorPlus;

public:
  Offset2MinMaxValue (Value* offset, Value* size, bool operatorPlus);
  virtual ~Offset2MinMaxValue ();
  virtual void SetFloatValue (float fl)
  {
    if (operatorPlus)
    {
      float min = offset->GetFloatValue () - size->GetFloatValue () / 2.0f;
      offset->SetFloatValue ((fl+min) / 2.0f);
      size->SetFloatValue (fl-min);
    }
    else
    {
      float max = offset->GetFloatValue () + size->GetFloatValue () / 2.0f;
      offset->SetFloatValue ((fl+max) / 2.0f);
      size->SetFloatValue (max-fl);
    }
  }
  virtual float GetFloatValue ()
  {
    if (operatorPlus)
      return offset->GetFloatValue () + size->GetFloatValue () / 2.0f;
    else
      return offset->GetFloatValue () - size->GetFloatValue () / 2.0f;
  }
};

/**
 * This standard action creates a new child for a collection based
 * on a suggestion from a dialog.
 * It assumes the collection supports the NewValue() method.
 */
class NewInvisibleChildAction : public AbstractNewAction
{
public:
  NewInvisibleChildAction (Value* collection) :
    AbstractNewAction (collection) { }
  virtual ~NewInvisibleChildAction () { }
  virtual const char* GetName () const { return "New Invisible..."; }
  virtual bool Do (View* view, wxWindow* component);
};

/**
 * The dialog for editing dynamic factories.
 */
class DynfactDialog : public wxDialog, public View
{
private:
  UIManager* uiManager;
  csRef<iTimerEvent> timerOp;

  csRef<CS::Animation::iBodyManager> bodyManager;

  DynfactMeshView* meshView;
  csRef<ListSelectedValue> bonesSelectedValue;
  csRef<ListSelectedValue> bonesColliderSelectedValue;
  csRef<ListSelectedValue> colliderSelectedValue;
  csRef<ListSelectedValue> pivotsSelectedValue;
  csRef<ListSelectedValue> jointsSelectedValue;
  csRef<TreeSelectedValue> factorySelectedValue;

  csRef<DynfactValue> dynfactValue;
  csRef<BoneValue> boneValue;

  UIDialog* factoryDialog;
  UIDialog* attributeDialog;
  UIDialog* selectBoneDialog;

  void OnOkButton (wxCommandEvent& event);

  void SetupDialogs ();
  void SetupListHeadings ();
  void SetupColliderEditor (Value* colSelValue, const char* suffix);
  void SetupJointsEditor (Value* jointsSelValue);
  void SetupSelectedValues ();
  void SetupActions ();

public:
  DynfactDialog (wxWindow* parent, UIManager* uiManager);
  ~DynfactDialog ();

  void Show ();
  void Tick ();

  /// Calculate the best-fit for a given collider.
  void FitCollider (iDynamicFactory* fact, csColliderGeometryType type);

  UIManager* GetUIManager () const { return uiManager; }
  CS::Animation::iBodyManager* GetBodyManager () const { return bodyManager; }

  iDynamicFactory* GetCurrentFactory ();
  CS::Animation::iBodyBone* GetCurrentBone ();
  CS::Animation::iSkeletonFactory* GetSkeletonFactory (const char* factName);
  DynfactMeshView* GetMeshView () const { return meshView; }

  long GetSelectedCollider ();
  long GetSelectedPivot ();
  long GetSelectedJoint ();
  csString GetSelectedBone ();
  Value* GetColliderSelectedValue () const { return colliderSelectedValue; }

  DECLARE_EVENT_TABLE ();
};

#endif // __appares_dynfactdialog_h

