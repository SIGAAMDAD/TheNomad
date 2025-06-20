cmake_minimum_required(VERSION 3.22.1)
project(SIREngineApplication VERSION 1.0 LANGUAGES CXX)

# Compiler and build configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -g -Og -std=c++17")

# Detect platform and architecture
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
		set(CMAKE_SYSTEM_PROCESSOR "x86")
	endif()
endif()

# Find packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(PANGO REQUIRED pango)
pkg_check_modules(FREETYPE2 REQUIRED freetype2)
pkg_check_modules(SDL2 REQUIRED sdl2)

# Include directories
include_directories(${GTK3_INCLUDE_DIRS} ${PANGO_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${FREETYPE2_INCLUDE_DIRS}
	../ ../Source/ ../ThirdParty/EASTL/include/ ../ThirdParty/EABase/include/Common/ ../ThirdParty/VulkanMemoryAllocator/ ../ThirdParty/glm/
	../ThirdParty/json/single_include/ ../ThirdParty/DLSS/include/ ../ThirdParty/Streamline/include/ ../ThirdParty/ ../ThirdParty/imgui/ )

# Link directories
link_directories(${GTK3_LIBRARY_DIRS} ${PANGO_LIBRARY_DIRS} ${SDL2_LIBRARY_DIRS} ${FREETYPE2_LIBRARY_DIRS})

add_definitions(-DSIRENGINE_BUILD_TYPE=1)

# Source files
set(SOURCE_FILES
	Source/ImGui/imgui_implementation.cpp
	Source/EditorLayer.h
	Source/ContentBrowser/BrowserHistory.cpp
	Source/ContentBrowser/BrowserHistory.h
	Source/ContentBrowser/BrowserInstance.cpp
	Source/ContentBrowser/BrowserInstance.h
	Source/ContentBrowser/DirectoryView.cpp
	Source/ContentBrowser/DirectoryView.h
	Source/ContentBrowser/FileView.cpp
	Source/ContentBrowser/FileView.h
	Source/ContentBrowser.cpp
	Source/ContentBrowser.h
	Source/EditorPreferences.cpp
	Source/EditorPreferences.h
	Source/ValdenApplication.cpp
	Source/ValdenApplication.h
	../Source/Engine/RenderLib/Backend/OpenGL/ngl.h
	../Source/Engine/RenderLib/Backend/OpenGL/OpenGLProcs.cpp
	../Source/Engine/RenderLib/Backend/OpenGL/OpenGLContext.h
	../Source/Engine/RenderLib/Backend/OpenGL/OpenGLContext.cpp
	../Source/Engine/RenderLib/Backend/Vulkan/VulkanCommon.h
	../Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.h
	../Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.cpp
	../Source/Engine/RenderLib/Backend/Vulkan/VulkanBuffer.h
	../Source/Engine/RenderLib/Backend/Vulkan/VulkanBuffer.cpp
	../Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.h
	../Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.cpp
	../Source/Engine/RenderLib/Backend/RenderContext.h
	../Source/Engine/RenderLib/Backend/RenderContext.cpp
	../Source/Engine/Core/Input/InputManager.cpp
	../Source/Engine/Core/Input/InputManager.h
	../Source/Engine/Core/Input/BindingManager.cpp
	../Source/Engine/Core/Input/BindingManager.h
	../Source/Engine/Core/Events/ControllerStatusEvent.h
	../Source/Engine/Core/Events/EventBase.h
	../Source/Engine/Core/Events/EventData.h
	../Source/Engine/Core/Events/EventManager.cpp
	../Source/Engine/Core/Events/EventManager.h
	../Source/Engine/Core/Events/GamepadEvent.cpp
	../Source/Engine/Core/Events/GamepadEvent.h
	../Source/Engine/Core/Events/KeyCodes.h
	../Source/Engine/Core/Events/KeyEvent.cpp
	../Source/Engine/Core/Events/KeyEvent.h
	../Source/Engine/Core/Events/MouseEvent.h
	../Source/Engine/Core/Events/TouchEvent.h
	../Source/Engine/Core/Events/WindowEvent.h
	../Source/Engine/Core/Pch.h
	../Source/Engine/Core/Platform.h
	../Source/Engine/Core/Types.h
	../Source/Engine/Core/Core.h
	../Source/Engine/Core/Util.h
	../Source/Engine/Core/Util.cpp
	../Source/Engine/Core/Serialization/Ini/ini.cpp
	../Source/Engine/Core/Serialization/Ini/ini.h
	../Source/Engine/Core/Serialization/Ini/IniReader.cpp
	../Source/Engine/Core/Serialization/Ini/IniWriter.cpp
	../Source/Engine/Core/Serialization/Ini/IniSerializer.h
	../Source/Engine/Core/Serialization/SerializerBase.h
	../Source/Engine/Core/FileSystem/CachedFile.h
	../Source/Engine/Core/FileSystem/CachedFile.cpp
	../Source/Engine/Core/FileSystem/FileWriter.h
	../Source/Engine/Core/FileSystem/FileWriter.cpp
	../Source/Engine/Core/FileSystem/FileReader.h
	../Source/Engine/Core/FileSystem/FileReader.cpp
	../Source/Engine/Core/FileSystem/FileSystem.h
	../Source/Engine/Core/FileSystem/FileSystem.cpp
	../Source/Engine/Core/FileSystem/FileList.h
	../Source/Engine/Core/FileSystem/FileList.cpp
	../Source/Engine/Core/ConsoleManager.h
	../Source/Engine/Core/ConsoleManager.cpp
	../Source/Engine/Core/ThreadSystem/Threads.h
	../Source/Engine/Core/ThreadSystem/Threads.cpp
	../Source/Engine/Core/Logging/Logger.h
	../Source/Engine/Core/Logging/Logger.cpp
	../Source/Engine/Core/System/GenericApplication/GenericApplication.h
	../Source/Engine/Core/System/GenericApplication/GenericApplication.cpp
	../Source/Engine/Core/System/Posix/PosixCrashHandler.cpp
	../Source/Engine/Core/System/Posix/PosixApplication.cpp
)

# Add executable
add_executable(Valden.x64 ${SOURCE_FILES})

target_precompile_headers(Valden.x64 PUBLIC ../Source/Engine/Core/Pch.h)

# Link libraries
target_link_libraries(Valden.x64
	${GTK3_LIBRARIES}
	${PANGO_LIBRARIES}
	${SDL2_LIBRARIES}
	-lGL
	-lvulkan
	${CMAKE_SOURCE_DIR}/libEASTL.a
	${CMAKE_SOURCE_DIR}/ThirdParty/DLSS/lib/Linux_x86_64/libnvsdk_ngx.a
	-lbacktrace
	-lfreetype
	-lSDL2
	-lboost_thread
	-lboost_chrono
	-lboost_filesystem
	-lmimalloc
)

# Shader files
#set(SHADER_FILES
#	${CMAKE_BINARY_DIR}/Resources/Shaders/Vulkan/Generic_fp.spv
#	${CMAKE_BINARY_DIR}/Resources/Shaders/Vulkan/Generic_vp.spv
#)
#add_custom_command(
#	OUTPUT ${CMAKE_BINARY_DIR}/Resources/Shaders/Vulkan/Generic_fp.spv
#	COMMAND glslangValidator -H -V -o ${CMAKE_BINARY_DIR}/Resources/Shaders/Vulkan/Generic_fp.spv -S frag ${CMAKE_SOURCE_DIR}/Resources/Shaders/Vulkan/Generic_fp.glsl
#	DEPENDS ${CMAKE_SOURCE_DIR}/Resources/Shaders/Vulkan/Generic_fp.glsl
#	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Resources/Shaders/Vulkan
#)
#add_custom_command(
#	OUTPUT ${CMAKE_BINARY_DIR}/Resources/Shaders/Vulkan/Generic_vp.spv
#	COMMAND glslangValidator -H -V -o ${CMAKE_BINARY_DIR}/Resources/Shaders/Vulkan/Generic_vp.spv -S vert ${CMAKE_SOURCE_DIR}/Resources/Shaders/Vulkan/Generic_vp.glsl
#	DEPENDS ${CMAKE_SOURCE_DIR}/Resources/Shaders/Vulkan/Generic_vp.glsl
#	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Resources/Shaders/Vulkan
#)
#add_custom_target(
#	shaders ALL
#	DEPENDS ${SHADER_FILES}
#)

# Create output directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/obj)

# Add include directories and compiler flags
target_include_directories(Valden.x64 PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_options(Valden.x64 PRIVATE
	-Wnon-virtual-dtor
	-Wnull-dereference
	-Wcast-align
	-Wduplicated-cond
	-Wduplicated-branches
	-Wlogical-op
	-Wno-format
	-Werror=pointer-arith
	-Werror=write-strings
)

# Install rules
install(TARGETS Valden.x64 DESTINATION .)

# Clean target
add_custom_target(clean-all
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/obj
	COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/Valden.x64
)