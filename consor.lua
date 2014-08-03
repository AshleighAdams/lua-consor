local core = require("consor.core")

return {
	core = core,
	Vector = function(x,y) return {X = x or 0, Y = y or 0} end,
	Size = function(w,h) return {Width = w or 0, Height = h or 0} end,
	Colour = function(r,g,b,a) return {R = r or 0, G = g or 0, B = b or 0, A = a or 1} end,
}
