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

return LIBPARSER_MODULE
