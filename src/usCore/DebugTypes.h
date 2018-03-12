#ifndef GW_DEBUG_TYPE_INCLUDE
#define GW_DEBUG_TYPE_INCLUDE

typedef enum 
{
    MD_NONE        = 0x00000000,

    MD_TERRAIN     = 0x00000001,
    MD_ASTRO       = 0x00000002,
    MD_FLIGHT      = 0x00000004,
    MD_INPUT       = 0x00000008,
    MD_GL          = 0x00000010,
    MD_VIEW        = 0x00000020,
    MD_COCKPIT     = 0x00000040,
    MD_GENERAL     = 0x00000080,
    MD_MATH        = 0x00000100,
    MD_EVENT       = 0x00000200,
    MD_AIRCRAFT    = 0x00000400,
    MD_AUTOPILOT   = 0x00000800,
    MD_IO          = 0x00001000,
    MD_CLIPPER     = 0x00002000,
    MD_NETWORK     = 0x00004000,
    MD_ATC         = 0x00008000,
    MD_NASAL       = 0x00010000,
    MD_INSTR       = 0x00020000,
    MD_SYSTEMS     = 0x00040000,
    MD_AI          = 0x00080000,
    MD_ENVIRONMENT = 0x00100000,
    MD_SOUND       = 0x00200000,
    MD_NAVAID      = 0x00400000,
    MD_GUI         = 0x00800000,
    MD_TERRASYNC   = 0x01000000,
    MD_UNDEFD      = 0x02000000, // For range checking

    MD_ALL         = 0xFFFFFFFF
} mdDebugClass;


typedef enum 
{
    MD_BULK = 1,     
    MD_DEBUG,        
    MD_INFO,         
    MD_WARN,         
    MD_ALERT         
    // MD_EXIT,      
    // MD_ABORT      
} gwDebugPriority;

#endif //GW_DEBUG_TYPE_INCLUDE
