package = "consor"
version = "scm-0"
source = {
	 url = "git://github.com/KateAdams/lua-consor.git"
}
description = {
	 summary = "Lua bindings to Consor",
	 homepage = "https://github.com/KateAdams/lua-consor",
	 license = "GNU LGPL v3"
}
dependencies = {
	 "lua >= 5.2"
}
build = {
	type = "make",
	install_pass = false,
	install = {
		lib = {
			["consor.core"] = "consor/core.so",
			["consor"] = "consor.lua"
		}
	}
}
