#include <Engine/Core/Compiler.h>

#include "ngl.h"
#include "OpenGLContext.h"

#define NGL( ret, name, ... ) PFN ## name n ## name = NULL;
NGL_Core_Procs
NGL_Debug_Procs
NGL_Buffer_Procs
NGL_FBO_Procs
NGL_Shader_Procs
NGL_GLSL_SPIRV_Procs
NGL_Texture_Procs
NGL_VertexArray_Procs
NGL_BufferARB_Procs
NGL_VertexArrayARB_Procs
NGL_VertexShaderARB_Procs

NGL_ARB_direct_state_access
NGL_ARB_buffer_storage
NGL_ARB_map_buffer_range
NGL_ARB_sync
NGL_ARB_bindless_texture
NGL_ARB_transform_feedback
NGL_ARB_shader_subroutine
#undef NGL

void GL_LoadProcs( void )
{
#define NGL( ret, name, ... ) n ## name = ( PFN ## name ) SDL_GL_GetProcAddress( #name );
	NGL_Core_Procs
	NGL_Shader_Procs
	NGL_Texture_Procs
	NGL_Buffer_Procs
	NGL_GLSL_SPIRV_Procs
	NGL_Debug_Procs
	NGL_FBO_Procs
	NGL_VertexArray_Procs

	NGL_ARB_map_buffer_range
	NGL_ARB_bindless_texture
	NGL_ARB_direct_state_access
	NGL_ARB_buffer_storage
#undef NGL
}