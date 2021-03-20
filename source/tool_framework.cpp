#include "tool_framework.h"
#include <nfd.h>

ToolFramework::ToolFramework()
{
}

ToolFramework::~ToolFramework()
{
}

const char* ToolFramework::GetWindowTitle(void)
{
    return "VK ToolFramework";
}

void ToolFramework::Init(void)
{
}

void ToolFramework::Render(int screenWidth, int screenHeight)
{
}

bool ToolFramework::openDialog(std::string& out, const char* filter)
{
    nfdchar_t *outPath = nullptr;
    auto res = NFD_OpenDialog( filter, NULL, &outPath );
    if (res == NFD_OKAY) {
        out = outPath;
        free(outPath);
        return true;
    }
    return false;
}

bool ToolFramework::saveDialog(std::string& out, const char* filter)
{
    nfdchar_t *outPath = nullptr;
    auto res = NFD_SaveDialog( filter, nullptr, &outPath );
    if (res == NFD_OKAY) {
        out = outPath;
        free(outPath);
        return true;
    }
    return false;
}