#include "ValdenApplication.h"
#include <Engine/RenderLib/Backend/RenderContext.h>
#include <Engine/RenderLib/Backend/OpenGL/OpenGLContext.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>

namespace Valden {

SIRENGINE_DEFINE_LOG_CATEGORY( Valden, ELogLevel::Info );

CApplication::CApplication( void )
{
}

CApplication::~CApplication()
{
}

void CApplication::Init( void )
{
	if ( RenderLib::vid_RenderAPI.GetValue() != (uint32_t)RenderLib::ERenderAPI::OpenGL) {
		SIRENGINE_LOG_LEVEL( Valden, ELogLevel::Fatal, "The editor will only run with OpenGL!" );
	}

	SIRENGINE_LOG_LEVEL( Valden, ELogLevel::Info, "Initializing ImGui Context..." );

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL( RenderLib::g_pContext->GetWindow(), RenderLib::OpenGL::g_pGLContext->GetContext() );
	ImGui_ImplOpenGL3_Init();
}

void CApplication::Shutdown( void )
{
	SIRENGINE_LOG_LEVEL( Valden, ELogLevel::Info, "Shutting down ImGui..." );
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();

	ImGui::DestroyContext();
}

void CApplication::Frame( uint32_t nFrameTic )
{
	if ( !System::g_pApplication ) {
		return;
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	if ( ImGui::BeginMainMenuBar() ) {
		if ( ImGui::BeginMenu( "FILE" ) ) {
			if ( ImGui::MenuItem( "EXIT" ) ) {
				System::g_pApplication->QuitGame( NULL );
				return;
			}
			ImGui::EndMenu();
		}
		if ( ImGui::BeginMenu( "EDIT" ) ) {
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGui::ShowDebugLogWindow();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
}

void CApplication::Restart( void )
{
}

void CApplication::RegisterCvars( void )
{
}

};