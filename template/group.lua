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
        limit = "15 KB",
        expire = "80 sec",
        action = "ACCEPT",
    },
}

local groups = {
    group1,
    group2,
}

function GROUP_CONFIG_MODULE.init()
    --TODO: translate the bandwidth limit and expiration time to standard units
    return groups
end

return GROUP_CONFIG_MODULE
