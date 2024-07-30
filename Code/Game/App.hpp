#pragma once
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/Clock.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------------------
class Game;
class InputSystem;
//--------------------------------------------------------------------------------------------------------------------------------------------------------
class App
{
public:
	App();
	~App();
	void             Startup();
	void             Shutdown();
	void             RunFrame();
	void             Run();
	bool             DeveloperCheats();
	bool             IsQuitting() const { return m_isQuitting; }
	bool             HandleQuitRequested();
	void			 BeginFrame();
	void			 Update(float deltaSeconds);
	void			 Render() const;
	void			 EndFrame();
	void			 DisplayHelpText();
	static bool		 Event_Quit(EventArgs& args);
	
	bool				m_isQuitting = false;
	bool				m_fastMode = false;
	Camera				m_worldCamera;
	Camera				m_screenCamera1;
	Clock				m_clock;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------

