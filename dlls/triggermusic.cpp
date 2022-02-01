#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "triggermusic.h"
#include "UserMessages.h"
#include <string>
#include <vector>

void CTriggerMusic::Spawn()
{
    Precache();
}

void CTriggerMusic::Precache()
{
    std::string filePath = "media/keygasm.mp3";
    std::string filePath2 = "media/Half-Life11.mp3";
    g_trackprecache.push_back(filePath);
    g_trackprecache.push_back(filePath2);

}

void CTriggerMusic::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{

    if (pActivator->IsPlayer())
    {
        std::string filePath = "media/keygasm.mp3";
        MESSAGE_BEGIN(MSG_ONE, gmsgPlaySound, NULL, pActivator->pev);
        WRITE_STRING(filePath.c_str());
        MESSAGE_END();
    }
}
LINK_ENTITY_TO_CLASS(trigger_music, CTriggerMusic);