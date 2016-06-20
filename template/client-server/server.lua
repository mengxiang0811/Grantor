-----------------------------------------------------------------------------
-- TCP sample: Little program to dump lines received at a given port
-----------------------------------------------------------------------------
local socket = require("socket")

host = host or "*"
port = port or 8080

if arg then
	host = arg[1] or host
	port = arg[2] or port
end

print("Binding to host '" ..host.. "' and port " ..port.. "...")

s = assert(socket.bind(host, port))
i, p   = s:getsockname()
assert(i, p)

print("Waiting connection from talker on " .. i .. ":" .. p .. "...")

c = assert(s:accept())

print("Connected. Here is the stuff:")

local f = io.open("test.txt", "wb")

l, e = c:receive()
f:write(l .. "\n")

while not e do
    print(l)

    l, e = c:receive()

    if l ~= nil then
        f:write(l .. "\n")
    end
end

c:close()
f:close()
print(e)
