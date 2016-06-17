--[[
    This module will do operations about an online filtering list file
--]]

local LIBURLOP_MODULE = {}

local socket = require("socket")
local http = require("socket.http")
local parser = require("libparser")

--parser to parse LPM rules
local default_parser = "default_ipv4_parser"

local function check_params(url, rule_parser, file)

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
function LIBURLOP_MODULE.url2file(url, rule_parser, file)

    local ret = check_params(url, rule_parser, file)

    if ret == -1 then
        return
    end

    print("--->>> Processing url: " .. url)

    local body, code, headers = http.request(url)

    if code == 200 then
        --parse the file to extract LPM rules

        if rule_parser == nil or parser[rule_parser] == nil then 
            print("###### Invalid parser, using the default_ipv4_parser!")
            rule_parser = default_parser
        end

        local rules = parser[rule_parser](body)

        if not rules then
            print("###### HTTP code: " .. code .. "! Empty rules!")
            return
        else
            print("--->>> The parser extracts the following rules:")
            for i,v in pairs(rules) do
                io.write(i, ': ', v, '\n')
            end
        end
    else
        print("###### HTTP code: " .. code .. ", instead of 200!")
        return
    end

    -- save the content to a file
    local f = assert(io.open(file, 'wb')) -- open in "binary" mode
    f:write(body)
    f:close()
end

return LIBURLOP_MODULE
