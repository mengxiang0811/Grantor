--This is the module to maintain the group information

local GROUP_CONFIG_MODULE = {}

--Deny all the packets
local group1 = {
    ["params"] = {
        action = "DENY",
    },
}

local group2 = {
    ["params"] = {
        action = "ACCEPT",
    },
}

local groups = {
    group1,
    group2,
}

function GROUP_CONFIG_MODULE.init()
    local standard_groups = {}
    --local standard_groups = groups
    --TODO: translate the bandwidth limit and expiration time to standard units
    return standard_groups
end

return GROUP_CONFIG_MODULE
