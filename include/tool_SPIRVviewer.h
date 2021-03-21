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

#ifndef SPIRV_VIEWER_
#define SPIRV_VIEWER_

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include "tool_framework.h"
#include <cross/spirv_hlsl.hpp>
#include <cross/spirv_glsl.hpp>
#include <cross/spirv_msl.hpp>
#include <cross/spirv_common.hpp>
#include <cross/spirv.hpp>

#include <shaderc/shaderc.hpp>

enum ShaderType
{
	HLSL_TYPE,
	GLSL_TYPE,
	MSL_TYPE,
	UNKNOWN_TYPE
};

//store type, binary and sources
struct shaderModule_t
{
	enum moduleType_t : unsigned int
	{
		vertex,
		tessControl,
		tessEvaluation,
		geometry,
		fragment,
		compute,
		kernel,
		unknown,
		numStages,
		invalid = (unsigned int)-1
	};

	std::vector<uint32_t>					binaryList = {};
	std::string								hlslSource = {};
	std::string								glslSource = {};
	std::string								mslSource = {};
	std::string								spirvSource = {};
	spirv_cross::ShaderResources			shaderResources = {};
	spirv_cross::CompilerGLSL::Options		shaderOptions = {};
	std::string								intPrecision = {};
	std::string								floatPrecision = {};
	moduleType_t							moduleType = moduleType_t::invalid;
};
	//spv::ExecutionModel						
// -------------------------------------------------------- PipelineLayoutTool -----------------------------------------------

class shaderTool_t : public ToolFramework
{
	std::string								fileName = "default.vert.spv";
	std::vector<shaderModule_t>				shaderModules;
	ShaderType								shaderType = GLSL_TYPE;
	const char*								currentItem = "GLSL source code";
	const char*								entityItems[3] = { "GLSL source code","HLSL source code","MSL source code" };

	unsigned int currentModule = 0;
    // ---------------------- Names list UI helper ----------------------

    int DisplayNamedList(const char* title, const char* listboxName, const char* objname, const char* abbrev,
                         std::vector<const char*> listItems, int& activeItem, int listSize,
                         char *newNameBuffer, char *renameBuffer, bool *listChanged = nullptr);
	void DrawMenu();
	void DrawMeta();
	void DrawShaderTypes();
	void DrawShaderReflection();
	void DrawSPIRV(ImVec2 dimensions);
	void DrawHLSL(ImVec2 dimensions);
	void DrawGLSL(ImVec2 dimensions);
	void DrawMSL(ImVec2 dimensions);

	bool CheckShaderType(shaderModule_t& module, shaderc::AssemblyCompilationResult& result);
	void DetermineShaderModuleType(shaderModule_t& module, spv::ExecutionModel model);

	void Save(std::string fileName);
	void Load(std::string fileName);

	std::vector<uint32_t> ReadSPIRVFile(const char* fileName);

	bool IsAsciiSPIRVFile(const char* fileName);
	void ReadFromAsciiSPIRVFile(const char* fileName);

public:
    const char* GetWindowTitle(void) override;
    void Init(void) override;
    void Render(int screenWidth, int screenHeight) override;
	void SetShaderType(ShaderType type) { shaderType = type; }
	ShaderType GetShaderType() { return shaderType; }

	std::string resourcePath;
	std::string binaryPath;
};

#endif //SPIRV_VIEWER_