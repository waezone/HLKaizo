#pragma once

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include <vector>
#include <string>

class CTriggerMusic : public CBaseEntity
{
public:
    //Runs when the entity spawns
    void Spawn() override;
    void Precache();
    //When another entity triggers this one
    void Use (CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;

   
};

//Items to precache elsewhere
extern std::vector<std::string> g_trackprecache;