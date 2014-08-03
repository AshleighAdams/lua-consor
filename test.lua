#!/usr/bin/env lua

local Consor = require("consor")

local renderer = Consor.Console.ConsoleRenderer()
local input = Consor.Input.InputSystem()

Consor.WindowSystem.Setup(renderer, input)

Consor.WindowSystem.RegisterHotKey(nil, string.byte("`"), false, false, function()
	Consor.Util.Log("This is a hot key!");
end)

while true do
	local name = Consor.Util.InputBox("Please enter your name", "Name")
	local clicked = Consor.Util.MessageBox("Hello, " .. name .. "!", "Hello", {"Again", "Exit"})
	
	if clicked == "Exit" then break end
end

Consor.WindowSystem.Close()
