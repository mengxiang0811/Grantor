-----------------------------------------------------------------------------
-- TCP sample: Little program to send text lines to a given host/port
-----------------------------------------------------------------------------
local socket = require("socket")

host = host or "localhost"
port = port or 8080

if arg then
	host = arg[1] or host
	port = arg[2] or port
end

--to see if the file exists
local function file_exists(file)
    local f = io.open(file, "rb")

    if f then f:close() end

    return f ~= nil
end

function lines_from(file)
    if not file_exists(file) then return {} end
    lines = {}
    for line in io.lines(file) do 
        lines[#lines + 1] = line
    end
    return lines
end

print("Attempting connection to host '" ..host.. "' and port " ..port.. "...")

c = assert(socket.connect(host, port))

print("Connected!")
print("Send file: bogon-bn-agg.rule")

c:setoption("tcp-nodelay", true)

local filename = "bogon-bn-agg.rule"

assert(c:send(0 .. " " .. #filename .. " " .. filename .. "\n"))

l, e = c:receive()

print(l)

lines = lines_from("./bogon-bn-agg.rule")

--c:send(1 .. " " .. #lines .. "\n")

content = ""
length = 0
for k,v in pairs(lines) do
    length = length + #v + 1
    content = content .. v .. "\n"
end

print("Notify the server the length of the content!")
assert(c:send(1 .. " " .. length .. "\n"))

l, e = c:receive()

print(l)

--[[
for k,v in pairs(lines) do
    print('line[' .. k .. ']', v)
    --assert(c:send(2 .. " " .. #v .. " " .. v .. "\n"))
    assert(c:send(v .. "\n"))
end
--]]

print("Begin to send content!!!")
assert(c:send(content))

print("Close the connection!!!")
assert(c:send(2 .. "\n"))

l, e = c:receive()

print(l)

c:close()
