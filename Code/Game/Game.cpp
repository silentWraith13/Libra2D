
#include "Game.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/SpriteDefinition.hpp"
#include "Engine/Math/Vec2.hpp"
#include "GameCommon.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/Map.hpp"
#include "Game/Player.hpp"
#include "Game/Entities.hpp"
#include "Game/Scorpio.hpp"
#include "Game/Leo.hpp"
#include "Game/Aries.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Renderer/SpriteAnimDefinition.hpp"
#include "Engine/Renderer/SpriteDefinition.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Math/OBB2D.hpp"
#include "Engine/Renderer/Renderer.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------------------
SpriteSheet* g_terrainSprites = nullptr;
//--------------------------------------------------------------------------------------------------------------------------------------------------------
Game::Game()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
Game::~Game()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Game::Startup()
{
	LoadAssets();
 	EnterState(GameState::ATTRACT);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Game::ShutDown()
{
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Game::Update(float deltaSeconds)
{
	if (m_currentgameState != m_nextGameState)
	{
		ExitState(m_currentgameState);
		m_currentgameState = m_nextGameState;
		EnterState(m_currentgameState);
	}

	switch (m_currentgameState)
	{

	case GameState::ATTRACT:
		UpdateAttractMode(deltaSeconds);
		break;

	case GameState::PLAYING:
		UpdatePlayingState(deltaSeconds);
		break;
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Game::Render() const
{
	switch (m_currentgameState)
	{

	case GameState::ATTRACT:
		RenderAttractMode();
		break;

	case GameState::PLAYING:
		RenderPlayingState();
		break;
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Game::RenderAttractMode() const
{
	
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Game::UpdateAttractMode(float deltaSeconds)
{
	if (g_theInput->WasKeyJustPressed(' '))
	{
		SoundID clickSoundID = g_theAudio->CreateOrGetSound(m_buttonClickSound, false);
		g_theAudio->StartSound(clickSoundID);
		m_nextGameState = GameState::PLAYING;
	}

	if (g_theInput->WasKeyJustPressed(KEYCODE_ESC))
	{
		SoundID clickSoundID = g_theAudio->CreateOrGetSound(m_buttonClickSound, false);
		g_theAudio->StartSound(clickSoundID);
		FireEvent("Quit");
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Game::LoadAssets()
{
	Texture* terrainTexture = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/Terrain_8x8.png");
	g_terrainSprites = new SpriteSheet(*terrainTexture, IntVec2(8, 8));
	TileDefinition::InitializeTileDefs();
	m_buttonClickSound = "Data/Audio/Click.mp3";
	m_mainMenuMusic = "Data/Audio/AttractMusic.mp3";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Game::UpdatePlayingState(float deltaSeconds)
{
	if (m_currentMap)
	{
		m_currentMap->Update(deltaSeconds);
	}

	if (g_theInput->WasKeyJustPressed(KEYCODE_ESC))
	{
		m_nextGameState = GameState::ATTRACT;
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Game::RenderPlayingState() const
{
	if (m_currentMap)
	{
		m_currentMap->Render();
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
SoundPlaybackID Game::PlayMusic(const std::string& musicFilePath, bool loop)
{
	SoundID musicID = g_theAudio->CreateOrGetSound(musicFilePath, false);
	return g_theAudio->StartSound(musicID, loop, m_musicVolume);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Game::EnterAttractMode()
{
	m_currentgameState = GameState::ATTRACT;
	m_mainMenuMusicPlaybackID = PlayMusic(m_mainMenuMusic, true);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Game::EnterPlayingMode()
{
	m_currentgameState = GameState::PLAYING;

	if (g_theAudio->IsPlaying(m_mainMenuMusicPlaybackID))
	{
		g_theAudio->StopSound(m_mainMenuMusicPlaybackID);
	}

	m_currentMap = new Map(this, IntVec2(20, 30));

	
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Game::EnterState(GameState state)
{
	switch (state)
	{

	case GameState::ATTRACT:
		EnterAttractMode();
		break;

	case GameState::PLAYING:
		EnterPlayingMode();
		break;
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Game::ExitState(GameState state)
{
	switch (state)
	{

	case GameState::ATTRACT:
		ExitAttractMode();
		break;

	case GameState::PLAYING:
		ExitPlayingMode();
		break;
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Game::ExitPlayingMode()
{
	m_nextGameState = GameState::ATTRACT;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Game::ExitAttractMode()
{
	m_nextGameState = GameState::PLAYING;
	g_theAudio->StopSound(m_mainMenuMusicPlaybackID);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------