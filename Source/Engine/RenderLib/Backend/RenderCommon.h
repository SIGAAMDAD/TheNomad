#ifndef __SIRENGINE_RENDER_COMMON_H__
#define __SIRENGINE_RENDER_COMMON_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <glm/glm.hpp>

namespace SIREngine::RenderLib {

typedef struct DrawVert_s {
	glm::vec4 Color;
	glm::vec2 TexCoords;
	glm::vec2 Position;
	glm::vec2 WorldPos;
} DrawVert_t;

};

#endif