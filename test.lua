#!/usr/bin/env lua

local Consor = require("consor")

function printtable(tbl, done, depth)
	done = done or {}
	depth = depth or 0
	
	if done[tbl] then return end
	done[tbl] = true
	
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

printtable(Consor)

--[[ -- This here is the same as below, but without using the Lua class wrapper
local renderer = Consor.core.consor_console_renderer_ctor()
local input = Consor.core.consor_input_inputsystem_ctor()

local bg = Consor.core.consor_console_renderer_requestcolour(renderer, Consor.Colour(0, 0, 0), true)
local fg = Consor.core.consor_console_renderer_requestcolour(renderer, Consor.Colour(1.0, 0.5, 0), true)

--- ...
]]

local renderer = Consor.Console.ConsoleRenderer()
local input = Consor.Input.InputSystem()

local bg = renderer:RequestColour(Consor.Colour(0, 0, 0), true)
local fg = renderer:RequestColour(Consor.Colour(1.0, 0.5, 0), true)

renderer:DrawString(renderer:RendererName(), Consor.Vector(1, 1), fg, bg)
renderer:DrawString(renderer:VersionString(), Consor.Vector(1, 2), fg, bg)

renderer:FlushToScreen()
os.execute("sleep 2")
