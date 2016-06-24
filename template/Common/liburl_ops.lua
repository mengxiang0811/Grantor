--[[
This module will do operations on an online filtering list file
--]]

local LIBURLOP_MODULE = {}

--[[
local socket = require("socket")
local http = require("socket.http")
--TODO: need to support https
--local https = require("ssl.https")
--]]

local ffi = require 'ffi'
local fop = require("libfop")
local parser = require("liblpm_parser")

--parser to parse LPM rules
local default_parser = "default_ipv4_parser"

--Declare the C function
ffi.cdef[[
int url2file(const char *url, const char *outputfile);
]]

--ask LuaJIT to load the library that contains the functions
local download = ffi.load('download')

local function check_params(url, file)

    local ret = 0

    if url == nil then 
        print("###### Please confirm the URL address!")
        ret = -1
    end

    if file == nil then
        print("###### Please specify the path of the output file!")
        ret = -1
    end

    return ret
end

function LIBURLOP_MODULE.url2file(url, file)

    local ret = check_params(url, file)

    if ret == -1 then
        return ""
    end

    print("--->>> Processing url: " .. url)

    ret = download.url2file(url, file)

    if ret ~= 0 then
        return ""
    end

    return fop.content_from(file)
end

--[[
--use the libcurl instead, it easily supports the https protocol
--retrieve the content of a URL
--save the content to file
function LIBURLOP_MODULE.url2file(url, file)

    local ret = check_params(url, file)

    if ret == -1 then
        return ""
    end

    print("--->>> Processing url: " .. url)

    --TODO: need to support https
    local body, code, headers = http.request(url)

    if code ~= 200 then
        print("###### HTTP code: " .. code .. ", instead of 200!")
        return ""
    end

    -- save the content to a file
    local f = assert(io.open(file, 'wb')) -- open in "binary" mode
    f:write(body)
    f:close()

    --print(body)

    return body
end
--]]

--@file_or_content: the name of the file or the content itself
--@rule_parser: the name of the chosen LPM rule parser
--@groupid: the group of bandwidth limit and expiration time that the rules belong to
function LIBURLOP_MODULE.parse(file_or_content, rule_parser, groupid)
    --parse the file to extract LPM rules
    if rule_parser == nil or parser[rule_parser] == nil then 
        print("###### Invalid parser, using the default_ipv4_parser!")
        rule_parser = default_parser
    end

    return parser[rule_parser](file_or_content, groupid)
end

function LIBURLOP_MODULE.print_parsed_rules(rules)

    for i,v in pairs(rules) do

        if i == "group" or i == "proto" then
            io.write(i, ': ', v, '\n')
        else
            io.write(i, ': ', v["raw"], '\n')
        end
    end

end

--RETURN VALUE: the #rules written into the file
function LIBURLOP_MODULE.rules2file(rules, file)

    if rules == nil or file == nil or file == "" then
        return 0
    end
    -- save the content to a file
    local f = assert(io.open(file, 'wb')) -- open in "binary" mode
    
    for i,v in pairs(rules) do

        if i ~= "group" and i ~= "proto" then
            f:write("Entry{\n")
            f:write("\t[\"proto\"]\t=\t\"" .. rules["proto"] .. "\",\n")
            f:write("\t[\"raw\"]\t=\t\"" .. v["raw"] .. "\",\n")
            f:write("\t[\"ip\"]\t=\t" .. v["ip"] .. ",\n")
            f:write("\t[\"prefix\"]\t=\t" .. v["prefix"] .. ",\n")
            f:write("\t[\"nh\"]\t=\t" .. rules["group"] .. ",\n}\n")
            f:write("\n")
        end
    end

    f:close()
end

return LIBURLOP_MODULE
