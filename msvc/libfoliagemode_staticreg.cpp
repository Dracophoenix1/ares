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
static char const metainfo_foliagemode[] =
"<?xml version=\"1.0\"?>"
"<!-- foliagemode.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>ares.editor.modes.foliage</name>"
"        <implementation>FoliageMode</implementation>"
"        <description>AresEd Foliage Editing Mode</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef FoliageMode_FACTORY_REGISTER_DEFINED 
  #define FoliageMode_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(FoliageMode) 
  #endif

class foliagemode
{
SCF_REGISTER_STATIC_LIBRARY(foliagemode,metainfo_foliagemode)
  #ifndef FoliageMode_FACTORY_REGISTERED 
  #define FoliageMode_FACTORY_REGISTERED 
    FoliageMode_StaticInit FoliageMode_static_init__; 
  #endif
public:
 foliagemode();
};
foliagemode::foliagemode() {}

}
