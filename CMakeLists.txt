cmake_minimum_required (VERSION 2.6)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project (SPIRV_Viewer C CXX)
set(PROJECT_LABEL "SPIRV_Viewer")

find_package(OpenGL REQUIRED)

set (OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin/")

set (INCLUDE_DIR "./include")
set (COMMON_INCLUDE_DIR "./lib/include")
set (IMGUI_INCLUDE_DIR "./lib/include/imgui")
set (NFD_INCLUDE_DIR "./lib/include/nfd")
set (SHADERC_INCLUDE_DIR "./lib/shaderc/libshaderc/include")
set (GLFW_INCLUDE_DIR "./lib/include/glfw")

set (SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source/")
set (LIB_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/src/")

set (IMGUI_SOURCES 
	"${LIB_SOURCES_DIR}/imgui/imgui.cpp" 
	"${LIB_SOURCES_DIR}/imgui/imgui_draw.cpp" 
	"${LIB_SOURCES_DIR}/imgui/imgui_demo.cpp"
	"${LIB_SOURCES_DIR}/imgui/imgui_tables.cpp"
	"${LIB_SOURCES_DIR}/imgui/imgui_widgets.cpp"
	"${LIB_SOURCES_DIR}/imgui/imgui_impl_glfw.cpp"
	"${LIB_SOURCES_DIR}/imgui/backends/imgui_impl_opengl3.cpp")
SOURCE_GROUP(imgui FILES ${IMGUI_SOURCES})

if(WIN32)
set(NFD_SOURCES 
	"${LIB_SOURCES_DIR}/nfd/nfd_common.c" 
	"${LIB_SOURCES_DIR}/nfd/nfd_win.cpp")
elseif(UNIX)
#set(NFD_SOURCES "${NFD_SOURCES}" "${LIB_SOURCES_DIR}/") //add linux nfd later
endif()
SOURCE_GROUP(nfd FILES ${NFD_SOURCES})

set (SPIRV_SOURCES 
	"${LIB_SOURCES_DIR}/spirv/spirv_cross_parsed_ir.cpp" 
	"${LIB_SOURCES_DIR}/spirv/spirv_cfg.cpp" 
	"${LIB_SOURCES_DIR}/spirv/spirv_cpp.cpp" 
	"${LIB_SOURCES_DIR}/spirv/spirv_cross.cpp" 
	"${LIB_SOURCES_DIR}/spirv/spirv_hlsl.cpp" 
	"${LIB_SOURCES_DIR}/spirv/spirv_glsl.cpp" 
	"${LIB_SOURCES_DIR}/spirv/spirv_msl.cpp" 
	"${LIB_SOURCES_DIR}/spirv/spirv_parser.cpp")
SOURCE_GROUP(spirv FILES ${SPIRV_SOURCES})

set (GLFW_SOURCES 
	"${LIB_SOURCES_DIR}/GL/gl3w.c")
SOURCE_GROUP(glfw FILES ${GLFW_SOURCES})

set (SOURCES 
	"./source/main.cpp" 
	"./source/tool_framework.cpp" 
	"./source/tool_SPIRVviewer.cpp")

include_directories("${INCLUDE_DIR}")
include_directories("${COMMON_INCLUDE_DIR}")
include_directories("${IMGUI_INCLUDE_DIR}")
include_directories("${NFD_INCLUDE_DIR}")
include_directories("${SHADERC_INCLUDE_DIR}")
include_directories("${COMMON_INCLUDE_DIR}/cross")
include_directories("${COMMON_INCLUDE_DIR}/GL")
include_directories("${GLFW_INCLUDE_DIR}")

set(CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH}
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/find")

set (HEADERS 
	"${INCLUDE_DIR}/tool_framework.h" 
	"${INCLUDE_DIR}/resource.h" 
	"${INCLUDE_DIR}/tool_SPIRVviewer.h")

set (LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
add_subdirectory("${LIB_DIR}/shaderc" EXCLUDE_FROM_ALL)
link_directories(${LIB_DIR})
set(LIB_DIR_DEBUG "${LIB_DIR}/Debug")
set(LIB_DIR_RELEASE "${LIB_DIR}/Release")
set(LIBS "")

if (CMAKE_BUILD_TYPE MATCHES Release)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
	link_directories(${LIB_DIR} ${LIB_DIR_RELEASE})
	set (LIBS shaderc_combined jsoncpp glfw3)
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
	link_directories(${LIB_DIR} ${LIB_DIR_DEBUG})
	set (LIBS shaderc_combined jsoncppd glfw3d)
else() #by default use release flags
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
	link_directories(${LIB_DIR} ${LIB_DIR_RELEASE})
	set (LIBS shaderc_combined jsoncpp glfw3)
endif()

add_executable(SPIRV_Viewer ${SOURCES} ${IMGUI_SOURCES} ${NFD_SOURCES} ${SPIRV_SOURCES} ${GLFW_SOURCES} ${HEADERS})

target_link_libraries(SPIRV_Viewer ${LIBS} ${OPENGL_LIBRARIES})
set_property(TARGET SPIRV_Viewer PROPERTY OUTPUT_NAME "SPIRV_Viewer")
