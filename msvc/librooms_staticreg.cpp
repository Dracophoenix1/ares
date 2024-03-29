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
static char const metainfo_rooms[] =
"<?xml version=\"1.0\"?>"
"<!-- rooms.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>utility.rooms</name>"
"        <implementation>RoomMeshCreator</implementation>"
"        <description>Rooms Mesh Plugin</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef RoomMeshCreator_FACTORY_REGISTER_DEFINED 
  #define RoomMeshCreator_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(RoomMeshCreator) 
  #endif

class rooms
{
SCF_REGISTER_STATIC_LIBRARY(rooms,metainfo_rooms)
  #ifndef RoomMeshCreator_FACTORY_REGISTERED 
  #define RoomMeshCreator_FACTORY_REGISTERED 
    RoomMeshCreator_StaticInit RoomMeshCreator_static_init__; 
  #endif
public:
 rooms();
};
rooms::rooms() {}

}
