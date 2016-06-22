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

c:setoption("tcp-nodelay", true)

cmd_id = 0
len = 0
--setup the connection
assert(c:send(cmd_id .. " " .. len .. "\n"))
l, e = c:receive()
print(l)

print("Send file: bogon-bn-agg.rule")
local filename = "bogon-bn-agg.rule"

--send filename control information
cmd_id = 2
len = #filename
assert(c:send(cmd_id .. " " .. len .. "\n"))
l, e = c:receive()
print(l)

--send filename
assert(c:send(filename))
l, e = c:receive()
print(l)

print("Read file ...")
lines = lines_from("./bogon-bn-agg.rule")

--c:send(1 .. " " .. #lines .. "\n")

content = ""
length = 0
for k,v in pairs(lines) do
    length = length + #v + 1
    content = content .. v .. "\n"
end

print("Notify the server the length of the content!")

--send file data control information
cmd_id = 3
len = length
assert(c:send(cmd_id .. " " .. len .. "\n"))
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

--send file data
assert(c:send(content))
l, e = c:receive()
print(l)


print("Close the connection!!!")
--terminate the connection
cmd_id = 1
len = 0
assert(c:send(cmd_id .. " " .. len .. "\n"))
l, e = c:receive()
print(l)

c:close()
