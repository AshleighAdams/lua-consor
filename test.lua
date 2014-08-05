#!/usr/bin/env lua
local Consor = require("consor")

print = function(...)
	local args = {...}
	for k, v in pairs(args) do
		args[k] = tostring(args[k])
	end
	Consor.Util.Log(table.concat(args, "\t"))
end

function printtable(tbl, done, depth)
	done = done or {}
	depth = depth or 0
	if done[tbl] then return else done[tbl] = true end
	local tabs = string.rep("\t", depth)

	for k,v in pairs(tbl) do
		if type(v) == "table" then
			print(tabs .. tostring(k) .. ":")
			printtable(v, done, depth + 1)
		else
			print(tabs .. tostring(k), v)
		end
	end
end

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

Consor.core.consor_button_click(btn, function()
	Consor.core.consor_windowsystem_unregisterwindow(btn)
end)

local window = Consor.core.consor_windowcontainer_ctor(btn, "Hi!")

Consor.core.consor_windowsystem_registerwindow(window, Consor.Vector(-1, -1))

Consor.WindowSystem.Close()
