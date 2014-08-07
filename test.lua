#!/usr/bin/env lua

local Consor = require("consor")


local renderer = Consor.Console.ConsoleRenderer()
local input = Consor.Input.InputSystem()

Consor.WindowSystem.Setup(renderer, input)
Consor.WindowSystem.SetSkin("Mono")

local btn = Consor.Button()
local window = Consor.WindowContainer(btn, "Hi!")

btn:SetText("Hello")
btn:OnClick(function()
	btn:SetText("GO 'WAY OR I PUNK YOU A SECOND TIME-A")
	Consor.Util.Sleep(2.5)
	window:Close()
end)

window:Show()

Consor.WindowSystem.Close()


--[[
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

local function ping(host)
	local cmd = string.format("ping -c 1 -n %s", host or "google.com")
	local file = io.popen(cmd)
	local output = file:read("*a") -- read it all
	local time = output:match("([0-9%.]+) ms")
	return tonumber(time)
end

local client = Consor.core.consor_graph_ctor(16+6-2)

Consor.core.consor_graph_setxlabel(client, "X")
Consor.core.consor_graph_setylabel(client, "Y")
Consor.core.consor_graph_addxaxisnotch(client, "T", 0.5)

Consor.core.consor_graph_onclick(client, function(x, val)
	--Consor.Util.MessageBox(string.format("%d ms", val), "T", {"Close"})
end)

local window = Consor.core.consor_windowcontainer_ctor(client, "Hi!")

Consor.core.consor_windowsystem_registerwindow(client, Consor.Vector(0, 1))

for i = 1, 78 do
	Consor.core.consor_graph_addbar(client, ping() / 50)
	Consor.core.consor_windowsystem_draw()
	Consor.Util.Sleep(0.2)
end

Consor.Util.MessageBox("Awesome!", "Done", {"OK"})
Consor.core.consor_windowsystem_unregisterwindow(client)
--Consor.core.consor_windowcontainer_show(window)
]]
