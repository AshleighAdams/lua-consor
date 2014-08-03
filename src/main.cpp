#include <unordered_map>

// Consor includes
#include <Consor/PlatformConsoleRenderer.hpp>
#include <Consor/PlatformInputSystem.hpp>

#include <Consor/Util/StringUtils.hpp>
#include <Consor/Util/Time.hpp>
#include <Consor/Util/FrequencyController.hpp>
#include <Consor/Util/Math.hpp>
#include <Consor/Util/Prompts.hpp>

#include <Consor/Controls/Label.hpp>
#include <Consor/Controls/Button.hpp>
#include <Consor/Controls/TextBox.hpp>
#include <Consor/Controls/PasswordBox.hpp>
#include <Consor/Controls/CheckBox.hpp>
#include <Consor/Controls/ProgressBar.hpp>
#include <Consor/Controls/VerticalProgressBar.hpp>
#include <Consor/Controls/HorizontalScrollbar.hpp>
#include <Consor/Controls/VerticalScrollbar.hpp>

#include <Consor/Containers/WindowContainer.hpp>
#include <Consor/Containers/BorderContainer.hpp>
#include <Consor/Containers/FlowContainer.hpp>
#include <Consor/Containers/AlignContainer.hpp>
#include <Consor/Containers/ScrollContainer.hpp>

#include <Consor/WindowSystem.hpp>

#include <lua.hpp>

using namespace std;
using namespace Consor;
using namespace Consor::Console;

#include "stackhelper.cpp"

int consor_console_renderer_ctor(lua_State* L)
{
	int renderer = Object<PlatformConsoleRenderer>::Make();
	Stack<int>::Push(L, renderer);
	return 1;
}

int consor_console_renderer_dtor(lua_State* L)
{
	int renderer = Stack<int>::Get(L, 1);
	Object<PlatformConsoleRenderer>::Destroy(renderer);
	return 0;
}


#define FUNC(_x_) { #_x_, &_x_ }
static const luaL_Reg R[] =
{
	FUNC(consor_console_renderer_ctor),
	FUNC(consor_console_renderer_dtor),
	{ NULL, NULL }
};

extern "C"
{
	LUALIB_API int luaopen_consor_core(lua_State* L)
	{
		luaL_newlib(L, R);
		return 1;
	}
}
