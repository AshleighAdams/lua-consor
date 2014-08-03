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

// IConsoleRenderer
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

int consor_console_renderer_renderername(lua_State* L)
{
	IConsoleRenderer* renderer = Object<IConsoleRenderer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, renderer, "argument #1 expected ConsoleRenderer");
	
	string ret = renderer->RendererName();
	Stack<string>::Push(L, ret);
	return 1;
}

int consor_console_renderer_versionstring(lua_State* L)
{
	IConsoleRenderer* renderer = Object<IConsoleRenderer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, renderer, "argument #1 expected ConsoleRenderer");
	
	string ret = renderer->VersionString();
	Stack<string>::Push(L, ret);
	return 1;
}

int consor_console_renderer_flushtoscreen(lua_State* L)
{
	IConsoleRenderer* renderer = Object<IConsoleRenderer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, renderer, "argument #1 expected ConsoleRenderer");
	
	renderer->FlushToScreen();
	return 0;
}

//int consor_console_renderer_getcharinformation(lua_State* L)

int consor_console_renderer_getsize(lua_State* L)
{
	IConsoleRenderer* renderer = Object<IConsoleRenderer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, renderer, "argument #1 expected ConsoleRenderer");
	
	Stack<Size>::Push(L, renderer->GetSize());
	return 1;
}

int consor_console_renderer_supportsunicode(lua_State* L)
{
	IConsoleRenderer* renderer = Object<IConsoleRenderer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, renderer, "argument #1 expected ConsoleRenderer");
	
	Stack<bool>::Push(L, renderer->SupportsUnicode());
	return 1;
}

int consor_console_renderer_maxcolours(lua_State* L)
{
	IConsoleRenderer* renderer = Object<IConsoleRenderer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, renderer, "argument #1 expected ConsoleRenderer");
	
	Stack<size_t>::Push(L, renderer->MaxColours());
	return 1;
}

//int consor_console_renderer_getcolours(lua_State* L)

//int consor_console_renderer_setcolours(lua_State* L)

int consor_console_renderer_resetcolours(lua_State* L)
{
	IConsoleRenderer* renderer = Object<IConsoleRenderer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, renderer, "argument #1 expected ConsoleRenderer");
	
	renderer->ResetColours();
	return 0;
}

int consor_console_renderer_requestcolour(lua_State* L)
{
	IConsoleRenderer* renderer = Object<IConsoleRenderer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, renderer, "argument #1 expected ConsoleRenderer");
	lua_check(L, Stack<Colour>::Check(L, 2), "argument #2 expected colour");
	
	Colour col = Stack<Colour>::Get(L, 2);
	bool make = Stack<bool>::Check(L, 3) ? Stack<bool>::Get(L, 3) : false;
		
	Stack<Colour>::Push(L, renderer->RequestColour(col, make));
	return 1;
}

int consor_console_renderer_flushrequestedcolours(lua_State* L)
{
	IConsoleRenderer* renderer = Object<IConsoleRenderer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, renderer, "argument #1 expected ConsoleRenderer");
	
	renderer->FlushRequestedColours();
	return 0;
}

int consor_console_renderer_settitle(lua_State* L)
{
	IConsoleRenderer* renderer = Object<IConsoleRenderer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, renderer, "argument #1 expected ConsoleRenderer");
	lua_check(L, Stack<string>::Check(L, 2), "argument #2 expected string");
	
	string title = Stack<string>::Get(L, 2);
	renderer->SetTitle(title);
	return 0;
}

int consor_console_renderer_clear(lua_State* L)
{
	IConsoleRenderer* renderer = Object<IConsoleRenderer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, renderer, "argument #1 expected ConsoleRenderer");
	lua_check(L, Stack<Colour>::Check(L, 2), "argument #2 expected colour");
	
	Colour col = Stack<Colour>::Get(L, 2);
	renderer->Clear(col);
	return 0;
}

#define FUNC(_x_) { #_x_, &_x_ }
static const luaL_Reg R[] =
{
	FUNC(consor_console_renderer_ctor),
	FUNC(consor_console_renderer_dtor),
	FUNC(consor_console_renderer_renderername),
	FUNC(consor_console_renderer_versionstring),
	FUNC(consor_console_renderer_flushtoscreen),
	FUNC(consor_console_renderer_getsize),
	FUNC(consor_console_renderer_supportsunicode),
	FUNC(consor_console_renderer_maxcolours),
	FUNC(consor_console_renderer_resetcolours),
	FUNC(consor_console_renderer_requestcolour),
	FUNC(consor_console_renderer_flushrequestedcolours),
	FUNC(consor_console_renderer_settitle),
	FUNC(consor_console_renderer_clear),
	
	//FUNC(consor_console_renderer_),
	{ NULL, NULL } //   
};

extern "C"
{
	LUALIB_API int luaopen_consor_core(lua_State* L)
	{
		luaL_newlib(L, R);
		return 1;
	}
}
