#pragma once
#include "Game/GameCommon.hpp"
#include "App.hpp"


//--------------------------------------------------------------------------------------------------------------------------------------------------------
struct Vec2;
class Map;
class Player;
class Scorpio;
class Leo;
class Aries;
//--------------------------------------------------------------------------------------------------------------------------------------------------------
enum class GameState
{
	NONE,
	ATTRACT,
	PLAYING,
	COUNT
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------
class Game
{
public:
	Game();
	~Game();
	void                Startup();
	void                ShutDown();
	void                Update(float deltaSeconds);
	void                Render() const;
	void				RenderAttractMode() const;
	void				UpdateAttractMode(float deltaSeconds);
	void				LoadAssets();
	void				UpdatePlayingState(float deltaSeconds);
	void				RenderPlayingState() const;
	SoundPlaybackID		PlayMusic(const std::string& musicFilePath, bool loop);
	void				EnterAttractMode();
	void				EnterPlayingMode();
	void				EnterState(GameState state);
	void				ExitState(GameState state);
	void				ExitAttractMode();
	void				ExitPlayingMode();

	
	bool				g_noClip = false;	
	bool				m_debugHeatMap;
	
	Map*				m_currentMap = nullptr;
	
	
	
	Camera              m_attractModeCamera;
	

	float				m_secondsIntoMode = 10.f;
	GameState			m_currentgameState	= GameState::ATTRACT;
	GameState			m_nextGameState		= GameState::ATTRACT;

	//Audio
	std::string			m_mainMenuMusic;
	std::string			m_buttonClickSound;
	float				m_musicVolume = 1.f;
	SoundPlaybackID		m_mainMenuMusicPlaybackID;
};