#include "extdll.h"
#include "util.h"
#include "cbase.h"


class CTriggerHelloWorld : public CBaseEntity
{
    //Runs when the entity spawns
    void Spawn() override;

    //When another entity triggers this one
    void Use (CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;

};

LINK_ENTITY_TO_CLASS(trigger_helloworld, CTriggerHelloWorld);

void CTriggerHelloWorld::Spawn()
{

}

void CTriggerHelloWorld::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{

    ALERT(at_console, "REEEEE\n");
    if (pActivator->IsPlayer())
    {
        ALERT(at_console, "Hello Player\n");
    }
}