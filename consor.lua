local core = require("consor.core")
return {
	core = core,
	Vector = function(x,y) return {X = x or 0, Y = y or 0} end,
	Size = function(w,h) return {Width = w or 0, Height = h or 0} end,
	Colour = function(r,g,b,a) return {R = r or 0, G = g or 0, B = b or 0, A = a or 1} end,
	
	WindowSystem = {
		Setup                    = function(renderer, input)                    return core.consor_windowsystem_setup                   (renderer.handle, input.handle) end,
		Renderer                 = function()                                   return core.consor_windowsystem_renderer                () end,
		Draw                     = function()                                   return core.consor_windowsystem_draw                    () end,
		Lock                     = function()                                   return core.consor_windowsystem_lock                    () end,
		Unlock                   = function()                                   return core.consor_windowsystem_unlock                  () end,
		HandleInput              = function(key, input)                         return core.consor_windowsystem_handleinput             (key, input.handle) end,
		RegisterWindow           = function(ctrl, pos)                          return core.consor_windowsystem_registerwindow          (ctrl.handle, pos) end,
		UnregisterWindow         = function(ctrl)                               return core.consor_windowsystem_unregisterwindow        (ctrl.handle) end,
		RegisterHotKey           = function(control, key, ctrl, shift, func)    return core.consor_windowsystem_registerhotkey          ((control and control.handle or 0), key, ctrl, shift, func) end,
		Running                  = function()                                   return core.consor_windowsystem_running                 () end,
		Close                    = function()                                   return core.consor_windowsystem_close                   () end,
		RendererName             = function()                                   return core.consor_windowsystem_renderername            () end,
		RendeererVersionString   = function()                                   return core.consor_windowsystem_rendererversionstring   () end,
		RequestColour            = function(col, make)                          return core.consor_windowsystem_requestcolour           (col, make) end,
	},
	
	Util = {
		Log = function(str, ...)
			str = str:format(...)
			core.consor_util_log(str)
		end,
		Round          = core.consor_util_round,
		Map            = core.consor_util_map,
		MessageBox     = core.consor_util_messagebox,
		ChoiceList     = core.consor_util_choicelist,
		InputBox       = core.consor_util_inputbox,
		FormatTimeSpan = core.consor_util_formattimespan,
		WrapText       = core.consor_util_wraptext,
		GetTime        = core.consor_util_gettime,
		Sleep          = core.consor_util_sleep,
	},
	
	Console = {
		ConsoleRenderer = (function()
			local meta = {}
			meta.__index = meta
			
			function meta:RendererName(...)             return core.consor_console_renderer_renderername            (self.handle, ...) end
			function meta:VersionString(...)            return core.consor_console_renderer_versionstring           (self.handle, ...) end
			function meta:FlushToScreen(...)            return core.consor_console_renderer_flushtoscreen           (self.handle, ...) end
			function meta:GetCharInformation(...)       return core.consor_console_renderer_getcharinformation      (self.handle, ...) end
			function meta:GetSize(...)                  return core.consor_console_renderer_getsize                 (self.handle, ...) end
			function meta:GetColours(...) --[[not imp]] return core.consor_console_renderer_getcolours              (self.handle, ...) end
			function meta:SetColours(...) --[[not imp]]	return core.consor_console_renderer_setcolours              (self.handle, ...) end
			function meta:ResetColours(...)             return core.consor_console_renderer_resetcolours            (self.handle, ...) end
			function meta:RequestColour(...)            return core.consor_console_renderer_requestcolour           (self.handle, ...) end
			function meta:FlushRequestedColours(...)    return core.consor_console_renderer_flushrequestedcolours   (self.handle, ...) end
			function meta:SetTitle(...)                 return core.consor_console_renderer_settitle                (self.handle, ...) end
			function meta:Clear(...)                    return core.consor_console_renderer_clear                   (self.handle, ...) end
			function meta:DrawBox(...)                  return core.consor_console_renderer_drawbox                 (self.handle, ...) end
			function meta:DrawRect(...)                 return core.consor_console_renderer_drawrect                (self.handle, ...) end
			function meta:DrawString(...)               return core.consor_console_renderer_drawstring              (self.handle, ...) end
			function meta:PushRenderBounds(...)         return core.consor_console_renderer_pushrenderbounds        (self.handle, ...) end
			function meta:PopRenderBounds(...)          return core.consor_console_renderer_poprenderbounds         (self.handle, ...) end
			function meta:RenderSize(...)               return core.consor_console_renderer_rendersize              (self.handle, ...) end
			function meta:RenderOffset(...)             return core.consor_console_renderer_renderoffset            (self.handle, ...) end
			function meta:InRenderBounds(...)           return core.consor_console_renderer_inrenderbounds          (self.handle, ...) end
			
			function meta:__gc()
				core.consor_console_renderer_dtor(self.handle)
			end
			return function()
				local ret = {handle = core.consor_console_renderer_ctor()}
				return setmetatable(ret, meta)
			end
		end)()
	},
	Input = {
		InputSystem = (function()
			local meta = {}
			meta.__index = meta
			
			function meta:KeyWaiting(...)   return core.consor_input_inputsystem_keywaiting    (self.handle, ...) end
			function meta:GetKeyPress(...)  return core.consor_input_inputsystem_getkeypress   (self.handle, ...) end
			function meta:ControlDown(...)  return core.consor_input_inputsystem_controldown   (self.handle, ...) end
			function meta:ShiftDown(...)    return core.consor_input_inputsystem_shiftfown     (self.handle, ...) end
			
			function meta:__gc()
				core.consor_input_inputsystem_dtor(self.handle)
			end
			return function()
				local ret = {handle = core.consor_input_inputsystem_ctor()}
				return setmetatable(ret, meta)
			end
		end)(),
		Key = {
			Escape  = 0, Enter = 1, Space = 32, Backspace = '\b', Tab = '\t', Insert = 2, Home = 3, PageUp = 4, Delete = 5, End = 6, PageDown = 7,
			Up = 14, Down = 15, Left = 16, Right = 17, F1 = 18, F2 = 19, F3 = 20, F4 = 21, F5 = 22, F6 = 23, F7 = 24, F8 = 25, F9 = 26, F10 = 27,
			F11 = 28, F12 = 29, NumpadDelete = 30, Numpad0 = 31, Numpad1 = 127, Numpad2 = 128, Numpad3 = 129, Numpad4 = 130, Numpad5 = 131,
			Numpad6 = 132, Numpad7 = 133, Numpad8 = 134, Numpad9 = 135,
			
			A = 97, B = 98, C = 99, D = 100, E = 101, F = 102, G = 103, H = 104, I = 105, J = 106, K = 107, L = 108, M = 109, N = 110, O = 111,
			P = 112, Q = 113, R = 114, S = 115, T = 116, U = 117, V = 118, W = 119, X = 120, Y = 121, Z = 122,
		}
	}
}


--[[
function meta:XXXX(...)
	return core.consor_namespace_class_xxxx(self.handle, ...)
end
]]
