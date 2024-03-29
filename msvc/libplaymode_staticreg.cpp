// This file is automatically generated.
#include "cssysdef.h"
#include "csutil/scf.h"

// Put static linking stuff into own section.
// The idea is that this allows the section to be swapped out but not
// swapped in again b/c something else in it was needed.
#if !defined(CS_DEBUG) && defined(CS_COMPILER_MSVC)
#pragma const_seg(".CSmetai")
#pragma comment(linker, "/section:.CSmetai,r")
#pragma code_seg(".CSmeta")
#pragma comment(linker, "/section:.CSmeta,er")
#pragma comment(linker, "/merge:.CSmetai=.CSmeta")
#endif

namespace csStaticPluginInit
{
static char const metainfo_playmode[] =
"<?xml version=\"1.0\"?>"
"<!-- playmode.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>ares.editor.modes.play</name>"
"        <implementation>PlayMode</implementation>"
"        <description>AresEd Play Editing Mode</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef PlayMode_FACTORY_REGISTER_DEFINED 
  #define PlayMode_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(PlayMode) 
  #endif

class playmode
{
SCF_REGISTER_STATIC_LIBRARY(playmode,metainfo_playmode)
  #ifndef PlayMode_FACTORY_REGISTERED 
  #define PlayMode_FACTORY_REGISTERED 
    PlayMode_StaticInit PlayMode_static_init__; 
  #endif
public:
 playmode();
};
playmode::playmode() {}

}
