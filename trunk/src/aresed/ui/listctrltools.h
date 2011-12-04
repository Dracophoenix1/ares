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

#ifndef __appares_listctrltools_h
#define __appares_listctrltools_h

#include <csutil/stringarray.h>

#include <wx/wx.h>
#include <wx/imaglist.h>
#include <wx/listctrl.h>
#include <wx/listbox.h>
#include <wx/xrc/xmlres.h>

class UIDialog;

/**
 * This class represents a row model. Instances of this interface
 * can be used by supporting views (like ListCtrlView).
 */
class RowModel : public csRefCount
{
public:
  /**
   * Reset the iterator for this rowmodel.
   * This method (together with HasRows() and NextRow()) are used to fetch
   * all the data so that the list can be filled.
   */
  virtual void ResetIterator () = 0;

  /**
   * Check if there are still rows to process.
   */
  virtual bool HasRows () = 0;

  /**
   * Get the next row.
   */
  virtual csStringArray NextRow () = 0;

  // -----------------------------------------------------------------------------

  /**
   * Start the update from the given list. This is called by the list control
   * when it is time to update the data. This call is followed by a series of
   * UpdateRow() calls and finished by a call to FinishUpdate().
   */
  virtual void StartUpdate () = 0;

  /**
   * Update a row.
   * Returns false in case of error.
   */
  virtual bool UpdateRow (const csStringArray& row) = 0;

  /**
   * Finish the update.
   */
  virtual void FinishUpdate () { }

  // -----------------------------------------------------------------------------

  /**
   * Get the columns for the list as a single ',' seperated string.
   */
  virtual const char* GetColumns () = 0;

  /**
   * Return true if this datamodel allows editing of rows.
   */
  virtual bool IsEditAllowed () const { return true; }

  // -----------------------------------------------------------------------------

  /**
   * Return a dialog with which a row in this model can be edited.
   * This can return 0 in which case the view should call EditRow()
   * for more controlled editing. The dialog should support the fields
   * with the same name as the columns in the model.
   */
  virtual UIDialog* GetEditorDialog () { return 0; }

  /**
   * Edit a given row and return the updated row.
   * If the given row is empty (contains no items) then
   * the editor will create a new row.
   * If the returned row is empty then the dialog was canceled.
   */
  virtual csStringArray EditRow (const csStringArray& origRow)
  {
    return csStringArray ();
  }
};

/**
 * A view based on a list control on top of a RowModel.
 */
class ListCtrlView : public wxEvtHandler
{
private:
  wxListCtrl* list;
  csRef<RowModel> model;
  csStringArray columns;
  UIDialog* forcedDialog;
  bool ownForcedDialog;

  void UnbindModel ();
  void BindModel (RowModel* model);

  void OnContextMenu (wxContextMenuEvent& event);
  void OnAdd (wxCommandEvent& event);
  void OnEdit (wxCommandEvent& event);
  void OnDelete (wxCommandEvent& event);

  /// This is used in case the model has a dialog to use.
  csStringArray DialogEditRow (const csStringArray& origRow);

  /// Get the right dialog.
  csStringArray DoDialog (const csStringArray& origRow);

public:
  ListCtrlView (wxListCtrl* list) : list (list), forcedDialog (0), ownForcedDialog (false) { }
  ListCtrlView (wxListCtrl* list, RowModel* model) : list (list), forcedDialog (0), ownForcedDialog (false)
  {
    BindModel (model);
  }
  ~ListCtrlView ();

  void SetModel (RowModel* model) { BindModel (model); Refresh (); }

  /**
   * Manually force an editor dialog to use. If 'own' is true
   * this view will assume ownership of the dialog and remove the
   * dialog in the destructor.
   */
  void SetEditorDialog (UIDialog* dialog, bool own = false);

  /**
   * Refresh the list from the data in the model.
   */
  void Refresh ();

  /**
   * Update the model based on the contents of the list.
   */
  void Update ();
};

/**
 * Various tools for lists.
 */
class ListCtrlTools
{
public:
  /**
   * Read a row from the list control.
   */
  static csStringArray ReadRow (wxListCtrl* list, int row);

  /**
   * Add a row to the list control (at the end).
   */
  static long AddRow (wxListCtrl* list, const char* value, ...);

  /**
   * Add a row to the list control (at the end).
   */
  static long AddRow (wxListCtrl* list, const csStringArray& values);

  /**
   * Replace a row in the list control.
   */
  static void ReplaceRow (wxListCtrl* list, int idx, const char* value, ...);

  /**
   * Replace a row in the list control.
   */
  static void ReplaceRow (wxListCtrl* list, int idx, const csStringArray& values);

  /**
   * Set the color of some row.
   */
  static void ColorRow (wxListCtrl* list, int idx,
    unsigned char red, unsigned char green, unsigned char blue);

  /**
   * Set the background color of some row.
   */
  static void BackgroundColorRow (wxListCtrl* list, int idx,
    unsigned char red, unsigned char green, unsigned char blue);

  /**
   * Add a column.
   */
  static void SetColumn (wxListCtrl* list, int idx, const char* name, int width);

  /**
   * Find a row index based on a given value in a column.
   * Returns -1 if not found.
   */
  static long FindRow (wxListCtrl* list, int col, const char* value);

  /**
   * Return the index of the first selected row or -1 if nothing is selected.
   */
  static long GetFirstSelectedRow (wxListCtrl* list);

  /**
   * Check if a list control is hit with the given point (in screen space).
   * This is mostly used in combination with context menus.
   * 'hasItem' will be set to true if the point points to an item in the list
   * control.
   */
  static bool CheckHitList (wxListCtrl* list, bool& hasItem, const wxPoint& pos);

  /**
   * Check if a list box is hit with the given point (in screen space).
   * This is mostly used in combination with context menus.
   * 'hasItem' will be set to true if the point points to an item in the list
   * control.
   */
  static bool CheckHitList (wxListBox* list, bool& hasItem, const wxPoint& pos);
};

#endif // __appares_listctrltools_h

