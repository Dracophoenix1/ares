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

#ifndef __i3dview_h__
#define __i3dview_h__

#include "csutil/scf.h"
#include <wx/wx.h>

#include "propclass/dynworld.h"

struct iDynamicObject;
struct iDynamicFactory;
struct iSelection;
struct iDynamicCell;
struct iPcDynamicWorld;
struct iAresEditor;
struct iELCM;
struct iEditorCamera;
struct iCelPlLayer;
struct iCelEntityTemplate;
struct iAsset;
struct iPaster;
struct iModelRepository;
struct iParameterManager;
struct iMeshWrapper;

/**
 * The 3D view in the editor.
 */
struct i3DView : public virtual iBase
{
  SCF_INTERFACE(i3DView,0,0,1);

  /// Get the CS camera for the view.
  virtual iCamera* GetCsCamera () const = 0;

  /// Get the view.
  virtual iView* GetView () const = 0;

  virtual iCelPlLayer* GetPL () const = 0;
  virtual iEngine* GetEngine () const = 0;
  virtual iParameterManager* GetPM () = 0;

  /// Get the editor camera.
  virtual iEditorCamera* GetEditorCamera () const = 0;

  /// Get the light that moves with the camera.
  virtual iLight* GetCameraLight () const = 0;

  /// Time settings.
  virtual bool IsAutoTime () const = 0;
  virtual void SetAutoTime (bool a) = 0;
  virtual void ModifyCurrentTime (csTicks t) = 0;
  virtual csTicks GetCurrentTime () const = 0;
  virtual bool IsSimulation () const = 0;

  /**
   * Calculate a segment representing a beam that starts from camera
   * position towards a given point on screen.
   */
  virtual csSegment3 GetBeam (int x, int y, float maxdist = 1000.0f) = 0;
  
  /**
   * Calculate a segment representing a beam that starts from camera
   * position towards a given point on screen as pointed to by the mouse.
   */
  virtual csSegment3 GetMouseBeam (float maxdist = 1000.0f) = 0;
  
  /**
   * Given a beam, calculate the dynamic object at that position.
   */
  virtual iDynamicObject* TraceBeam (const csSegment3& beam, csVector3& isect) = 0;

  /**
   * Given a beam, see if something is selected at the position.
   * This can be a dynamic object but also static geometry.
   * Return the mesh that was hit or 0 otherwise.
   */
  virtual iMeshWrapper* TraceBeamHit (const csSegment3& beam, csVector3& isect) = 0;

  /**
   * Hit a beam with the terrain and return the intersection point.
   */
  virtual bool TraceBeamTerrain (const csVector3& start, const csVector3& end,
      csVector3& isect) = 0;

  /**
   * Get the current selection.
   */
  virtual iSelection* GetSelection () const = 0;

  /**
   * Set the static state of the current selected objects.
   */
  virtual void SetStaticSelectedObjects (bool st) = 0;

  /**
   * Set the name of the current selected objects (only works for the first
   * selected object).
   */
  virtual void ChangeNameSelectedObject (const char* name) = 0;

  /**
   * Get the dynamics system.
   */
  virtual CS::Physics::iPhysicalSector* GetDynamicSystem () const = 0;
  virtual CS::Physics::iPhysicalSystem* GetPhysicalSystem () const = 0;

  /**
   * Spawn an item. 'trans' is an optional relative transform to use for
   * the new item.
   */
  virtual iDynamicObject* SpawnItem (const csString& name,
      csReversibleTransform* trans = 0) = 0;

  /**
   * Get the current dynamic cell.
   */
  virtual iDynamicCell* GetDynamicCell () const = 0;

  /**
   * Get a reference to the dynamic world property class.
   */
  virtual iPcDynamicWorld* GetDynamicWorld () const = 0;

  /**
   * Get the size of the 3D view.
   */
  virtual int GetViewWidth () const = 0;
  virtual int GetViewHeight () const = 0;

  /**
   * Get the ares editor.
   */
  virtual iAresEditor* GetApplication  () = 0;

  /**
   * Get the ELCM.
   */
  virtual iELCM* GetELCM () const = 0;

  /// Delete selected objects.
  virtual void DeleteSelectedObjects () = 0;

  /**
   * Property remove a series of resources and make sure everything is correctly
   * fixed for this removal.
   */
  virtual void RemoveResources (const csSet<csPtrKey<iObject> >& resources) = 0;

  /**
   * Move an item to another category. If the item doesn't already exist
   * then this is equivalent to calling AddItem().
   */
  virtual void ChangeCategory (const char* newCategory, const char* itemname) = 0;

  /**
   * When the physical properties of a factory change or a new factory is created
   * we need to change various internal settings for this.
   */
  virtual void RefreshFactorySettings (iDynamicFactory* fact) = 0;

  /**
   * Get the paster.
   */
  virtual iPaster* GetPaster () = 0;

  /**
   * Get the model repository.
   */
  virtual iModelRepository* GetModelRepository () = 0;
};


#endif // __i3dview_h__

