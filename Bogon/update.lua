--[[
    To keep the Bogon list updated, we may have many choices, e.g., 
    (1) Scripts using Linux commands, e.g., wget, curl commands, etc.
    (2) We may use LuaSocket(http://w3.impa.br/~diego/software/luasocket/), 
        which is a Lua extension library composed by two parts: a C core that 
        provides support for the TCP and UDP transport layers, and a set of Lua 
        modules that add support for functionality commonly needed by applications 
        that deal with the Internet. However, the official website of LuaSocket 
        has not been updated for long time. It may be out-of-dated once 
        Gatekeeper is in production.
        
        Github version: https://github.com/diegonehab/luasocket
        Besides, LuaSocket has many features that are not neccessary for our project.
    (3) LuaCURL (http://luacurl.luaforge.net) is Lua 5.x compatible module 
        providing Internet browsing capabilities based on the CURL library.
        However, the LuaCURL official website also has not been updated for a 
        long time. The latest version is here:
        http://files.luaforge.net/releases/luacurl/luacurl

        You can also find latest github version of Lua binding to libcurl here:
        https://github.com/Lua-cURL/Lua-cURLv3

        Besides, LuaCURL has many features that are not neccessary for our project.
    (4) LuaJIT FFI library + libcurl. The LuaJIT FFI library allows us to call
        external C functions and using C data structures from pure Lua code.
        
        The FFI library largely obviates the need to write tedious manual Lua/C 
        bindings in C. So, we can easily call the C functons in Lua.

        The libcurl is well maintained, including many examples, so we can 
        always use its latest APIs and modify the examples to meet our requirements.

        One easy way is to wrap the essential libcurl calls to meet our own needs, 
        and generate our own library, which can be called easily by LuaJIT FFI Library.
--]]

-- Load the LuaJIT FFI library.
local ffi = require 'ffi'

--Declare the C function.
ffi.cdef[[
int url2file(const char *outputfile, const char *url);
void Sleep(int ms);
int poll(struct pollfd *fds, unsigned long nfds, int timeout);
]]

--Ask LuaJIT to load the library that contains the functions
local download = ffi.load('download')

local function createDir (dirname)
    os.execute("mkdir " .. dirname)
end

local sleep
if ffi.os == "Windows" then
    function sleep(s)
        ffi.C.Sleep(s*1000)
    end
else
    function sleep(s)
        ffi.C.poll(nil, 0, s*1000)
    end
end

local function fetch_files(folder, index)
    --invalid index
    if ((index < 1) or (index > 6)) then
        return
    else 

    print("The current index is " .. index)

    --Download the fullbogons for IPv4 addresses, 
    --The traditional bogon prefixes, plus prefixes that have been allocated to RIRs 
    --but not yet assigned by those RIRs to ISPs, end-users, etc.
    of_full_v4 = folder .. "/fullbogons-ipv4-" .. index .. ".txt"
    fullbogons_v4_url = "http://www.team-cymru.org/Services/Bogons/fullbogons-ipv4.txt"
    print("Download the online file: " .. fullbogons_v4_url)
    download.url2file(of_full_v4, fullbogons_v4_url)

    --download the list of bit notation bogons, aggregated, in text format.
    of_bogon = folder .. "/bogon-bn-agg-" .. index .. ".txt"
    bogons_url = "http://www.team-cymru.org/Services/Bogons/bogon-bn-agg.txt"
    print("Download the online file: " .. bogons_url)
    download.url2file(of_bogon, bogons_url)

    --download the fullbogons for IPv6 addresses
    --IPv6 "fullbogons", all IPv6 prefixes that have not been allocated to RIRs and 
    --that have not been assigned by RIRs to ISPs, end-users, etc.
    of_full_v6 = folder .. "/fullbogons-ipv6-" .. index .. ".txt"
    fullbogons_v6_url = "http://www.team-cymru.org/Services/Bogons/fullbogons-ipv6.txt"
    print("Download the online file: " .. fullbogons_v6_url)
    download.url2file(of_full_v6, fullbogons_v6_url)
    end
end

local directory
local order = 1
local last_time = os.date("*t", os.time())
local cur_time = os.date("*t", os.time())
local sleep_time = 2 * 60 * 60 -- seconds for 2 hours

--Updated every four hours.
--Download 6 times per day
while true do

    --current time
    cur_time = os.date("*t", os.time())

    --a new day, update the time
    if directory == nil or (cur_time.year ~= last_time.year) or (cur_time.month ~= last_time.month) or (cur_time.day ~= last_time.day) then
        order = 1
        --current date
        --print(os.date("%Y%m%d"))
        directory = os.date("%Y%m%d")
        createDir(directory)
        fetch_files(directory, order)

        last_time = os.date("*t", os.time())
        order =  order + 1
    --elseif order <= 4 then
    elseif order <= 6 and cur_time.hour - last_time.hour > 4 then
        fetch_files(directory, order)
        --update the last download time
        last_time = os.date("*t", os.time())
        order =  order + 1
    end

    --sleep(1)
    sleep(sleep_time)
end

