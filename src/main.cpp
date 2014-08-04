#include <unordered_map>

// Consor includes
#include <Consor/PlatformConsoleRenderer.hpp>
#include <Consor/PlatformInputSystem.hpp>

#include <Consor/Util/StringUtils.hpp>
#include <Consor/Util/Time.hpp>
#include <Consor/Util/FrequencyController.hpp>
#include <Consor/Util/Math.hpp>
#include <Consor/Util/Prompts.hpp>

// All Consor controls
#include <Consor/Controls/Button.hpp>
#include <Consor/Controls/CheckBox.hpp>
#include <Consor/Controls/Graph.hpp>
#include <Consor/Controls/HorizontalScrollbar.hpp>
#include <Consor/Controls/Label.hpp>
#include <Consor/Controls/PasswordBox.hpp>
#include <Consor/Controls/ProgressBar.hpp>
#include <Consor/Controls/RadioBox.hpp>
#include <Consor/Controls/TextBox.hpp>
#include <Consor/Controls/VerticalProgressBar.hpp>
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
using namespace Consor::Input;

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

int consor_console_renderer_drawbox(lua_State* L)
{
	IConsoleRenderer* renderer = Object<IConsoleRenderer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, renderer, "argument #1 expected ConsoleRenderer");
	lua_check(L, Stack<Vector>::Check(L, 2), "argument #2 expected vector");
	lua_check(L, Stack<Size>::Check(L, 3), "argument #3 expected size");
	lua_check(L, Stack<Colour>::Check(L, 4), "argument #4 expected colour");
	
	auto pos = Stack<Vector>::Get(L, 2);
	auto size = Stack<Size>::Get(L, 3);
	auto col = Stack<Colour>::Get(L, 4);
	
	renderer->DrawBox(pos, size, col);
	return 0;
}

int consor_console_renderer_drawrect(lua_State* L)
{
	IConsoleRenderer* renderer = Object<IConsoleRenderer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, renderer, "argument #1 expected ConsoleRenderer");
	lua_check(L, Stack<Vector>::Check(L, 2), "argument #2 expected vector");
	lua_check(L, Stack<Size>::Check(L, 3), "argument #3 expected size");
	lua_check(L, Stack<Colour>::Check(L, 4), "argument #4 expected colour");
	lua_check(L, Stack<Colour>::Check(L, 5), "argument #5 expected colour");
	
	auto pos = Stack<Vector>::Get(L, 2);
	auto size = Stack<Size>::Get(L, 3);
	auto fg = Stack<Colour>::Get(L, 4);
	auto bg = Stack<Colour>::Get(L, 5);
	
	renderer->DrawRect(pos, size, fg, bg);
	return 0;
}

int consor_console_renderer_drawstring(lua_State* L)
{
	IConsoleRenderer* renderer = Object<IConsoleRenderer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, renderer, "argument #1 expected ConsoleRenderer");
	lua_check(L, Stack<string>::Check(L, 2), "argument #2 expected string");
	lua_check(L, Stack<Vector>::Check(L, 3), "argument #3 expected vector");
	lua_check(L, Stack<Colour>::Check(L, 4), "argument #4 expected colour");
	lua_check(L, Stack<Colour>::Check(L, 5), "argument #5 expected colour");
	
	auto str = Stack<string>::Get(L, 2);
	auto pos = Stack<Vector>::Get(L, 3);
	auto fg = Stack<Colour>::Get(L, 4);
	auto bg = Stack<Colour>::Get(L, 5);
	
	renderer->DrawString(str, pos, fg, bg);
	return 0;
}

int consor_console_renderer_pushrenderbounds(lua_State* L)
{
	IConsoleRenderer* renderer = Object<IConsoleRenderer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, renderer, "argument #1 expected ConsoleRenderer");
	lua_check(L, Stack<Vector>::Check(L, 2), "argument #2 expected vector");
	lua_check(L, Stack<Size>::Check(L, 3), "argument #3 expected size");
	
	auto pos = Stack<Vector>::Get(L, 2);
	auto size = Stack<Size>::Get(L, 3);
	
	renderer->PushRenderBounds(pos, size);
	return 0;
}

int consor_console_renderer_poprenderbounds(lua_State* L)
{
	IConsoleRenderer* renderer = Object<IConsoleRenderer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, renderer, "argument #1 expected ConsoleRenderer");
		
	renderer->PopRenderBounds();
	return 0;
}

int consor_console_renderer_rendersize(lua_State* L)
{
	IConsoleRenderer* renderer = Object<IConsoleRenderer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, renderer, "argument #1 expected ConsoleRenderer");
	
	Size size = renderer->RenderSize();
	Stack<Size>::Push(L, size);
	return 1;
}

int consor_console_renderer_renderoffset(lua_State* L)
{
	IConsoleRenderer* renderer = Object<IConsoleRenderer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, renderer, "argument #1 expected ConsoleRenderer");
	
	Vector pos = renderer->RenderOffset();
	Stack<Vector>::Push(L, pos);
	return 1;
}

int consor_console_renderer_inrenderbounds(lua_State* L)
{
	IConsoleRenderer* renderer = Object<IConsoleRenderer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, renderer, "argument #1 expected ConsoleRenderer");
	lua_check(L, Stack<Vector>::Check(L, 2), "argument #2 expected vector");
	
	Vector pos = Stack<Vector>::Get(L, 2);
	
	if(Stack<Size>::Check(L, 3))
	{
		// bool InRenderBounds (const Vector &pos, const Consor::Size &size)
		Size size = Stack<Size>::Get(L, 3);
		bool ret = renderer->InRenderBounds(pos, size);
		Stack<bool>::Push(L, ret);
		return 1;
	}
	else
	{
		Vector dir;
		bool ret = renderer->InRenderBounds(pos, &dir);
		
		Stack<bool>::Push(L, ret);
		Stack<Vector>::Push(L, dir);
		return 2;
	}
}

// IInputSystem
int consor_input_inputsystem_ctor(lua_State* L)
{
	int system = Object<PlatformInputSystem>::Make();
	Stack<int>::Push(L, system);
	return 1;
}

int consor_input_inputsystem_dtor(lua_State* L)
{
	int system = Stack<int>::Get(L, 1);
	Object<PlatformInputSystem>::Destroy(system);
	return 0;
}

int consor_input_inputsystem_keywaiting(lua_State* L)
{
	IInputSystem* input = Object<IInputSystem>::Get(Stack<int>::Get(L, 1));
	lua_check(L, input, "argument #1 expected InputSystem");
	
	Stack<bool>::Push(L, input->KeyWaiting());
	return 1;
}

int consor_input_inputsystem_getkeypress(lua_State* L)
{
	IInputSystem* input = Object<IInputSystem>::Get(Stack<int>::Get(L, 1));
	lua_check(L, input, "argument #1 expected InputSystem");
	
	Stack<int>::Push(L, input->GetKeyPress());
	return 1;
}

int consor_input_inputsystem_controldown(lua_State* L)
{
	IInputSystem* input = Object<IInputSystem>::Get(Stack<int>::Get(L, 1));
	lua_check(L, input, "argument #1 expected InputSystem");
	
	Stack<bool>::Push(L, input->ControlDown());
	return 1;
}

int consor_input_inputsystem_shiftdown(lua_State* L)
{
	IInputSystem* input = Object<IInputSystem>::Get(Stack<int>::Get(L, 1));
	lua_check(L, input, "argument #1 expected InputSystem");
	
	Stack<bool>::Push(L, input->ShiftDown());
	return 1;
}

// WindowSystem
int consor_windowsystem_setup(lua_State* L)
{
	IConsoleRenderer* renderer = Object<IConsoleRenderer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, renderer, "argument #1 expected ConsoleRenderer");
	
	IInputSystem* input = Object<IInputSystem>::Get(Stack<int>::Get(L, 2));
	lua_check(L, input, "argument #2 expected InputSystem");
	
	WindowSystem::Setup(renderer, input);
	return 0;
}

int consor_windowsystem_renderer(lua_State* L)
{
	IConsoleRenderer* renderer = &WindowSystem::Renderer();
	int ret = Object<IConsoleRenderer>::FromPointer(renderer);
	Stack<int>::Push(L, ret);
	
	return 1;
}

int consor_windowsystem_draw(lua_State* L)
{
	WindowSystem::Draw();
	return 0;
}

int consor_windowsystem_lock(lua_State* L)
{
	WindowSystem::Lock();
	return 0;
}

int consor_windowsystem_unlock(lua_State* L)
{
	WindowSystem::Unlock();
	return 0;
}

int consor_windowsystem_handleinput(lua_State* L)
{
	lua_check(L, Stack<int>::Check(L, 1), "argument #1 expected number");
	
	IInputSystem* input = Object<IInputSystem>::Get(Stack<int>::Get(L, 2));
	lua_check(L, input, "argument #2 expected InputSystem");
	
	int key = Stack<int>::Get(L, 1);
	WindowSystem::HandleInput((Key)key, *input);
	return 0;
}

int consor_windowsystem_registerwindow(lua_State* L)
{
	Control* ctrl = Object<Control>::Get(Stack<int>::Get(L, 1));
	lua_check(L, ctrl, "argument #1 expected Control");
	lua_check(L, Stack<Vector>::Check(L, 2), "argument #2 expected vector");
	
	Vector pos = Stack<Vector>::Get(L, 2);
	
	WindowSystem::RegisterWindow(*ctrl, pos);
	return 0;
}

int consor_windowsystem_unregisterwindow(lua_State* L)
{
	Control* ctrl = Object<Control>::Get(Stack<int>::Get(L, 1));
	lua_check(L, ctrl, "argument #1 expected Control");
	
	WindowSystem::UnregisterWindow(*ctrl);
	return 0;
}

int consor_windowsystem_registerhotkey(lua_State* L)
{
	Control* ctrl = Object<Control>::Get(Stack<int>::Get(L, 1)); // can be nullptr
	
	lua_check(L, Stack<int>::Check(L, 2), "argument #2 expected number");
	lua_check(L, Stack<bool>::Check(L, 3), "argument #3 expected boolean");
	lua_check(L, Stack<bool>::Check(L, 4), "argument #4 expected boolean");
	lua_check(L, lua_isfunction(L, 5), "argument #5 expected function");
	
	int key = Stack<int>::Get(L, 2);
	bool ctl = Stack<bool>::Get(L, 3);
	bool sft = Stack<bool>::Get(L, 4);
	lua_function_reference<void()> func(L, 5);
	
	WindowSystem::RegisterHotKey(ctrl, (Key)key, ctl, sft, func);
	return 0;
}

int consor_windowsystem_running(lua_State* L)
{
	Stack<bool>::Push(L, WindowSystem::Running());
	return 1;
}

int consor_windowsystem_close(lua_State* L)
{
	WindowSystem::Close();
	return 0;
}

int consor_windowsystem_renderername(lua_State* L)
{
	Stack<string>::Push(L, WindowSystem::RendererName());
	return 1;
}

int consor_windowsystem_rendererversionstring(lua_State* L)
{
	Stack<string>::Push(L, WindowSystem::RendererVersionString());
	return 1;
}

int consor_windowsystem_requestcolour(lua_State* L)
{
	lua_check(L, Stack<Colour>::Check(L, 1), "argument #1 expected colour");
	
	Colour col = Stack<Colour>::Get(L, 1);
	bool make = Stack<bool>::Check(L, 2) ? Stack<bool>::Get(L, 2) : false;
	
	Colour ret = WindowSystem::RequestColour(col, make);
	Stack<Colour>::Push(L, ret);
	return 1;
}

// Util
//int consor_util_hooklog(lua_State* L);

int consor_util_log(lua_State* L)
{
	string msg = Stack<string>::Get(L, 1);
	Util::Log(msg);
	return 0;
}

int consor_util_round(lua_State* L)
{
	double value = Stack<double>::Get(L, 1);
	double prec = Stack<double>::Get(L, 2);

	Stack<double>::Push(L, Util::Round(value, prec));
	return 1;
}

int consor_util_map(lua_State* L)
{
	double inval = Stack<double>::Get(L, 1);
	double inmin = Stack<double>::Get(L, 2);
	double inmax = Stack<double>::Get(L, 3);
	double otmin = Stack<double>::Get(L, 4);
	double otmax = Stack<double>::Get(L, 5);

	Stack<double>::Push(L, Util::Map(inval, inmin, inmax, otmin, otmax));
	return 1;
}


int consor_util_messagebox(lua_State* L)
{
	string msg = Stack<string>::Get(L, 1);
	string title = Stack<string>::Get(L, 2);
	lua_check(L, lua_istable(L, 3), "argument #3 expected table");
	
	list<string> ops;
	
	lua_pushnil(L);
	while(lua_next(L, -2))
	{
		ops.emplace_back(lua_tostring(L, -1));
		lua_pop(L, 1);
	}
	lua_pop(L, 1);
	
	string res = Util::MessageBox(msg, title, ops);
	Stack<string>::Push(L, res);
	return 1;
}

int consor_util_choicelist(lua_State* L)
{
	string msg = Stack<string>::Get(L, 1);
	string title = Stack<string>::Get(L, 2);
	lua_check(L, lua_istable(L, 3), "argument #3 expected table");
	
	list<string> ops;
	
	lua_pushnil(L);
	while(lua_next(L, -2))
	{
		ops.emplace_back(lua_tostring(L, -1));
		lua_pop(L, 1);
	}
	lua_pop(L, 1);
	
	string res = Util::ChoiceList(msg, title, ops);
	Stack<string>::Push(L, res);
	return 1;
}

int consor_util_inputbox(lua_State* L)
{
	string msg = Stack<string>::Get(L, 1);
	string title = Stack<string>::Get(L, 2);
	
	string res = Util::InputBox(msg, title);
	Stack<string>::Push(L, res);
	return 1;
}

int consor_util_formattimespan(lua_State* L)
{
	double t = Stack<double>::Get(L, 1);
	
	string res = Util::FormatTimeSpan(t);
	Stack<string>::Push(L, res);
	return 1;
}

int consor_util_wraptext(lua_State* L)
{
	string str = Stack<string>::Get(L, 1);
	int width = Stack<int>::Get(L, 2);
	
	size_t ow, oh;
	string res = Util::WrapText(str, width, &ow, &oh);
	
	Stack<string>::Push(L, res);
	Stack<int>::Push(L, ow);
	Stack<int>::Push(L, oh);
	return 3;
}

int consor_util_gettime(lua_State* L)
{
	double t = Util::GetTime();
	Stack<double>::Push(L, t);
	return 1;
}

int consor_util_sleep(lua_State* L)
{
	double t = Stack<double>::Get(L, 1);
	Util::Sleep(t);
	return 0;
}

// Control bindings
int consor_control_dtor(lua_State* L)
{
	int renderer = Stack<int>::Get(L, 1);
	Object<Control>::Destroy(renderer);
	return 0;
}

int consor_control_getsize(lua_State* L)
{
	Control* ctrl = Object<Control>::Get(Stack<int>::Get(L, 1));
	lua_check(L, ctrl, "argument #1 expected control");
	
	Stack<Size>::Push(L, ctrl->GetSize());
	return 1;
}

int consor_control_onresize(lua_State* L)
{
	Control* ctrl = Object<Control>::Get(Stack<int>::Get(L, 1));
	lua_check(L, ctrl, "argument #1 expected control");
	lua_check(L, Stack<Size>::Check(L, 2), "argument #2 expected size");
	
	Size sz = Stack<Size>::Get(L, 2);
	ctrl->OnResize(sz);
	return 0;
}

int consor_control_forceresize(lua_State* L)
{
	Control* ctrl = Object<Control>::Get(Stack<int>::Get(L, 1));
	lua_check(L, ctrl, "argument #1 expected control");
	lua_check(L, Stack<Size>::Check(L, 2), "argument #2 expected size");
	
	Size sz = Stack<Size>::Get(L, 2);
	ctrl->ForceResize(sz);
	return 0;
}

int consor_control_draw(lua_State* L)
{
	Control* ctrl = Object<Control>::Get(Stack<int>::Get(L, 1));
	IConsoleRenderer* renderer = Object<IConsoleRenderer>::Get(Stack<int>::Get(L, 2));
	ISkin* skin = Object<ISkin>::Get(Stack<int>::Get(L, 4));
	
	lua_check(L, ctrl, "argument #1 expected control");
	lua_check(L, renderer, "argument #2 expected renderer");
	lua_check(L, Stack<bool>::Check(L, 3), "argument #3 expected boolean");
	lua_check(L, skin, "argument #4 expected skin");
	
	bool focused = Stack<bool>::Get(L, 3);
	ctrl->Draw(*renderer, focused, *skin);
	return 0;
}

int consor_control_handleinput(lua_State* L)
{
	Control* ctrl = Object<Control>::Get(Stack<int>::Get(L, 1));
	IInputSystem* input = Object<IInputSystem>::Get(Stack<int>::Get(L, 3));
	lua_check(L, ctrl, "argument #1 expected control");
	lua_check(L, Stack<int>::Check(L, 2), "argument #2 expected key");
	lua_check(L, input, "argument #3 expected inputsystem");
	
	int key = Stack<int>::Get(L, 2);
	Stack<bool>::Push(L, ctrl->HandleInput((Key)key, *input));
	return 1;	
}

int consor_control_canfocus(lua_State* L)
{
	Control* ctrl = Object<Control>::Get(Stack<int>::Get(L, 1));
	lua_check(L, ctrl, "argument #1 expected control");
	
	Stack<bool>::Push(L, ctrl->CanFocus());
	return 1;
}

//////////////////////////////////////////////////////////////////////////////// Containers

// AlignContainer
int consor_aligncontainer_ctor(lua_State* L)
{
	Control* client = Object<Control>::Get(Stack<int>::Get(L, 1));
	lua_check(L, client, "argument #1 expected control");
	lua_check(L, Stack<int>::Check(L, 2), "argument #2 expected number");
	lua_check(L, Stack<int>::Check(L, 3), "argument #3 expected number");
	
	int axis = Stack<int>::Get(L, 2);
	int align = Stack<int>::Get(L, 3);
	
	int handel = Object<AlignContainer>::Make(*client, (AlignContainer::Axis)axis, (AlignContainer::Align)align);
	Stack<int>::Push(L, handel);
	return 1;
}

int consor_aligncontainer_getsize(lua_State* L)
{
	struct {} not_implimented; throw not_implimented;
}

// BorderContainer
int consor_bordercontainer_ctor(lua_State* L)
{
	Control* client = Object<Control>::Get(Stack<int>::Get(L, 1));
	lua_check(L, client, "argument #1 expected control");
	
	if(lua_gettop(L) == 2) // client, border
	{
		lua_check(L, Stack<int>::Check(L, 2), "argument #2 expected number");
		int border = Stack<int>::Get(L, 2);
		
		int handel = Object<BorderContainer>::Make(*client, border);
		Stack<int>::Push(L, handel);
		return 1;
	}
	else
	{
		lua_check(L, Stack<int>::Check(L, 2), "argument #2 expected number");
		lua_check(L, Stack<int>::Check(L, 3), "argument #3 expected number");
		lua_check(L, Stack<int>::Check(L, 4), "argument #4 expected number");
		lua_check(L, Stack<int>::Check(L, 5), "argument #5 expected number");
		int left   = Stack<int>::Get(L, 2),
		    right  = Stack<int>::Get(L, 3),
		    top    = Stack<int>::Get(L, 4),
		    bottom = Stack<int>::Get(L, 5);
		
		int handel = Object<BorderContainer>::Make(*client, left, right, top, bottom);
		Stack<int>::Push(L, handel);
		return 1;
	}
}

// FlowContainer
int consor_flowcontainer_ctor(lua_State* L)
{
	lua_check(L, Stack<int>::Check(L, 1), "argument #1 expected number");
	lua_check(L, Stack<int>::Check(L, 2), "argument #2 expected number");
	
	int axis = Stack<int>::Get(L, 1);
	int seperation = Stack<int>::Get(L, 2);
	
	int handel = Object<FlowContainer>::Make((FlowContainer::FlowAxis)axis, seperation);
	Stack<int>::Push(L, handel);
	return 1;
}

int consor_flowcontainer_addcontrol(lua_State* L)
{
	FlowContainer* self = Object<FlowContainer>::Get(Stack<int>::Get(L, 1));
	Control* ctrl = Object<Control>::Get(Stack<int>::Get(L, 2));
	lua_check(L, self, "argument #1 expected flowcontainer");
	lua_check(L, ctrl, "argument #1 expected control");
		
	double size = Stack<double>::Get(L, 3);
	self->AddControl(*ctrl, size);
	return 0;
}

// ScrollContainer
int consor_scrollcontainer_ctor(lua_State* L)
{
	Control* client = Object<Control>::Get(Stack<int>::Get(L, 1));
	lua_check(L, client, "argument #1 expected control");
	lua_check(L, Stack<string>::Check(L, 2), "argument #2 expected size");
	
	Size sz = Stack<Size>::Get(L, 2);
	
	int handel = Object<ScrollContainer>::Make(*client, sz);
	Stack<int>::Push(L, handel);
	return 1;
}

int consor_scrollcontainer_scrollleft(lua_State* L)
{
	ScrollContainer* self = Object<ScrollContainer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, self, "argument #1 expected scrollcontainer");
	
	size_t count = Stack<size_t>::Check(L, 2) ? Stack<size_t>::Get(L, 2) : 1;
	Stack<bool>::Push(L, self->ScrollLeft(count));
	return 1;
}

int consor_scrollcontainer_scrollright(lua_State* L)
{
	ScrollContainer* self = Object<ScrollContainer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, self, "argument #1 expected scrollcontainer");
	
	size_t count = Stack<size_t>::Check(L, 2) ? Stack<size_t>::Get(L, 2) : 1;
	Stack<bool>::Push(L, self->ScrollRight(count));
	return 1;
}

int consor_scrollcontainer_scrollup(lua_State* L)
{
	ScrollContainer* self = Object<ScrollContainer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, self, "argument #1 expected scrollcontainer");
	
	size_t count = Stack<size_t>::Check(L, 2) ? Stack<size_t>::Get(L, 2) : 1;
	Stack<bool>::Push(L, self->ScrollUp(count));
	return 1;
}

int consor_scrollcontainer_scrolldown(lua_State* L)
{
	ScrollContainer* self = Object<ScrollContainer>::Get(Stack<int>::Get(L, 1));
	lua_check(L, self, "argument #1 expected scrollcontainer");
	
	size_t count = Stack<size_t>::Check(L, 2) ? Stack<size_t>::Get(L, 2) : 1;
	Stack<bool>::Push(L, self->ScrollDown(count));
	return 1;
}

// WindowContainer
int consor_windowcontainer_ctor(lua_State* L)
{
	Control* client = Object<Control>::Get(Stack<int>::Get(L, 1));
	lua_check(L, client, "argument #1 expected control");
	lua_check(L, Stack<string>::Check(L, 2), "argument #2 expected string");
	
	string title = Stack<string>::Get(L, 2);
	
	int handel = Object<WindowContainer>::Make(*client, title);
	Stack<int>::Push(L, handel);
	return 1;
}

//////////////////////////////////////////////////////////////////////////////// Controls

// Button
int consor_button_ctor(lua_State* L)
{	
	int handel = Object<Button>::Make();
	Stack<int>::Push(L, handel);
	return 1;
}

int consor_button_settext(lua_State* L)
{
	Button* self = Object<Button>::Get(Stack<int>::Get(L, 1));
	lua_check(L, self, "argument #1 expected button");
	lua_check(L, Stack<string>::Check(L, 2), "argument #1 expected string");
	
	string txt = Stack<string>::Get(L, 2);
	self->SetText(txt);
	return 0;
}


#define FUNC(_x_) { #_x_, &_x_ }
static const luaL_Reg R[] =
{
	// Classes
	
	// IConsoleRenderer
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
	FUNC(consor_console_renderer_drawbox),
	FUNC(consor_console_renderer_drawrect),
	FUNC(consor_console_renderer_drawstring),
	FUNC(consor_console_renderer_pushrenderbounds),
	FUNC(consor_console_renderer_poprenderbounds),
	FUNC(consor_console_renderer_rendersize),
	FUNC(consor_console_renderer_renderoffset),
	FUNC(consor_console_renderer_inrenderbounds),
	
	// IInputSystem
	FUNC(consor_input_inputsystem_ctor),
	FUNC(consor_input_inputsystem_dtor),
	FUNC(consor_input_inputsystem_keywaiting),
	FUNC(consor_input_inputsystem_getkeypress),
	FUNC(consor_input_inputsystem_controldown),
	FUNC(consor_input_inputsystem_shiftdown),
	
	// Windowsystem
	FUNC(consor_windowsystem_setup),
	FUNC(consor_windowsystem_renderer),
	FUNC(consor_windowsystem_draw),
	FUNC(consor_windowsystem_lock),
	FUNC(consor_windowsystem_unlock),
	FUNC(consor_windowsystem_handleinput),
	FUNC(consor_windowsystem_registerwindow),
	FUNC(consor_windowsystem_unregisterwindow),
	FUNC(consor_windowsystem_registerhotkey),
	FUNC(consor_windowsystem_running),
	FUNC(consor_windowsystem_close),
	FUNC(consor_windowsystem_renderername),
	FUNC(consor_windowsystem_rendererversionstring),
	FUNC(consor_windowsystem_requestcolour),
	
	// Util
	FUNC(consor_util_log),
	FUNC(consor_util_round),
	FUNC(consor_util_map),
	FUNC(consor_util_messagebox),
	FUNC(consor_util_choicelist),
	FUNC(consor_util_inputbox),
	FUNC(consor_util_formattimespan),
	FUNC(consor_util_wraptext),
	FUNC(consor_util_gettime),
	FUNC(consor_util_sleep),
	
	// Control
	FUNC(consor_control_dtor),
	FUNC(consor_control_getsize),
	FUNC(consor_control_onresize),
	FUNC(consor_control_forceresize),
	FUNC(consor_control_draw),
	FUNC(consor_control_handleinput),
	FUNC(consor_control_canfocus),
	
	// AlignContainer
	FUNC(consor_aligncontainer_ctor),
	FUNC(consor_aligncontainer_getsize),
	
	// BorderContainer
	FUNC(consor_bordercontainer_ctor),
	
	// FlowContainer
	FUNC(consor_flowcontainer_ctor),
	FUNC(consor_flowcontainer_addcontrol),
	
	FUNC(consor_scrollcontainer_ctor),
	FUNC(consor_scrollcontainer_scrollleft),
	FUNC(consor_scrollcontainer_scrollright),
	FUNC(consor_scrollcontainer_scrollup),
	FUNC(consor_scrollcontainer_scrolldown),
	
	// WindowContainer
	FUNC(consor_windowcontainer_ctor),
	
	// Button
	FUNC(consor_button_ctor),
	FUNC(consor_button_settext),
	
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
