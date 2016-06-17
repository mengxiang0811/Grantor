--[[
This module will do operations on an online filtering list file
--]]

local LIBURLOP_MODULE = {}

local socket = require("socket")
local http = require("socket.http")
local parser = require("libparser")

--parser to parse LPM rules
local default_parser = "default_ipv4_parser"

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

--retrieve the content of a URL
function LIBURLOP_MODULE.url2file(url, file)

    local ret = check_params(url, file)

    if ret == -1 then
        return ""
    end

    print("--->>> Processing url: " .. url)

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

return LIBURLOP_MODULE
