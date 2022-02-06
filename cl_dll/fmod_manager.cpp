#include "stdio.h"
#include "stdlib.h"

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "fmod_manager.h"
#include "fmod/fmod_errors.h"
#include "filesystem_utils.h"

#include <string>
#include <vector>
#include <map>



using namespace FMOD;

extern cl_enginefunc_t gEngfuncs;

struct SoundContainer
{
	Sound *pSound;
	Channel *pChannel;
};


DECLARE_MESSAGE(m_FMODManager, PlaySound);
DECLARE_MESSAGE(m_FMODManager, PrecacheSnd);
DECLARE_MESSAGE(m_FMODManager, ClrSndCache);

std::map<std::string, SoundContainer> TrackList;

System			*pSystem;
SoundGroup		*pSoundGroup;
ChannelGroup	*pChannelGroup;
FMOD_RESULT		result;

CFMODManager gFMODMng;
CFMODManager* FMODManager()
{
	return &gFMODMng;
}

CFMODManager::CFMODManager()
{
	m_fFadeDelay = 0.0;
	newSoundFileToTransitionTo = "NULL";
	currentSound = "NULL";
	m_bShouldTransition = false;
	m_bFadeIn = false;
	m_bFadeOut = false;
	
}

CFMODManager::~CFMODManager()
{
	m_fFadeDelay = 0.0;
	newSoundFileToTransitionTo = "NULL";
	currentSound = "NULL";
	m_bShouldTransition = false;
	m_bFadeIn = false;
	m_bFadeOut = false;
}

// Starts FMOD
void CFMODManager::InitFMOD( void )
{

	HOOK_MESSAGE(PlaySound);
	HOOK_MESSAGE(PrecacheSnd);
	HOOK_MESSAGE(ClrSndCache);

	result = System_Create( &pSystem ); // Create the main system object.
	FMODError(&result);

	result = pSystem->init(100, FMOD_INIT_NORMAL, 0);   // Initialize FMOD system.
	FMODError(&result);

	result = pSystem->createChannelGroup("music", &pChannelGroup);
	FMODError(&result);

	float volume = CVAR_GET_FLOAT("MP3Volume");
	result = pChannelGroup->setVolume(volume);
	FMODError(&result);
}

// Stops FMOD
void CFMODManager::ExitFMOD( void )
{
	result = pSystem->release();
	FMODError(&result);
}

//gmsgPlaySound on server dll, plays a sound that has been precached
bool CFMODManager::MsgFunc_PlaySound(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ(pbuf, iSize);
	std::string sound = READ_STRING();

	bool paused;
	bool isPlaying;
	TrackList[sound].pChannel->isPlaying(&isPlaying);
	TrackList[sound].pChannel->getPaused(&paused);

	//return if the sound is already playing
	if (!paused && isPlaying)
		return true;

	//replay the sound if its already played
	if (!isPlaying)	
		result = pSystem->playSound(TrackList[sound].pSound, pChannelGroup, true, &TrackList[sound].pChannel);

	//unpause the sound to play it
	TrackList[sound].pChannel->setPaused(false);
	FMODError(&result);
	
	return true;
}

//gmsgPcacheSnd on server dll, precaches a sound
bool CFMODManager::MsgFunc_PrecacheSnd(const char* pszName, int iSize, void* pbuf)
{
	//setup full path to the mod folder for easy use
	char directory[128];
	g_pFileSystem->GetCurrentDirectory(directory,256);
	const char* gamefolder = gEngfuncs.pfnGetGameDirectory();
	char path[128];

	sprintf(path, "%s/%s/", directory, gamefolder);

	BEGIN_READ(pbuf, iSize);

	std::string shortPath = READ_STRING();
	std::string fullPath = path + shortPath;


	//Return if the track has already been precached
	auto it = TrackList.find(shortPath); //returns an iterator to where the element is
	if (it != TrackList.end()) //if the element isn't at the end it must exist already
		return true;

	SoundContainer track;

	//createsound caches the sound
	result = pSystem->createSound(fullPath.c_str(), FMOD_DEFAULT, NULL, &track.pSound);
	FMODError(&result);

	//set to be able to play but start paused, only unpause when its needed
	result = pSystem->playSound(track.pSound, pChannelGroup, true, &track.pChannel);
	FMODError(&result);

	TrackList.emplace(std::pair<std::string, SoundContainer>(shortPath, track));

	ConsolePrint(std::string("Sound: " + fullPath + " Precached successfully!" + "\n").c_str());
	char size[64];
	sprintf(size, "PrecacheBufferSize: %d\n", TrackList.size());
	ConsolePrint(size);

	return true;
}

bool CFMODManager::MsgFunc_ClrSndCache(const char* pszName, int iSize, void* pbuf)
{
	TrackList.clear();
	return true;
}

void CFMODManager::Think(struct ref_params_s* pparams)
{
	float volume = CVAR_GET_FLOAT("MP3Volume");
	
	pChannelGroup->setVolume(volume);

	//pause the music when the server is paused
	if (pparams->paused)
		pChannelGroup->setPaused(true);
	else
		pChannelGroup->setPaused(false);

	result = pSystem->update();
	if (result != FMOD_OK)
		ConsolePrint("Error Updating FMOD");
}

bool CFMODManager::FMODError(FMOD_RESULT *result)
{
	if (*result != FMOD_OK)
    {
        std::string fmod_error_str = FMOD_ErrorString(*result);
        std::string error_msg = "FMOD ERROR: " + *result + fmod_error_str + "\n";
        fprintf(stderr, error_msg.c_str());
        ConsolePrint(error_msg.c_str());
        return false;
    }
	return true;
}