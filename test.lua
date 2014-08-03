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

local renderer = Consor.core.consor_console_renderer_ctor();
print(renderer)
Consor.core.consor_console_renderer_dtor(renderer);