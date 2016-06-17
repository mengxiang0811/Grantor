--[[
    This module will do operations about an online filtering list file
--]]

local LIBPARSER_MODULE = {}

function LIBPARSER_MODULE.default_ipv4_parser(content)
    local lpm_rules = {}

    for rule in string.gmatch(content, "%d+%.%d+%.%d+%.%d+%/%d+") do
        lpm_rules[#lpm_rules + 1] = rule
        --print(rule)
    end

    return lpm_rules
end

return LIBPARSER_MODULE
