#!/usr/bin/env lua
-- stole this HSV thing
function HSV(h, s, l, a)
  h = h / 360
  local r, g, b

  if s == 0 then
    r, g, b = l, l, l -- achromatic
  else
    function hue2rgb(p, q, t)
      if t < 0   then t = t + 1 end
      if t > 1   then t = t - 1 end
      if t < 1/6 then return p + (q - p) * 6 * t end
      if t < 1/2 then return q end
      if t < 2/3 then return p + (q - p) * (2/3 - t) * 6 end
      return p
    end

    local q
    if l < 0.5 then q = l * (1 + s) else q = l + s - l * s end
    local p = 2 * l - q

    r = hue2rgb(p, q, h + 1/3)
    g = hue2rgb(p, q, h)
    b = hue2rgb(p, q, h - 1/3)
  end

  return r, g, b, a
end



local Consor = require("consor")

local host
local ret_queue = {}
local cur_seq = 1

local function ping()
	local cmd = string.format("ping -i 0.2 -w 1 -n %s", host)
	local pingproc = io.popen(cmd)
	
	local output = pingproc:read("*a") -- read it all
	local time, jitter = output:match("= [0-9%.]+/([0-9%.]+)/[0-9%.]+/([0-9%.]+)")
	
	if not time or not jitter then return -1, 0 end
	return tonumber(time), tonumber(jitter)
end

local renderer = Consor.Console.ConsoleRenderer()
local input = Consor.Input.InputSystem()

Consor.WindowSystem.Setup(renderer, input)
Consor.WindowSystem.SetSkin("Mono")

local sz = renderer:GetSize()
host = Consor.Util.InputBox("Target", "Ping")

local flow_jitter = Consor.FlowContainer(Consor.FlowAxis.Horizontal, 0)
local flow_ping = Consor.FlowContainer(Consor.FlowAxis.Horizontal, 0)

local flow_ping_x = Consor.FlowContainer(Consor.FlowAxis.Vertical, 0)
local flow_jitter_x = Consor.FlowContainer(Consor.FlowAxis.Vertical, 0)

flow_ping:AddControl(flow_ping_x)
flow_jitter:AddControl(flow_jitter_x)

flow_ping.labels = {}
flow_jitter.labels = {}

for i = 1, sz.Height / 2 - 2 do
	local perc = Consor.Util.Map(i, 1, sz.Height / 2 - 2, 1, 0) -- remap to percent
	
	local lbl_ping = Consor.Label()
	local lbl_jitter = Consor.Label()
	
	if i % 2 == 0 then
		lbl_ping:SetText(string.format("%d%%", perc * 100))
		lbl_jitter:SetText(string.format("%d%%", perc * 100))
	
		flow_ping.labels[perc] = lbl_ping
		flow_jitter.labels[perc] = lbl_jitter
	else
		lbl_ping:SetText(string.format("   ", perc * 100))
		lbl_jitter:SetText(string.format("   ", perc * 100))
	end
	
	flow_ping_x:AddControl(lbl_ping)
	flow_jitter_x:AddControl(lbl_jitter)
end

local flow = Consor.FlowContainer(Consor.FlowAxis.Vertical, 0)

local jitter_lbl = Consor.Label()
local align_lbl = Consor.AlignContainer(jitter_lbl, 0, 2) -- horizontal, vertical

jitter_lbl:SetText("Jitter")

flow:AddControl(flow_ping)
flow:AddControl(align_lbl)
flow:AddControl(flow_jitter)

local window = Consor.WindowContainer(flow, "Ping")

local graphs_ping = {}
local graphs_jitter = {}

local colours = {}
local col_prec = 1 / 100 -- 100 rainbow colours
function perc_to_col(v)
	v = math.min(v, 1)
	v = Consor.Util.Round(v, col_prec)
	
	local col = colours[v]
	if col then return col end
	
	local hue = Consor.Util.Map(v, 0, 1, 120, 0) -- 120 = green, 0 = red
	local r,g,b = HSV(hue, 1, 0.5)
		
	colours[v] = Consor.WindowSystem.RequestColour(Consor.Colour(r, g, b), true)
	return colours[v]
end

do
	Consor.WindowSystem.Lock()
	for i = 0, 1, col_prec do
		perc_to_col(i)
	end
	Consor.WindowSystem.Unlock()
end

for i = 1, sz.Width - (2 + 4) do
	local vp = Consor.VerticalProgressBar()
	vp:ForceResize(Consor.Size(1, sz.Height / 2 - 2))
	vp:SetPercent(0)
	table.insert(graphs_ping, vp)
	flow_ping:AddControl(vp)
	
	local vp2 = Consor.VerticalProgressBar()
	vp2:ForceResize(Consor.Size(1, sz.Height / 2 - 2))
	vp2:SetPercent(0)
	table.insert(graphs_jitter, vp2)
	flow_jitter:AddControl(vp2)
end


Consor.WindowSystem.RegisterWindow(window, Consor.Vector(0,0))

local results = {}
for i = 1, #graphs_ping do table.insert(results, {time = 0, jitter = 0}) end
while true do
	local loss = renderer:RequestColour(Consor.Colour(1, 0, 0), true)
	local white = renderer:RequestColour(Consor.Colour(1.0, 1.0, 1.0), true)
	
	local t, j = ping(host)
	
	table.insert(results, {
		time = t,
		jitter = j
	})
	
	while #results > #graphs_ping do table.remove(results, 1) end
	
	local max_ping = 40
	local max_jitter = 20
	
	local colmax_time = 100 -- this is bad time
	local colmax_jitter = 20 -- this is bad jitter
	
	for k,v in pairs(results) do
		if v.time > max_ping then max_ping = v.time end
		if v.jitter > max_jitter then max_jitter = v.jitter end
	end

	for perc, lbl in pairs(flow_ping.labels) do
		local txt = perc == 0 and "0ms" or string.format("%d", Consor.Util.Map(perc, 0, 1, 0, max_ping))
		lbl:SetText(txt)
	end
	
	for perc, lbl in pairs(flow_jitter.labels) do
		local txt = perc == 0 and "0ms" or string.format("%d", Consor.Util.Map(perc, 0, 1, 0, max_jitter))
		lbl:SetText(txt)
	end

	for k,v in pairs(results) do
		if v == -1 then
			graphs_ping[k]:SetPercent(0)
			graphs_ping[k]:ColourOveride(loss)
		else
			graphs_ping[k]:SetPercent(v.time / max_ping)
			graphs_ping[k]:ColourOveride(perc_to_col(v.time / colmax_time))
			
			graphs_jitter[k]:SetPercent(v.jitter / max_jitter)
			graphs_jitter[k]:ColourOveride(perc_to_col(v.jitter / colmax_jitter))
		end
	end
	
	Consor.WindowSystem.Draw()
	--Consor.Util.Sleep(1)
end

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
