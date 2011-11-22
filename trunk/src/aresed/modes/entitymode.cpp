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

#include "../apparesed.h"
#include "../camerawin.h"
#include "entitymode.h"

#include "physicallayer/pl.h"
#include "physicallayer/entitytpl.h"
#include "tools/questmanager.h"

#include <wx/xrc/xmlres.h>
#include <wx/listbox.h>

//---------------------------------------------------------------------------

BEGIN_EVENT_TABLE(EntityMode::Panel, wxPanel)
  EVT_LISTBOX (XRCID("templateList"), EntityMode::Panel :: OnTemplateSelect)
END_EVENT_TABLE()

//---------------------------------------------------------------------------

EntityMode::EntityMode (wxWindow* parent, AresEdit3DView* aresed3d)
  : EditingMode (aresed3d, "Entity")
{
  panel = new Panel (parent, this);
  parent->GetSizer ()->Add (panel, 1, wxALL | wxEXPAND);
  wxXmlResource::Get()->LoadPanel (panel, parent, wxT ("EntityModePanel"));
  iMarkerManager* mgr = aresed3d->GetMarkerManager ();
  view = mgr->CreateGraphView ();
  view->Clear ();

  view->SetVisible (false);

  InitColors ();
}

EntityMode::~EntityMode ()
{
  aresed3d->GetMarkerManager ()->DestroyGraphView (view);
}

iMarkerColor* EntityMode::NewColor (const char* name,
    float r0, float g0, float b0, float r1, float g1, float b1, bool fill)
{
  iMarkerManager* mgr = aresed3d->GetMarkerManager ();
  iMarkerColor* col = mgr->CreateMarkerColor (name);
  col->SetRGBColor (SELECTION_NONE, r0, g0, b0, 1);
  col->SetRGBColor (SELECTION_SELECTED, r1, g1, b1, 1);
  col->SetRGBColor (SELECTION_ACTIVE, r1, g1, b1, 1);
  col->SetPenWidth (SELECTION_NONE, 1.2f);
  col->SetPenWidth (SELECTION_SELECTED, 1.2f);
  col->SetPenWidth (SELECTION_ACTIVE, 1.2f);
  col->EnableFill (SELECTION_NONE, fill);
  col->EnableFill (SELECTION_SELECTED, fill);
  col->EnableFill (SELECTION_ACTIVE, fill);
  return col;
}

void EntityMode::InitColors ()
{
  iMarkerManager* mgr = aresed3d->GetMarkerManager ();

  iMarkerColor* textColor = NewColor ("viewWhite", .7, .7, .7, 1, 1, 1, false);

  styleTemplate = mgr->CreateGraphNodeStyle ();
  styleTemplate->SetBorderColor (NewColor ("templateColorFG", .7, .7, .7, 1, 1, 1, false));
  styleTemplate->SetBackgroundColor (NewColor ("templateColorBG", .1, .4, .5, .2, .6, .7, true));
  styleTemplate->SetTextColor (textColor);

  stylePC = mgr->CreateGraphNodeStyle ();
  stylePC->SetBorderColor (NewColor ("pcColorFG", 0, 0, .7, 0, 0, 1, false));
  stylePC->SetBackgroundColor (NewColor ("pcColorBG", .1, .4, .5, .2, .6, .7, true));
  stylePC->SetTextColor (textColor);

  styleState = mgr->CreateGraphNodeStyle ();
  styleState->SetBorderColor (NewColor ("stateColorFG", 0, .7, 0, 0, 1, 0, false));
  styleState->SetBackgroundColor (NewColor ("stateColorBG", .1, .4, .5, .2, .6, .7, true));
  styleState->SetTextColor (textColor);

  styleResponse = mgr->CreateGraphNodeStyle ();
  styleResponse->SetBorderColor (NewColor ("respColorFG", 0, .7, .7, 0, 1, 1, false));
  styleResponse->SetBackgroundColor (NewColor ("respColorBG", .3, .6, .7, .4, .7, .8, true));
  styleResponse->SetRoundness (1);
  styleResponse->SetTextColor (NewColor ("respColorTxt", 0, 0, 0, 0, 0, 0, false));

  styleReward = mgr->CreateGraphNodeStyle ();
  styleReward->SetBorderColor (NewColor ("rewColorFG", 0, .7, .7, 0, 1, 1, false));
  styleReward->SetBackgroundColor (NewColor ("rewColorBG", .3, .6, .7, .4, .7, .8, true));
  styleReward->SetRoundness (1);
  styleReward->SetTextColor (textColor);

  view->SetDefaultNodeStyle (stylePC);

  iMarkerColor* thickLinkColor = mgr->CreateMarkerColor ("thickLink");
  thickLinkColor->SetRGBColor (SELECTION_NONE, .5, .5, 0, .5);
  thickLinkColor->SetRGBColor (SELECTION_SELECTED, 1, 1, 0, .5);
  thickLinkColor->SetRGBColor (SELECTION_ACTIVE, 1, 1, 0, .5);
  thickLinkColor->SetPenWidth (SELECTION_NONE, 1.2f);
  thickLinkColor->SetPenWidth (SELECTION_SELECTED, 2.0f);
  thickLinkColor->SetPenWidth (SELECTION_ACTIVE, 2.0f);
  thinLinkColor = mgr->CreateMarkerColor ("thinLink");
  thinLinkColor->SetRGBColor (SELECTION_NONE, .5, .5, 0, .5);
  thinLinkColor->SetRGBColor (SELECTION_SELECTED, 1, 1, 0, .5);
  thinLinkColor->SetRGBColor (SELECTION_ACTIVE, 1, 1, 0, .5);
  thinLinkColor->SetPenWidth (SELECTION_NONE, 0.8f);
  thinLinkColor->SetPenWidth (SELECTION_SELECTED, 0.8f);
  thinLinkColor->SetPenWidth (SELECTION_ACTIVE, 0.8f);
  arrowLinkColor = mgr->CreateMarkerColor ("arrowLink");
  arrowLinkColor->SetRGBColor (SELECTION_NONE, 0, .5, .5, .5);
  arrowLinkColor->SetRGBColor (SELECTION_SELECTED, 0, 1, 1, .5);
  arrowLinkColor->SetRGBColor (SELECTION_ACTIVE, 0, 1, 1, .5);
  arrowLinkColor->SetPenWidth (SELECTION_NONE, 0.5f);
  arrowLinkColor->SetPenWidth (SELECTION_SELECTED, 0.5f);
  arrowLinkColor->SetPenWidth (SELECTION_ACTIVE, 0.5f);

  view->SetLinkColor (thickLinkColor);
}

void EntityMode::SetupItems ()
{
  wxListBox* list = XRCCTRL (*panel, "templateList", wxListBox);
  list->Clear ();
  iCelPlLayer* pl = aresed3d->GetPlLayer ();
  wxArrayString names;
  for (size_t i = 0 ; i < pl->GetEntityTemplateCount () ; i++)
  {
    iCelEntityTemplate* tpl = pl->GetEntityTemplate (i);
    wxString name = wxString (tpl->GetName (), wxConvUTF8);
    names.Add (name);
  }
  list->InsertItems (names, 0);
}

void EntityMode::Start ()
{
  aresed3d->GetApp ()->GetCameraWindow ()->Hide ();
  SetupItems ();
  view->SetVisible (true);
}

void EntityMode::Stop ()
{
  view->SetVisible (false);
}

const char* EntityMode::GetTriggerType (iTriggerFactory* trigger)
{
  {
    csRef<iTimeoutTriggerFactory> s = scfQueryInterface<iTimeoutTriggerFactory> (trigger);
    if (s) return "Timeout";
  }
  {
    csRef<iEnterSectorTriggerFactory> s = scfQueryInterface<iEnterSectorTriggerFactory> (trigger);
    if (s) return "EnterSect";
  }
  {
    csRef<iSequenceFinishTriggerFactory> s = scfQueryInterface<iSequenceFinishTriggerFactory> (trigger);
    if (s) return "SeqFinish";
  }
  {
    csRef<iPropertyChangeTriggerFactory> s = scfQueryInterface<iPropertyChangeTriggerFactory> (trigger);
    if (s) return "PropChange";
  }
  {
    csRef<iTriggerTriggerFactory> s = scfQueryInterface<iTriggerTriggerFactory> (trigger);
    if (s) return "Trigger";
  }
  {
    csRef<iWatchTriggerFactory> s = scfQueryInterface<iWatchTriggerFactory> (trigger);
    if (s) return "Watch";
  }
  {
    csRef<iOperationTriggerFactory> s = scfQueryInterface<iOperationTriggerFactory> (trigger);
    if (s) return "Operation";
  }
  {
    csRef<iInventoryTriggerFactory> s = scfQueryInterface<iInventoryTriggerFactory> (trigger);
    if (s) return "Inventory";
  }
  {
    csRef<iMessageTriggerFactory> s = scfQueryInterface<iMessageTriggerFactory> (trigger);
    if (s) return "Message";
  }
  {
    csRef<iMeshSelectTriggerFactory> s = scfQueryInterface<iMeshSelectTriggerFactory> (trigger);
    if (s) return "MeshSel";
  }
  return "?";
}

const char* EntityMode::GetRewardType (iRewardFactory* reward)
{
  {
    csRef<iNewStateQuestRewardFactory> s = scfQueryInterface<iNewStateQuestRewardFactory> (reward);
    if (s) return "NewState";
  }
  {
    csRef<iDebugPrintRewardFactory> s = scfQueryInterface<iDebugPrintRewardFactory> (reward);
    if (s) return "DbPrint";
  }
  {
    csRef<iInventoryRewardFactory> s = scfQueryInterface<iInventoryRewardFactory> (reward);
    if (s) return "Inventory";
  }
  {
    csRef<iSequenceRewardFactory> s = scfQueryInterface<iSequenceRewardFactory> (reward);
    if (s) return "Sequence";
  }
  {
    csRef<iCsSequenceRewardFactory> s = scfQueryInterface<iCsSequenceRewardFactory> (reward);
    if (s) return "CsSequence";
  }
  {
    csRef<iSequenceFinishRewardFactory> s = scfQueryInterface<iSequenceFinishRewardFactory> (reward);
    if (s) return "SeqFinish";
  }
  {
    csRef<iChangePropertyRewardFactory> s = scfQueryInterface<iChangePropertyRewardFactory> (reward);
    if (s) return "ChangeProp";
  }
  {
    csRef<iCreateEntityRewardFactory> s = scfQueryInterface<iCreateEntityRewardFactory> (reward);
    if (s) return "CreateEnt";
  }
  {
    csRef<iDestroyEntityRewardFactory> s = scfQueryInterface<iDestroyEntityRewardFactory> (reward);
    if (s) return "DestroyEnt";
  }
  {
    csRef<iActionRewardFactory> s = scfQueryInterface<iActionRewardFactory> (reward);
    if (s) return "Action";
  }
  {
    csRef<iMessageRewardFactory> s = scfQueryInterface<iMessageRewardFactory> (reward);
    if (s) return "Message";
  }
  return "?";
}

void EntityMode::BuildRewardGraph (iRewardFactoryArray* rewards,
    const char* parentKey, const char* pcKey)
{
  for (size_t j = 0 ; j < rewards->GetSize () ; j++)
  {
    iRewardFactory* reward = rewards->Get (j);
    csString rewKey; rewKey.Format ("r:%d,%s", j, parentKey);
    csString rewLabel; rewLabel.Format ("%d:%s", j+1, GetRewardType (reward));
    view->CreateNode (rewKey, rewLabel, styleReward);
    view->LinkNode (parentKey, rewKey, thinLinkColor);

    csRef<iNewStateQuestRewardFactory> newState = scfQueryInterface<iNewStateQuestRewardFactory> (reward);
    if (newState)
    {
      // @@@ No support for expressions here!
      csString stateKey; stateKey.Format ("S:%s,%s", newState->GetStateParameter (), pcKey);
      view->LinkNode (rewKey, stateKey, arrowLinkColor, true);
    }
  }
}

void EntityMode::BuildStateGraph (iQuestStateFactory* state,
    const char* stateKey, const char* pcKey)
{
  csRef<iQuestTriggerResponseFactoryArray> responses = state->GetTriggerResponseFactories ();
  for (size_t i = 0 ; i < responses->GetSize () ; i++)
  {
    iQuestTriggerResponseFactory* response = responses->Get (i);
    csString responseKey; responseKey.Format ("t:%d,%s", i, stateKey);
    view->CreateNode (responseKey, GetTriggerType (response->GetTriggerFactory ()), styleResponse);
    view->LinkNode (stateKey, responseKey);
    csRef<iRewardFactoryArray> rewards = response->GetRewardFactories ();
    BuildRewardGraph (rewards, responseKey, pcKey);
  }

  csRef<iRewardFactoryArray> initRewards = state->GetInitRewardFactories ();
  if (initRewards->GetSize () > 0)
  {
    csString newKeyKey; newKeyKey.Format ("i:%s", stateKey);
    view->CreateNode (newKeyKey, "I", styleResponse);
    view->LinkNode (stateKey, newKeyKey);
    BuildRewardGraph (initRewards, newKeyKey, pcKey);
  }
  csRef<iRewardFactoryArray> exitRewards = state->GetExitRewardFactories ();
  if (exitRewards->GetSize () > 0)
  {
    csString newKeyKey; newKeyKey.Format ("e:%s", stateKey);
    view->CreateNode (newKeyKey, "E", styleResponse);
    view->LinkNode (stateKey, newKeyKey);
    BuildRewardGraph (exitRewards, newKeyKey, pcKey);
  }
}

csString EntityMode::GetQuestName (iCelPropertyClassTemplate* pctpl)
{
  iCelPlLayer* pl = aresed3d->GetPlLayer ();
  csStringID newquestID = pl->FetchStringID ("NewQuest");
  size_t idx = pctpl->FindProperty (newquestID);
  if (idx != csArrayItemNotFound)
  {
    celData data;
    csRef<iCelParameterIterator> parit = pctpl->GetProperty (idx,
		    newquestID, data);
    csStringID nameID = pl->FetchStringID ("name");
    csString questName;
    while (parit->HasNext ())
    {
      csStringID parid;
      iParameter* par = parit->Next (parid);
      // @@@ We don't support expression parameters here. 'params'
      // for creating entities is missing.
      if (parid == nameID)
      {
	return par->Get (0);
      }
    }
  }
  return "";
}

void EntityMode::BuildQuestGraph (iQuestFactory* questFact, const char* pcKey)
{
  csRef<iQuestStateFactoryIterator> it = questFact->GetStates ();
  while (it->HasNext ())
  {
    iQuestStateFactory* stateFact = it->Next ();
    csString stateKey; stateKey.Format ("S:%s,%s", stateFact->GetName (), pcKey);
    view->CreateNode (stateKey, stateFact->GetName (), styleState);
    view->LinkNode (pcKey, stateKey);
    BuildStateGraph (stateFact, stateKey, pcKey);
  }
}

void EntityMode::BuildQuestGraph (iCelPropertyClassTemplate* pctpl,
    const char* pcKey)
{
  csString questName = GetQuestName (pctpl);
  if (questName.IsEmpty ()) return;

  csRef<iQuestManager> quest_mgr = csQueryRegistryOrLoad<iQuestManager> (
    aresed3d->GetObjectRegistry (),
    "cel.manager.quests");
  iQuestFactory* questFact = quest_mgr->GetQuestFactory (questName);
  // @@@ Error check
  if (questFact) BuildQuestGraph (questFact, pcKey);
}

csString EntityMode::GetExtraPCInfo (iCelPropertyClassTemplate* pctpl)
{
  csString pcName = pctpl->GetName ();
  if (pcName == "pclogic.quest")
  {
    return GetQuestName (pctpl);
  }
  return "";
}

void EntityMode::GetPCKeyLabel (iCelPropertyClassTemplate* pctpl, csString& pcKey, csString& pcLabel)
{
  csString pcShortName;
  csString pcName = pctpl->GetName ();
  size_t lastDot = pcName.FindLast ('.');
  if (lastDot != csArrayItemNotFound)
    pcShortName = pcName.Slice (lastDot+1);

  pcKey.Format ("P:%s", pcName.GetData ());
  pcLabel = pcShortName;
  if (pctpl->GetTag () != 0)
  {
    pcKey.AppendFmt (":%s", pctpl->GetTag ());
    pcLabel.AppendFmt (" (%s)", pctpl->GetTag ());
  }

  csString extraInfo = GetExtraPCInfo (pctpl);
  if (!extraInfo.IsEmpty ()) { pcLabel += '\n'; pcLabel += extraInfo; }
}

void EntityMode::BuildTemplateGraph (const char* templateName)
{
  currentTemplate = templateName;

  view->Clear ();

  view->SetVisible (false);
  iCelPlLayer* pl = aresed3d->GetPlLayer ();
  iCelEntityTemplate* tpl = pl->FindEntityTemplate (templateName);
  if (!tpl) return;

  csString tplKey; tplKey.Format ("T:%s", templateName);
  view->CreateNode (tplKey, templateName, styleTemplate);

  for (size_t i = 0 ; i < tpl->GetPropertyClassTemplateCount () ; i++)
  {
    iCelPropertyClassTemplate* pctpl = tpl->GetPropertyClassTemplate (i);

    // Extract the last part of the name (everything after the last '.').
    csString pcKey, pcLabel;
    GetPCKeyLabel (pctpl, pcKey, pcLabel);
    view->CreateNode (pcKey, pcLabel, stylePC);
    view->LinkNode (tplKey, pcKey);
    csString pcName = pctpl->GetName ();
    if (pcName == "pclogic.quest")
      BuildQuestGraph (pctpl, pcKey);
  }
  view->SetVisible (true);
}

void EntityMode::OnTemplateSelect ()
{
  wxListBox* list = XRCCTRL (*panel, "templateList", wxListBox);
  csString templateName = (const char*)list->GetStringSelection ().mb_str(wxConvUTF8);
  BuildTemplateGraph (templateName);
}

void EntityMode::OnDelete ()
{
  printf ("Delete %s\n", currentNode.GetData ());
}

void EntityMode::OnCreatePC ()
{
  printf ("CreatePC %s\n", currentNode.GetData ());
}

void EntityMode::OnEdit ()
{
  printf ("Edit %s\n", currentNode.GetData ());
}

void EntityMode::OnZoom ()
{
  iCelPlLayer* pl = aresed3d->GetPlLayer ();
  iCelEntityTemplate* tpl = pl->FindEntityTemplate (currentTemplate);
  if (!tpl) return;

  csStringArray tokens (currentNode, ":");
  csString pcName = tokens[1];
  csString tagName;
  if (tokens.GetSize () >= 3) tagName = tokens[2];
  iCelPropertyClassTemplate* pctpl = tpl->FindPropertyClassTemplate (
      pcName, tagName);
  csString questName = GetQuestName (pctpl);
  if (questName.IsEmpty ()) return;

  csRef<iQuestManager> quest_mgr = csQueryRegistryOrLoad<iQuestManager> (
    aresed3d->GetObjectRegistry (),
    "cel.manager.quests");
  iQuestFactory* questFact = quest_mgr->GetQuestFactory (questName);
  // @@@ Error check
  if (questFact)
  {
    view->Clear ();
    view->SetVisible (false);

    csString pcKey, pcLabel;
    GetPCKeyLabel (pctpl, pcKey, pcLabel);
    view->CreateNode (pcKey, pcLabel, stylePC);

    BuildQuestGraph (questFact, pcKey);
    view->SetVisible (true);
  }
}

void EntityMode::AddContextMenu (wxFrame* frame, wxMenu* contextMenu, int& id,
    int mouseX, int mouseY)
{
  currentNode = view->FindHitNode (mouseX, mouseY);
  if (!currentNode.IsEmpty ())
  {
    contextMenu->AppendSeparator ();

    const char type = currentNode[0];
    if (strchr ("TPStier", type))
    {
      contextMenu->Append (id, wxT ("Delete"));
      frame->Connect (id, wxEVT_COMMAND_MENU_SELECTED,
	  wxCommandEventHandler (EntityMode::Panel::OnDelete), 0, panel);
      id++;
    }
    if (type == 'T')
    {
      contextMenu->Append (id, wxT ("Create Property Class..."));
      frame->Connect (id, wxEVT_COMMAND_MENU_SELECTED,
	  wxCommandEventHandler (EntityMode::Panel::OnCreatePC), 0, panel);
      id++;
    }
    if (strchr ("TPtr", type))
    {
      contextMenu->Append (id, wxT ("Edit..."));
      frame->Connect (id, wxEVT_COMMAND_MENU_SELECTED,
	  wxCommandEventHandler (EntityMode::Panel::OnEdit), 0, panel);
      id++;
    }
    if (type == 'P' && currentNode.StartsWith ("P:pclogic.quest"))
    {
      contextMenu->Append (id, wxT ("Zoom"));
      frame->Connect (id, wxEVT_COMMAND_MENU_SELECTED,
	  wxCommandEventHandler (EntityMode::Panel::OnZoom), 0, panel);
      id++;
    }
  }
}

void EntityMode::ReleaseContextMenu (wxFrame* frame)
{
  frame->Disconnect (wxEVT_COMMAND_MENU_SELECTED,
	wxCommandEventHandler (EntityMode::Panel::OnDelete), 0, panel);
  frame->Disconnect (wxEVT_COMMAND_MENU_SELECTED,
	wxCommandEventHandler (EntityMode::Panel::OnCreatePC), 0, panel);
  frame->Disconnect (wxEVT_COMMAND_MENU_SELECTED,
	wxCommandEventHandler (EntityMode::Panel::OnEdit), 0, panel);
  frame->Disconnect (wxEVT_COMMAND_MENU_SELECTED,
	wxCommandEventHandler (EntityMode::Panel::OnZoom), 0, panel);
}

void EntityMode::FramePre()
{
}

void EntityMode::Frame3D()
{
  aresed3d->GetG2D ()->Clear (aresed3d->GetG2D ()->FindRGB (100, 110, 120));
}

void EntityMode::Frame2D()
{
}

bool EntityMode::OnKeyboard(iEvent& ev, utf32_char code)
{
  if (code == '1')
  {
    float f = view->GetNodeForceFactor ();
    f -= 5.0f;
    view->SetNodeForceFactor (f);
    printf ("Node force factor %g\n", f); fflush (stdout);
  }
  if (code == '2')
  {
    float f = view->GetNodeForceFactor ();
    f += 5.0f;
    view->SetNodeForceFactor (f);
    printf ("Node force factor %g\n", f); fflush (stdout);
  }
  if (code == '3')
  {
    float f = view->GetLinkForceFactor ();
    f -= 0.01f;
    view->SetLinkForceFactor (f);
    printf ("Link force factor %g\n", f); fflush (stdout);
  }
  if (code == '4')
  {
    float f = view->GetLinkForceFactor ();
    f += 0.01f;
    view->SetLinkForceFactor (f);
    printf ("Link force factor %g\n", f); fflush (stdout);
  }
  return false;
}

bool EntityMode::OnMouseDown (iEvent& ev, uint but, int mouseX, int mouseY)
{
  return false;
}

bool EntityMode::OnMouseUp(iEvent& ev, uint but, int mouseX, int mouseY)
{
  return false;
}

bool EntityMode::OnMouseMove (iEvent& ev, int mouseX, int mouseY)
{
  return false;
}
