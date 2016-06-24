--[[
This module will do operations about an online filtering list file
--]]

local LIBPARSER_MODULE = {}

local fop = require("libfop")
--[[
parse LPM rules from string content
--]]

--[[
@content a string that contains all the IPv4 LPM rules
@groupid: the group of bandwidth limit and expiration time

@return value: the function returns the LPM rules table
with the last element specifying the groupid
--]]
function LIBPARSER_MODULE.default_ipv4_parser(content, groupid)
    local lpm_rules = {}

    if content == nil or content == "" then
        print("Cannot parse LPM rules, the cotent is empty!")
        return lpm_rules
    end

    --print(content)

    for rule in string.gmatch(content, "%d+%.%d+%.%d+%.%d+%/%d+") do
        local o1,o2,o3,o4,len = rule:match("(%d%d?%d?)%.(%d%d?%d?)%.(%d%d?%d?)%.(%d%d?%d?)%/(%d+)")
        local ip = 2^24*o1 + 2^16*o2 + 2^8*o3 + o4

        lpm_rules[#lpm_rules + 1] = {["raw"] = rule, ["ip"] = ip, ["prefix"] = len}
    end

    lpm_rules["group"] = groupid
    lpm_rules["proto"] = "IPv4"

    return lpm_rules
end

--[[
parse LPM rules from a file
--]]

--[[
@file: extracts LPM rules from the given file
@groupid: the group of bandwidth limit and expiration time

@return value: the function returns the LPM rules table 
with the last element specifying the groupid
--]]
function LIBPARSER_MODULE.default_ipv4_parser_from_file(file, groupid)

    local content = fop.content_from(file)

    local lpm_rules = LIBPARSER_MODULE.default_ipv4_parser(content, groupid)

    return lpm_rules
end

--need to do line by line
function LIBPARSER_MODULE.default_ipv6_parser(line, groupid)
    local lpm_rule = {}

    if line == nil or line == "" then
        print("Cannot parse LPM rules, the cotent is empty!")
        return lpm_rule
    end

    --print(line)

    local i = 8

    local content = ""

    while i >= 1 do

        local str = ("([a-fA-F0-9]*)%:"):rep(i):gsub(":$","")
        str = "^".. str .. "/(%d+)"

        --print(str)

        local chunks = {line:match(str)}

        if chunks ~= nil and #chunks ~= 0 then

            local size = #chunks

            for _,v in pairs(chunks) do
                if size == #chunks then
                    content = v
                elseif (size > 1) then
                    content = content .. ":" .. v
                else
                    content = content .. "/" .. v
                end

                size = size - 1
            end

            lpm_rule = {["raw"] = content, ["prefix"] = chunks[#chunks]}

            return lpm_rule
        end

        i = i - 1

    end

    return lpm_rule
end

function LIBPARSER_MODULE.default_ipv6_parser_from_file(file, groupid)

    local lpm_rules = {}

    local lines = fop.lines_from(file)

    for _,line in pairs(lines) do
        local lpm_rule = LIBPARSER_MODULE.default_ipv6_parser(line, groupid)

        if lpm_rule["raw"] ~= nil then
            lpm_rules[#lpm_rules + 1] = lpm_rule
        end
    end

    lpm_rules["group"] = groupid
    lpm_rules["proto"] = "IPv6"

    return lpm_rules
end

return LIBPARSER_MODULE
