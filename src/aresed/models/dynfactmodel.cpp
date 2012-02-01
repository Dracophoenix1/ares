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

#include <crystalspace.h>

#include "dynfactmodel.h"
#include "meshfactmodel.h"
#include "../tools/tools.h"
#include "../ui/uimanager.h"
#include "../apparesed.h"

using namespace Ares;

void CategoryCollectionValue::UpdateChildren ()
{
  if (!dirty) return;
  dirty = false;
  ReleaseChildren ();
  const csHash<csStringArray,csString>& categories = aresed3d->GetCategories ();
  const csStringArray& items = categories.Get (category, csStringArray ());
  for (size_t i = 0 ; i < items.GetSize () ; i++)
  {
    csRef<StringValue> strValue;
    strValue.AttachNew (new StringValue (items[i]));
    children.Push (strValue);
    strValue->SetParent (this);
  }
}

Value* CategoryCollectionValue::FindChild (const char* name)
{
  csString sName = name;
  for (size_t i = 0 ; i < children.GetSize () ; i++)
    if (sName == children[i]->GetStringValue ())
      return children[i];
  return 0;
}

void DynfactCollectionValue::UpdateChildren ()
{
  if (!dirty) return;
  dirty = false;
  ReleaseChildren ();
  const csHash<csStringArray,csString>& categories = aresed3d->GetCategories ();
  csHash<csStringArray,csString>::ConstGlobalIterator it = categories.GetIterator ();
  while (it.HasNext ())
  {
    csString category;
    it.Next (category);
    csRef<CategoryCollectionValue> catValue;
    catValue.AttachNew (new CategoryCollectionValue (aresed3d, category));
    children.Push (catValue);
    catValue->SetParent (this);
  }
}

bool DynfactCollectionValue::DeleteValue (Value* child)
{
  if (!child)
  {
    aresed3d->GetApp ()->GetUIManager ()->Error ("Please select an item!");
    return false;
  }
  Value* categoryValue = GetCategoryForValue (child);
  if (!categoryValue)
  {
    aresed3d->GetApp ()->GetUIManager ()->Error ("Please select an item!");
    return false;
  }
  if (categoryValue == child)
  {
    aresed3d->GetApp ()->GetUIManager ()->Error ("Can't delete an entire category at once!");
    return false;
  }

  iPcDynamicWorld* dynworld = aresed3d->GetDynamicWorld ();
  iDynamicFactory* factory = dynworld->FindFactory (child->GetStringValue ());
  CS_ASSERT (factory != 0);
  dynworld->RemoveFactory (factory);
  aresed3d->RemoveItem (categoryValue->GetStringValue (), child->GetStringValue ());

  dirty = true;
  FireValueChanged ();

  return true;
}

Value* DynfactCollectionValue::NewValue (size_t idx, Value* selectedValue,
    const DialogResult& suggestion)
{
  csString newname = suggestion.Get ("name", (const char*)0);
  if (newname.IsEmpty ())
  {
    aresed3d->GetApp ()->GetUIManager ()->Error ("Enter a valid mesh!");
    return 0;
  }
  Value* categoryValue = GetCategoryForValue (selectedValue);
  if (!categoryValue)
  {
    aresed3d->GetApp ()->GetUIManager ()->Error ("Please select a category!");
    return 0;
  }

  aresed3d->AddItem (categoryValue->GetStringValue (), newname);
  csRef<StringValue> strValue;
  strValue.AttachNew (new StringValue (newname));

  iPcDynamicWorld* dynworld = aresed3d->GetDynamicWorld ();
  iDynamicFactory* fact = dynworld->AddFactory (newname, 1.0f, 1.0f);
  fact->SetAttribute ("category", categoryValue->GetStringValue ());
  aresed3d->SetupFactorySettings (fact);

  CategoryCollectionValue* categoryCollectionValue = static_cast<CategoryCollectionValue*> (categoryValue);
  categoryCollectionValue->Refresh ();
  FireValueChanged ();
  return categoryCollectionValue->FindChild (newname);
}

Value* DynfactCollectionValue::GetCategoryForValue (Value* value)
{
  for (size_t i = 0 ; i < children.GetSize () ; i++)
    if (children[i] == value) return value;
    else if (children[i]->IsChild (value)) return children[i];
  return 0;
}

Value* DynfactCollectionValue::FindValueForItem (const char* itemname)
{
  for (size_t i = 0 ; i < children.GetSize () ; i++)
  {
    CategoryCollectionValue* cvalue = static_cast<CategoryCollectionValue*> (
	children[i]);
    Value* value = cvalue->FindChild (itemname);
    if (value) return value;
  }
  return 0;
}
