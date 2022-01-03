#pragma once
#include <cstdint>
#include "hud.h"

class CHudSpeedometer : public CHudBase
{
	uint16_t speed;

	cvar_t* hud_speedometer;
	cvar_t* hud_speedometer_below_cross;
	cvar_t* hud_speedometer_height;

public:
	virtual bool Init();
	virtual bool VidInit();
	virtual bool Draw(float time);

	void UpdateSpeed(const float velocity[2]);
};