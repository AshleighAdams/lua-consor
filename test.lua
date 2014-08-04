#!/usr/bin/env lua

local Consor = require("consor")

local renderer = Consor.Console.ConsoleRenderer()
local input = Consor.Input.InputSystem()

Consor.WindowSystem.Setup(renderer, input)

Consor.WindowSystem.RegisterHotKey(nil, string.byte("`"), false, false, function()
	local tmp = {}
	for k,v in pairs(Consor.core) do
		table.insert(tmp, k)
	end
	
	table.sort(tmp)
	
	for k,v in pairs(tmp) do
		Consor.Util.Log("Core function: %s", v);
	end
end)

local btn = Consor.core.consor_button_ctor()
Consor.core.consor_button_settext(btn, "Hello, world")

local window = Consor.core.consor_windowcontainer_ctor(btn, "Hi!")

Consor.core.consor_windowsystem_registerwindow(window, Consor.Vector(-1, -1))

while true do end
Consor.WindowSystem.Close()
