--[[
This module will take a policy configuration table as input, and then extract
the effective urls for further processing
--]]

local LIBCONFIG_MODULE = {}

function LIBCONFIG_MODULE.print_policy_config(policy_config)

    if policy_config == nil then
        print("Error: an empty policy configuration!")
        return
    end

    print("************The Policy Configuration************")
    print("Policy name: " .. policy_config["name"])
    print("Policy reference: " .. policy_config["ref"])

    local url_tbl = policy_config["urls"]

    for idx, url in pairs(url_tbl) do
        print("\t############URL " .. idx .. "############")
        print("\tURL: " .. url["link"])
        print("\tUpdate interval: " .. url["interval"] .. " hour(s)/update")
        print("\tPolicy group: " .. url["group"])
        print("\tPolicy proto: " .. url["proto"])
        print("\tPolicy parser: " .. url["parser"])
        print("\tPolicy state: " .. url["state"])
        print("\tPolicy description:") 
        print("\t\t" .. url["desc"])
        print("\t---------------------------")
    end

end

function LIBCONFIG_MODULE.get_effective_policy(policy_config)
    local effective_policy = {}

    if policy_config == nil then
        print("Error: an empty policy configuration!")
        return effective_policy
    end

    effective_policy["name"] = policy_config["name"]
    effective_policy["ref"] = policy_config["ref"]

    local url_tbl = policy_config["urls"]

    local eut = {}

    for idx, url in pairs(url_tbl) do
        if url["state"] == "used" then
            eut[#eut + 1] = url_tbl[idx]
        end
    end

    effective_policy["urls"] = eut

    return effective_policy

end

return LIBCONFIG_MODULE
