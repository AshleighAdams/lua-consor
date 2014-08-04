#!/usr/bin/env lua

local Consor = require("consor")

local renderer = Consor.Console.ConsoleRenderer()
local input = Consor.Input.InputSystem()

Consor.WindowSystem.Setup(renderer, input)

Consor.WindowSystem.RegisterHotKey(nil, string.byte("`"), false, false, function()
	Consor.Util.Log("This is a hot key!");
end)

while true do
	local t = Consor.Util.GetTime()
	local tstr = Consor.Util.FormatTimeSpan(t)
	Consor.Util.MessageBox("You've been running this program for " .. tstr, "Running", {"OK"})
end

Consor.WindowSystem.Close()
