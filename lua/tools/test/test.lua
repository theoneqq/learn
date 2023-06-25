local tool = require 'MemoryReferenceInfo'
local _M = {}

collectgarbage("collect")
--tool.m_cMethods.DumpMemorySnapshot("./", "before", -1)

local x = {}
_M.func = function()
	x.k = "set"
	print("hello,world!\t", x.k)
end

_M.t = {
	a = 10,
	f = _M.func,
	tab = x,
}

_M.t.f()

local y = { t = x, k = 10 }
y.t.g = 100

collectgarbage("collect")
--tool.m_cMethods.DumpMemorySnapshot("./", "after", -1)

tool.m_cMethods.DumpMemorySnapshotSingleObject("./", "single-x", -1, "x", x)

tool.m_cMethods.DumpMemorySnapshotComparedFile("./", "Compared", -1, "before.txt", "after.txt")
