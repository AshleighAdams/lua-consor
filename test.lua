#!/usr/bin/env lua

local Consor = require("consor")




local window, consolewindow

----------------------------- Build the console

local lines_flow = Consor.FlowContainer(Consor.FlowAxis.Vertical, 0)
local scroll = Consor.ScrollContainer(lines_flow, Consor.Size(70, 15))
consolewindow = Consor.WindowContainer(scroll, "Console")

Consor.WindowSystem.RegisterHotKey(nil, string.byte("`"), false, false, function()
	if consolewindow.open then
		Consor.WindowSystem.UnregisterWindow(consolewindow)
		consolewindow.open = false
	else
		Consor.WindowSystem.RegisterWindow(consolewindow, Consor.Vector(2, 2))
		consolewindow.open = true
	end
end)

Consor.Util.HookLog(function(str)
	local lbl = Consor.Label()
	lbl:SetText(str)
	lines_flow:AddControl(lbl)
	
	while scroll:ScrollDown() do end
end)

---------------------------- Setup the system

local renderer = Consor.Console.ConsoleRenderer()
local input = Consor.Input.InputSystem()

Consor.WindowSystem.Setup(renderer, input)
Consor.WindowSystem.SetSkin("Mono")

---------------------------- Build theee test window

local button = Consor.Button()
button:SetText("Exit")
button:OnClick(function()
	window:Close()
end)


local themes = {
	Default = "Default",
	Orange = "Hacker",
	Mono = "Mono"
}
local themebtn = Consor.Button()
themebtn:SetText("Change Theme")
themebtn:OnClick(function()
	local ops = {}
	for k,v in pairs(themes) do
		table.insert(ops, k)
	end
	table.sort(ops)
	
	local theme = themes[Consor.Util.ChoiceList("Select a theme", "Theme", ops)]
	if theme ~= nil then
		Consor.WindowSystem.SetSkin(theme)
	end
end)

local checkbox = Consor.CheckBox()
checkbox:SetText("Check me")
checkbox:OnValueChanged(function(val)
	Consor.Util.Log("checkbox = %s", val)
end)

local label = Consor.Label()
label:SetText("I'm a label!")

local passbox = Consor.PasswordBox()
passbox:OnValueChanged(function(val)
	Consor.Util.Log("passbox = %s", val)
end)

local progress = Consor.ProgressBar()
progress:SetPercent(0.25)

local radiobox = Consor.RadioBox()
radiobox:AddChoice("First")
radiobox:AddChoice("Second")
radiobox:OnValueChanged(function(val)
	Consor.Util.Log("radiobox = %s", val)
end)

local textbox = Consor.TextBox()
textbox:OnValueChanged(function(val)
	Consor.Util.Log("textbox = %s", val)
end)

local vprogress = Consor.VerticalProgressBar()
vprogress:SetPercent(0.25)

local flow = Consor.FlowContainer(Consor.FlowAxis.Vertical, 0)
flow:AddControl(button)
flow:AddControl(themebtn)
flow:AddControl(checkbox)
flow:AddControl(label)
flow:AddControl(passbox)
flow:AddControl(progress)
flow:AddControl(radiobox)
flow:AddControl(textbox)
flow:AddControl(vprogress)

window = Consor.WindowContainer(flow, "Test")
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
