#ifndef FMOD_MANAGER_H
#define FMOD_MANAGER_H
#ifdef _WIN32
#pragma once
#endif
#include "ref_params.h"

#include "fmod/fmod.hpp"
#include <string>

class CFMODManager : public CHudBase
{
public:
	CFMODManager();
	~CFMODManager();
 
	void InitFMOD();
	void ExitFMOD();

	void FadeThink();

	bool IsSoundPlaying( const char* pathToFileFromSoundsFolder );

	void PlayAmbientSound( const char* pathToFileFromSoundsFolder, bool fadeIn );
	void StopAmbientSound( bool fadeOut );

	bool MsgFunc_PlaySound(const char* pszName, int iSize, void* pbuf);
	bool MsgFunc_PrecacheSnd(const char* pszName, int iSize, void* pbuf);
	bool MsgFunc_ClrSndCache(const char* pszName, int iSize, void* pbuf);

	void Think(struct ref_params_s* pparams);

	void TransitionAmbientSounds( const char* pathToFileFromSoundsFolder );

private:
	const char* GetFullPathToSound( const char* pathToFileFromModFolder );
	const char* GetCurrentSoundName( void );
	void SetGamePath();

	bool FMODError(FMOD_RESULT *result);

	const char* currentSound;
	const char* newSoundFileToTransitionTo;
	bool m_bShouldTransition;
	bool m_bFadeIn;
	bool m_bFadeOut;
	float m_fFadeDelay;
};
 
extern CFMODManager* FMODManager();
 
#endif //FMOD_MANAGER_H