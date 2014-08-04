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

while true do
	local t = Consor.Util.GetTime()
	local tstr = Consor.Util.FormatTimeSpan(t)
	Consor.Util.MessageBox("You've been running this program for " .. tstr, "Running", {"OK"})
end

Consor.WindowSystem.Close()
