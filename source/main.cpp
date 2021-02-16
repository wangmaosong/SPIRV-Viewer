/*
 Copyright (c) 2021 UAA Software
 
 Permission is hereby granted, free of charge, to any person obtaining
 a copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:
 
 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <stdio.h>
#include <memory>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "tool_framework.hpp"
#include "tool_SPIRVviewer.hpp"
#include <stdlib.h>
#include <fstream>
#include <string>

#if defined(WIN32)
#include <Windows.h>
#endif

using namespace std;
unique_ptr<shaderTool_t> framework;

static void setGUIStyle(void)
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.GrabRounding = style.ScrollbarRounding = style.FrameRounding = 2;
    style.WindowRounding = 0;
    style.ItemSpacing.x = 4; style.FramePadding.x = 6;
    style.Colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.84f, 0.84f, 0.84f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.21f, 0.21f, 0.21f, 0.90f);
    style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
    style.Colors[ImGuiCol_Border]                = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.15f, 0.15f, 0.15f, 0.09f);
    style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.78f, 0.80f, 0.80f, 0.30f);
    style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(1.00f, 1.00f, 1.00f, 0.37f);
    style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.22f, 0.60f, 0.82f, 0.00f);
    style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.88f, 0.88f, 0.88f, 0.45f);
    style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.82f, 0.82f, 0.82f, 0.90f);
    style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.82f, 0.82f, 0.82f, 0.91f);
    style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.53f, 0.53f, 0.53f, 0.67f);
    style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.53f, 0.53f, 0.53f, 0.82f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.00f, 0.00f, 0.00f, 0.15f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.82f, 0.82f, 0.82f, 0.67f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.22f, 0.60f, 0.82f, 1.00f);
    style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
    style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
    style.Colors[ImGuiCol_SliderGrab]            = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
    style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.22f, 0.60f, 0.82f, 1.00f);
    style.Colors[ImGuiCol_Button]                = ImVec4(0.82f, 0.82f, 0.82f, 0.67f);
    style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.81f, 0.82f, 0.82f, 0.77f);
    style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.22f, 0.60f, 0.82f, 1.00f);
    style.Colors[ImGuiCol_Header]                = ImVec4(0.22f, 0.60f, 0.82f, 0.50f);
    style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.22f, 0.60f, 0.82f, 0.70f);
    style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.22f, 0.60f, 0.82f, 1.00f);
    style.Colors[ImGuiCol_Separator]             = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
    style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.22f, 0.60f, 0.82f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
    style.Colors[ImGuiCol_PlotLines]             = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.65f, 0.22f, 0.00f);
    style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.93f, 0.52f, 0.02f, 0.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.92f, 0.82f, 0.00f);
    style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.22f, 0.60f, 0.82f, 1.00f);
    style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.20f, 0.20f, 0.20f, 0.22f);

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF( std::string(framework->resourcePath + "DejaVuSansMono.ttf").c_str(), 15);
}

int main(int numArgs, char* arguments[])
{
	framework = make_unique<shaderTool_t>();
	//printf("%i \n", numArgs);
	//printf("%s \n", arguments[0]);
	//printf("%s \n", arguments[1]);
	for (int argIter = 0; argIter < numArgs; argIter++)
	{
		if (argIter > 0)
		{
			//store the binary path IF the binary was double clicked
			framework->binaryPath = arguments[1];
		}
		else
		{
			//get the exe Path
			//printf("%s \n", arguments[argIter]);
			framework->resourcePath = arguments[0];
			//remove the file name and replace it with the resources folder
			auto position = framework->resourcePath.rfind('\\');
            position = framework->resourcePath.rfind('\\', position - 2);
			if (position != std::string::npos && (position + 1) != std::string::npos)
			{
				framework->resourcePath.erase(position + 1);
				framework->resourcePath += "resources\\";
				//printf("%s \n", framework->resourcePath.c_str());
			}
		}
	}

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1600, 900, framework->GetWindowTitle(), NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    gl3wInit();

    ImGui::CreateContext();
    const char* glsl_version = "#version 130";
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    //ImGui::StyleColorsDark();
    setGUIStyle();
    framework->Init();

    // Main loop.
    int fbSizeW, fbSizeH;
    bool showDebugTestWindow = false;

    while (!glfwWindowShouldClose(window)) {
        glfwWaitEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        glfwGetFramebufferSize(window, &fbSizeW, &fbSizeH);

        framework->Render(fbSizeW, fbSizeH);
        if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT)) {
            showDebugTestWindow = true;
        }
        if (showDebugTestWindow) {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
            ImGui::ShowDemoWindow(&showDebugTestWindow);
        }

        glViewport(0, 0, fbSizeW, fbSizeH);
        glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGui::EndFrame();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}