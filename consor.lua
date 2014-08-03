local core = require("consor.core")
return {
	core = core,
	Vector = function(x,y) return {X = x or 0, Y = y or 0} end,
	Size = function(w,h) return {Width = w or 0, Height = h or 0} end,
	Colour = function(r,g,b,a) return {R = r or 0, G = g or 0, B = b or 0, A = a or 1} end,
	
	Console = {
		ConsoleRenderer = (function()
			local meta = {}
			meta.__index = meta
			
			function meta:RendererName(...)
				return core.consor_console_renderer_renderername(self.handle, ...)
			end
			function meta:VersionString(...)
				return core.consor_console_renderer_versionstring(self.handle, ...)
			end
			function meta:FlushToScreen(...)
				return core.consor_console_renderer_flushtoscreen(self.handle, ...)
			end
			function meta:GetCharInformation(...) -- this isn't implimented y et
				return core.consor_console_renderer_getcharinformation(self.handle, ...)
			end
			function meta:GetSize(...)
				return core.consor_console_renderer_getsize(self.handle, ...)
			end
			function meta:GetColours(...) -- not imp
				return core.consor_console_renderer_getcolours(self.handle, ...)
			end
			function meta:SetColours(...) -- not imp
				return core.consor_console_renderer_setcolours(self.handle, ...)
			end
			function meta:ResetColours(...)
				return core.consor_console_renderer_resetcolours(self.handle, ...)
			end
			function meta:RequestColour(...)
				return core.consor_console_renderer_requestcolour(self.handle, ...)
			end
			function meta:FlushRequestedColours(...)
				return core.consor_console_renderer_flushrequestedcolours(self.handle, ...)
			end
			function meta:SetTitle(...)
				return core.consor_console_renderer_settitle(self.handle, ...)
			end
			function meta:Clear(...)
				return core.consor_console_renderer_clear(self.handle, ...)
			end
			function meta:DrawBox(...)
				return core.consor_console_renderer_drawbox(self.handle, ...)
			end
			function meta:DrawRect(...)
				return core.consor_console_renderer_drawrect(self.handle, ...)
			end
			function meta:DrawString(...)
				return core.consor_console_renderer_drawstring(self.handle, ...)
			end
			function meta:PushRenderBounds(...)
				return core.consor_console_renderer_pushrenderbounds(self.handle, ...)
			end
			function meta:PopRenderBounds(...)
				return core.consor_console_renderer_poprenderbounds(self.handle, ...)
			end
			function meta:RenderSize(...)
				return core.consor_console_renderer_rendersize(self.handle, ...)
			end
			function meta:RenderOffset(...)
				return core.consor_console_renderer_renderoffset(self.handle, ...)
			end
			function meta:InRenderBounds(...)
				return core.consor_console_renderer_inrenderbounds(self.handle, ...)
			end
			
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
			
			function meta:KeyWaiting(...)
				return core.consor_input_inputsystem_keywaiting(self.handle, ...)
			end
			function meta:GetKeyPress(...)
				return core.consor_input_inputsystem_getkeypress(self.handle, ...)
			end
			function meta:ControlDown(...)
				return core.consor_input_inputsystem_controldown(self.handle, ...)
			end
			function meta:ShiftDown(...)
				return core.consor_input_inputsystem_shiftfown(self.handle, ...)
			end
			
			function meta:__gc()
				core.consor_input_inputsystem_dtor(self.handle)
			end
			return function()
				local ret = {handle = core.consor_input_inputsystem_ctor()}
				return setmetatable(ret, meta)
			end
		end)()
	}
}


--[[
function meta:XXXX(...)
	return core.consor_namespace_class_xxxx(self.handle, ...)
end
]]
