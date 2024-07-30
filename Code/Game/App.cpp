#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include "Game/App.hpp"
#include "Game/Game.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Window/Window.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Game.hpp"
#include "Engine/Core/NamedProperties.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Input/InputSystem.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------------------
App*			g_theApp = nullptr;				
Renderer*		g_theRenderer = nullptr;
InputSystem*	g_theInput = nullptr;
AudioSystem*	g_theAudio = nullptr;
Window*			g_theWindow = nullptr;
Game*			g_theGame = nullptr;
//--------------------------------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void App::Startup() 
{
	m_screenCamera1.SetOrthoView(Vec2(0.f, 0.f), Vec2(1600.f, 800.f));

	//constructs the engine components
	EventSystemConfig eventSystemConfig;
	g_theEventSystem = new EventSystem(eventSystemConfig);

	RendererConfig rendererConfig;
	//rendererConfig.m_window = g_theWindow;
	g_theRenderer = new Renderer(rendererConfig);

	DevConsoleConfig devConsoleConfig;
	devConsoleConfig.m_renderer = g_theRenderer;
	devConsoleConfig.m_camera = &m_screenCamera1;
	g_theDevConsole = new DevConsole(devConsoleConfig);

	InputSystemConfig inputSystemConfig;
	g_theInput = new InputSystem(inputSystemConfig);

	WindowConfig  windowConfig;
	windowConfig.m_windowTitle = "Libra";
	windowConfig.m_clientAspect = 2.0f;
	windowConfig.m_inputSystem = g_theInput;
	g_theWindow = new Window(windowConfig);

	AudioSystemConfig audioConfig;
	g_theAudio = new AudioSystem(audioConfig);

	//Calling startup of all engine components
	g_theEventSystem->Startup();
	g_theDevConsole->Startup();
	g_theInput->Startup();
	g_theWindow->Startup();
	g_theRenderer->Startup();
	g_theAudio->Startup();
	m_clock.Reset();

	//creating a new game instance
	g_theGame = new Game();
	g_theGame->Startup();

	DisplayHelpText();

	g_theEventSystem->SubscribeEventCallbackFunction("Quit", App::Event_Quit);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void App::Shutdown()
{
	//deleting game 
	g_theGame->ShutDown();
	delete g_theGame;
	g_theGame = nullptr;

	//calling SHhut-down of all engine components
	g_theAudio->Shutdown();
	g_theRenderer->Shutdown();
	g_theWindow->Shutdown();
	g_theInput->Shutdown();
	g_theDevConsole->Shutdown();
	g_theEventSystem->Shutdown();

	//deleting engine components
	delete g_theAudio;
	g_theAudio = nullptr;

	delete g_theRenderer;
	g_theRenderer = nullptr;

	delete g_theWindow;
	g_theWindow = nullptr;

	delete g_theInput;
	g_theInput = nullptr;

	delete g_theDevConsole;
	g_theDevConsole = nullptr;

	delete g_theEventSystem;
	g_theEventSystem = nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
App::App()
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
App::~App()
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void App::RunFrame()
{
// 	static float timePrevious =static_cast<float>( GetCurrentTimeSeconds());
// 	float timeNow = static_cast<float>(GetCurrentTimeSeconds());
// 	float deltaSeconds = timeNow - timePrevious;
// 	constexpr float MAX_FRAME_SECONDS = (1.f / 10.f);
// 	
// 	if (deltaSeconds > MAX_FRAME_SECONDS)
// 	{
// 		deltaSeconds = MAX_FRAME_SECONDS;
// 	}
// 	timePrevious = timeNow;
	BeginFrame();
	Update(m_clock.GetDeltaSeconds());
	Render();
	EndFrame();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void App::Run()
{
	while (!IsQuitting() )
	{
		g_theApp->RunFrame();
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
bool App::DeveloperCheats()
{
// 	if (g_theInput->WasKeyJustPressed('Q'))
// 	{
// 		HandleQuitRequested();
// 		return true;
// 	}
// 
// 	if (g_theInput->WasKeyJustPressed(KEYCODE_F8))
// 	{
// 		m_game->ShutDown();
// 		delete m_game;
// 		m_game = new Game();
// 		m_game->Startup();
// 	}
// 
// 	if (g_theInput->WasKeyJustPressed(KEYCODE_F1))
// 	{
// 		m_game->m_debugDraw = !m_game->m_debugDraw;
// 	}
// 
// 	if (g_theInput->WasKeyJustPressed('Y'))
// 	{
// 		m_fastMode = !m_fastMode;
// 		return true;
// 	}
// 
// 	if (g_theInput->WasKeyJustPressed(KEYCODE_F3))
// 	{
// 		m_game->g_noClip = !m_game->g_noClip;
// 		return true;
// 	}
// 
// 	if (g_theInput->WasKeyJustPressed(KEYCODE_F4))
// 	{
// 		m_game->m_debugCamera = !m_game->m_debugCamera;
// 		return true;
// 	}

	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

bool App::HandleQuitRequested()
{
	m_isQuitting = true;
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void App::BeginFrame()
{
	g_theInput->BeginFrame();
	g_theAudio->BeginFrame();
	g_theWindow->BeginFrame();
	g_theRenderer->BeginFrame();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void App::Update(float deltaSeconds)
{
	Clock::TickSytemClock();

	

	if (g_theGame != nullptr)
	{
		g_theGame->Update(deltaSeconds);
	}

	if (g_theInput->WasKeyJustPressed(KEYCODE_ESC))
	{
		EventArgs args;
		FireEvent("Quit", args);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void App::Render() const
{	
	if (g_theGame)
	{
		g_theGame->Render();
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void App::EndFrame()
{
	g_theRenderer->Endframe();
	g_theWindow->EndFrame();
	g_theAudio->EndFrame();
	g_theInput->EndFrame();
}

void App::DisplayHelpText()
{
	g_theDevConsole->AddLine(Rgba8(0, 255, 0, 255), "Game Keys");
	g_theDevConsole->AddLine(Rgba8(0, 255, 0, 255), "Accelerate - E");
	g_theDevConsole->AddLine(Rgba8(0, 255, 0, 255), "Rotate down - S");
	g_theDevConsole->AddLine(Rgba8(0, 255, 0, 255), "Rotate up - W");
	g_theDevConsole->AddLine(Rgba8(0, 255, 0, 255), "Escape to attract mode - ESCAPE");
	g_theDevConsole->AddLine(Rgba8(0, 255, 0, 255), "Spawn more asteroids - I");
	g_theDevConsole->AddLine(Rgba8(0, 255, 0, 255), "Respawn - N");
	g_theDevConsole->AddLine(Rgba8(0, 255, 0, 255), "Shoot - SPACE");
	g_theDevConsole->AddLine(Rgba8(0, 255, 0, 255), "Pause - P");
	g_theDevConsole->AddLine(Rgba8(0, 255, 0, 255), "Time scale - T");
	g_theDevConsole->AddLine(Rgba8(0, 255, 0, 255), "StepThroughFrame - O");
	g_theDevConsole->AddLine(Rgba8(0, 255, 0, 255), "Correct way to set time scale - (ex) timescale 0.5");
}

bool App::Event_Quit(EventArgs& args)
{
	(void)args;

	if (!g_theApp)
	{
		return false;
	}

	g_theApp->HandleQuitRequested();
	return true;
}
