local core = require("consor.core")


local ControlMetas = {}

local function DeriveCoreControl(name, base, funcs)
	ControlMetas[base] = ControlMetas[base] or {}
	base = ControlMetas[base]
	
	local lowername = name:lower()
	
	ControlMetas[name] = ControlMetas[name] or {} -- so if a control derives from us, but is not yet loaded, we use
	local meta = ControlMetas[name]               -- the existing table created for us (from the start of this function)
	meta._ctor = core["consor_" .. lowername .. "_ctor"]
	meta._dtor = core.consor_control_dtor -- this should call any implimentations from the derived virtual dtor
	meta._base = base
	meta._name = name
	meta.__index = function(self, name)
		local basefunc = rawget(meta._base, name)
		
		if rawget(meta, name) == nil and basefunc ~= nil then
			return basefunc
		else
			return meta[name]
		end
	end
	
	meta.__tostring = function(self)
		return string.format("%s: %d", meta._name, self.handle)
	end
	
	if dtor then -- if a dtor is passed, set it up
		meta.__gc = function()
			meta._dtor()
		end
	end
	
	for funcname_, corefunc_ in pairs(funcs) do
		local funcname, corefunc = funcname_, corefunc_ -- make copies
		
		if type(corefunc) == "string" then
			if corefunc == "auto" then
				local cfn = string.format("consor_%s_%s", lowername, funcname:lower())
				corefunc = core[cfn] or error(cfn .. " could not be found!")
			else
				error("unknown " .. corefunc)
			end
		end
		
		meta[funcname] = function(self, ...)
			return corefunc(self.handle, ...)
		end
	end
		
	if not meta._ctor then -- this control can't be constructed
		core.consor_util_log(name .. " no ctor")
		return nil
	end
	
	return function(...)
		local ctrl = {
			handle = meta._ctor(...)
		}
		return setmetatable(ctrl, meta)
	end
end

local Consor; Consor = {
	core = core,
	Vector = function(x,y) return {X = x or 0, Y = y or 0} end,
	Size = function(w,h) return {Width = w or 0, Height = h or 0} end,
	Colour = function(r,g,b,a) return {R = r or 0, G = g or 0, B = b or 0, A = a or 1} end,
	
	-- The base control
	Control = DeriveCoreControl("Control", "", {
		GetSize = "auto",
		OnResize = "auto",
		ForceResize = "auto",
		Draw = "auto",
		HandleInput = "auto",
		CanFocus = "auto"
	}),
	
	-- Containers
	AlignContainer = DeriveCoreControl("AlignContainer", "Control", {
		-- no custom methods...
	}),
	
	BorderContainer = DeriveCoreControl("BorderContainer", "Control", {
		-- no custom methods...
	}),
	
	FlowContainer = DeriveCoreControl("FlowContainer", "Control", {
		AddControl = "auto",
	}),
	FlowAxis = {
		Horizontal = 0,
		Vertical = 1
	},
	
	ScrollContainer = DeriveCoreControl("ScrollContainer", "Control", {
		ScrollLeft = "auto",
		ScrollRight = "auto",
		ScrollUp = "auto",
		ScrollDown = "auto"
	}),
	
	WindowContainer = DeriveCoreControl("WindowContainer", "Control", {
		-- SetTitle = core.consor_windowcontainer_settitle
		Show = "auto",
		Close = "auto"
	}),
	
	-- Controls
	Button = DeriveCoreControl("Button", "Control", {
		SetText = "auto",
		OnClick = "auto"
	}),
	
	CheckBox = DeriveCoreControl("CheckBox", "Control", {
		SetText = "auto",
		Checked = "auto",
		SetChecked = "auto",
		OnValueChanged = "auto",
	}),
	
	HorizontalScrollbar = DeriveCoreControl("HorizontalScrollbar", "Control", {
		SetPercent = "auto",
		GetPercent = "auto",
		SetChangeSize = "auto",
		SetScrollRegionSize = "auto",
		GetBarSize = "auto",
		OnValueChanged = "auto"
	}),
	
	Label = DeriveCoreControl("Label", "Control", {
		SetText = "auto"
	}),
	
	PasswordBox = DeriveCoreControl("PasswordBox", "TextBox", {
		GetPassword = "auto",
		SetPasswordChar = "auto"
	}),
	--[[
	ProgressBar = DeriveCoreControl("ProgressBar", "Control", {
		SetPercent = "auto",
		GetPercent = "auto"
	}),
	
	RadioBox = DeriveCoreControl("RadioBox", "Control", {
		AddChoice = "auto",
		GetChoice = "auto",
		OnValueChanged = "auto",
	}),
	
	TextBox = DeriveCoreControl("TextBox", "Control", {
		SetText = "auto",
		GetText = "auto",
		OnValueChanged = "auto"
	}),
	
	ProgressBar = DeriveCoreControl("VerticalProgressBar", "ProgressBar", {
		SetGraphMode = "auto",
		ColourOveride = "auto",
		OnClick = "auto"
	}),
	
	VerticalScrollbar = DeriveCoreControl("VerticalScrollbar", "Control", {
		SetPercent = "auto",
		GetPercent = "auto",
		SetChangeSize = "auto",
		SetScrollRegionSize = "auto",
		GetBarSize = "auto",
		OnValueChanged = "auto"
	}),
	]]
	
	--[[
	TYPE = DeriveCoreControl("TYPE", "Control", {
		FUNC = "auto",
	}),
	]]
	
	WindowSystem = setmetatable({
		Setup                    = function(renderer, input)                    return core.consor_windowsystem_setup                   (renderer, input) end,
		Renderer                 = function()                                   return core.consor_windowsystem_renderer                () end,
		Draw                     = function()                                   return core.consor_windowsystem_draw                    () end,
		Lock                     = function()                                   return core.consor_windowsystem_lock                    () end,
		Unlock                   = function()                                   return core.consor_windowsystem_unlock                  () end,
		HandleInput              = function(key, input)                         return core.consor_windowsystem_handleinput             (key, input) end,
		RegisterWindow           = function(ctrl, pos)                          return core.consor_windowsystem_registerwindow          (ctrl, pos) end,
		UnregisterWindow         = function(ctrl)                               return core.consor_windowsystem_unregisterwindow        (ctrl) end,
		RegisterHotKey           = function(control, key, ctrl, shift, func)    return core.consor_windowsystem_registerhotkey          ((control and control or 0), key, ctrl, shift, func) end,
		Running                  = function()                                   return core.consor_windowsystem_running                 () end,
		Close                    = function()                                   return core.consor_windowsystem_close                   () end,
		RendererName             = function()                                   return core.consor_windowsystem_renderername            () end,
		RendeererVersionString   = function()                                   return core.consor_windowsystem_rendererversionstring   () end,
		RequestColour            = function(col, make)                          return core.consor_windowsystem_requestcolour           (col, make) end,
		SetSkin                  = function(skin)                               return core.consor_windowsystem_setskin                 (skin) end,
		},
		{
			__gc = function()
				core.consor_windowsystem_close()
			end
		}
	),
	
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
			
			function meta:RendererName(...)             return core.consor_console_renderer_renderername            (self, ...) end
			function meta:VersionString(...)            return core.consor_console_renderer_versionstring           (self, ...) end
			function meta:FlushToScreen(...)            return core.consor_console_renderer_flushtoscreen           (self, ...) end
			function meta:GetCharInformation(...)       return core.consor_console_renderer_getcharinformation      (self, ...) end
			function meta:GetSize(...)                  return core.consor_console_renderer_getsize                 (self, ...) end
			function meta:GetColours(...) --[[not imp]] return core.consor_console_renderer_getcolours              (self, ...) end
			function meta:SetColours(...) --[[not imp]]	return core.consor_console_renderer_setcolours              (self, ...) end
			function meta:ResetColours(...)             return core.consor_console_renderer_resetcolours            (self, ...) end
			function meta:RequestColour(...)            return core.consor_console_renderer_requestcolour           (self, ...) end
			function meta:FlushRequestedColours(...)    return core.consor_console_renderer_flushrequestedcolours   (self, ...) end
			function meta:SetTitle(...)                 return core.consor_console_renderer_settitle                (self, ...) end
			function meta:Clear(...)                    return core.consor_console_renderer_clear                   (self, ...) end
			function meta:DrawBox(...)                  return core.consor_console_renderer_drawbox                 (self, ...) end
			function meta:DrawRect(...)                 return core.consor_console_renderer_drawrect                (self, ...) end
			function meta:DrawString(...)               return core.consor_console_renderer_drawstring              (self, ...) end
			function meta:PushRenderBounds(...)         return core.consor_console_renderer_pushrenderbounds        (self, ...) end
			function meta:PopRenderBounds(...)          return core.consor_console_renderer_poprenderbounds         (self, ...) end
			function meta:RenderSize(...)               return core.consor_console_renderer_rendersize              (self, ...) end
			function meta:RenderOffset(...)             return core.consor_console_renderer_renderoffset            (self, ...) end
			function meta:InRenderBounds(...)           return core.consor_console_renderer_inrenderbounds          (self, ...) end
			
			function meta:__gc()
				core.consor_console_renderer_dtor(self)
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
			
			function meta:KeyWaiting(...)   return core.consor_input_inputsystem_keywaiting    (self, ...) end
			function meta:GetKeyPress(...)  return core.consor_input_inputsystem_getkeypress   (self, ...) end
			function meta:ControlDown(...)  return core.consor_input_inputsystem_controldown   (self, ...) end
			function meta:ShiftDown(...)    return core.consor_input_inputsystem_shiftfown     (self, ...) end
			
			function meta:__gc()
				core.consor_input_inputsystem_dtor(self)
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

return Consor
--[[
function meta:XXXX(...)
	return core.consor_namespace_class_xxxx(self, ...)
end
]]
