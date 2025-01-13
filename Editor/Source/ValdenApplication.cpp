#include "ValdenApplication.h"
#include <Engine/RenderLib/Backend/RenderContext.h>
#include <Engine/RenderLib/Backend/OpenGL/OpenGLContext.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include "EditorPreferences.h"
#include "../IconsFontAwesomeEditor.h"
#include "IconsFontAwesome5.h"
#include "ImGuiNotify.hpp"
#include "Roboto-Regular.embed"

namespace Valden {

SIRENGINE_DEFINE_LOG_CATEGORY( Valden, ELogLevel::Info );

static eastl::shared_ptr<CEditorPreferences> s_pEditorPrefs;

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

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplSDL2_InitForOpenGL( RenderLib::g_pContext->GetWindow(), RenderLib::OpenGL::g_pGLContext->GetContext() );
	ImGui_ImplOpenGL3_Init();

	// Load default font
	ImFontConfig fontConfig;
	fontConfig.FontDataOwnedByAtlas = false;
	ImFont *robotoFont = io.Fonts->AddFontFromMemoryTTF( (void *)g_RobotoRegular, sizeof( g_RobotoRegular ), 20.0f, &fontConfig );
	io.FontDefault = robotoFont;

	float iconFontSize = 32.0f * 2.0f / 3.0f;

	s_pEditorPrefs = eastl::make_shared<CEditorPreferences>();

	{
		static const ImWchar szIconRanges[] = { ICON_MIN_FA_EDITOR, ICON_MAX_FA_EDITOR, 0 };
		fontConfig.MergeMode = true;
		fontConfig.PixelSnapH = true;
		fontConfig.GlyphMinAdvanceX = iconFontSize;
		io.Fonts->AddFontFromFileTTF( "IconsFontAwesomeEditor.ttf", iconFontSize, &fontConfig, szIconRanges );
	}
	{
		static const ImWchar szIconRanges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
		fontConfig.MergeMode = true;
		fontConfig.PixelSnapH = true;
		fontConfig.GlyphMinAdvanceX = iconFontSize;
		io.Fonts->AddFontFromFileTTF( FONT_ICON_FILE_NAME_FAS, iconFontSize, &fontConfig, szIconRanges );
	}
	ImGui::GetIO().Fonts->Build();
}

void CApplication::Shutdown( void )
{
	for ( auto& it : m_LayerStack ) {
		it->Shutdown();
	}

	SIRENGINE_LOG_LEVEL( Valden, ELogLevel::Info, "Shutting down ImGui..." );
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();

	ImGui::DestroyContext();
}

void CApplication::Frame( uint32_t nFrameTic )
{
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
			ImGui::SeparatorText( "CONFIGURATION" );
			ImGui::Indent( 0.5f );
			if ( ImGui::MenuItem( "Editor Preferences" ) ) {
				PushLayer( s_pEditorPrefs );
			}
			ImGui::Indent( -0.5f );
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	for ( auto& it : m_LayerStack ) {
		if ( !it->IsActive() ) {
			continue;
		}
		if ( !it->Render() ) {
			it->Shutdown();
		}
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
	ImGui::UpdatePlatformWindows();
}

void CApplication::Restart( void )
{
}

void CApplication::RegisterCvars( void )
{
}

};