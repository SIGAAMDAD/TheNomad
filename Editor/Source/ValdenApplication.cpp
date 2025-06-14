#include "ValdenApplication.h"
#include <Engine/RenderLib/Backend/RenderContext.h>
#include <Engine/RenderLib/Backend/OpenGL/OpenGLContext.h>
#include "EditorPreferences.h"
#include "ContentBrowser.h"
#include "../IconsFontAwesomeEditor.h"
#include "IconsFontAwesome5.h"
#include "ImGuiNotify.hpp"
#include "Roboto-Regular.embed"
#include <imgui/imgui.h>

namespace Valden {

SIRENGINE_DEFINE_LOG_CATEGORY( Valden, ELogLevel::Info );

eastl::shared_ptr<CContentBrowser> g_pContentBrowser;
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

	// Load default font
	ImFontConfig fontConfig;
	fontConfig.FontDataOwnedByAtlas = false;
	ImGuiIO& io = ImGui::GetIO();
	ImFont *robotoFont = io.Fonts->AddFontFromMemoryTTF( (void *)g_RobotoRegular, sizeof( g_RobotoRegular ), 20.0f, &fontConfig );
	io.FontDefault = robotoFont;

	float iconFontSize = 32.0f * 2.0f / 3.0f;

	s_pEditorPrefs = eastl::make_shared<CEditorPreferences>();
	g_pContentBrowser = eastl::make_shared<CContentBrowser>();

	g_pContentBrowser->Init();
	g_pContentBrowser->AddInstance( SIREngine::System::GetCurrentPath() );

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
}

void CApplication::Frame( uint32_t nFrameTic )
{
	{
		ImVec2 size;
		const ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		const ImGuiViewport *viewport = ImGui::GetMainViewport();

		size = { viewport->WorkSize.x, viewport->WorkSize.y };
		//size = { viewport->WorkSize.x / 2.0f, viewport->WorkSize.y };

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;

		ImGui::SetNextWindowPos( viewport->WorkPos );
		ImGui::SetNextWindowSize( viewport->WorkSize );
		ImGui::SetNextWindowViewport( viewport->ID );
		ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 0.0f );
		ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if ( dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode ) {
			window_flags |= ImGuiWindowFlags_NoBackground;
		}

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );
		ImGui::Begin( "DockSpace Demo", nullptr, window_flags );
		ImGui::SetWindowSize( viewport->Size );
		ImGui::PopStyleVar();
		
		size = ImGui::GetWindowSize();

		ImGui::PopStyleVar( 2 );

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if ( io.ConfigFlags & ImGuiConfigFlags_DockingEnable ) {
			ImGuiID dockspace_id = ImGui::GetID( "VulkanAppDockspace" );
			ImGui::DockSpace( dockspace_id, ImVec2( 0.0f, 0.0f ), dockspace_flags );

			ImGuiDockNode *pNode = (ImGuiDockNode *)ImGui::GetCurrentContext()->DockContext.Nodes.GetVoidPtr( dockspace_id );
			if ( ImGui::DockNodeBeginAmendTabBar( pNode ) ) {
				if ( ImGui::BeginTabItem( "Test" ) ) {
					ImGui::EndTabItem();
				}
				ImGui::DockNodeEndAmendTabBar();
			}
		}
		ImGui::End();
	}

	if ( ImGui::BeginMainMenuBar() ) {
		if ( ImGui::BeginMenu( "FILE" ) ) {
			ImGui::SeparatorText( "" );
			ImGui::Indent( 1.5f );
			
			if ( ImGui::MenuItem( "EXIT" ) ) {
				System::g_pApplication->QuitGame( NULL );
				return;
			}
			ImGui::EndMenu();
		}
		if ( ImGui::BeginMenu( "EDIT" ) ) {
			ImGui::SeparatorText( "CONFIGURATION" );
			ImGui::Indent( 1.5f );
			if ( ImGui::MenuItem( "Editor Preferences" ) ) {
				PushLayer( s_pEditorPrefs );
			}
			ImGui::Indent( 1.0f );
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

	g_pContentBrowser->Render();
}

void CApplication::Restart( void )
{
}

void CApplication::RegisterCvars( void )
{
}

};