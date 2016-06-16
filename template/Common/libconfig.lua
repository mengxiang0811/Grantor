--[[
    This module will take a url configuration table as input, and then extract
    the effective urls for further processing
--]]

local LIBCONFIG_MODULE = {}

function LIBCONFIG_MODULE.print_url_config(url_config)
    
    if url_config == nil then
        print("Error: an empty url configuration!")
        return
    end

    print("************The URL Configuration************")
    print("Policy name: " .. url_config["name"])
    print("Policy reference: " .. url_config["ref"])

    local url_tbl = url_config["urls"]

    for idx, url in pairs(url_tbl) do
        print("\t############URL " .. idx .. "############")
        print("\tURL: " .. url["link"])
        print("\tUpdate interval: " .. url["interval"] .. " hour(s)/update")
        print("\tPolicy group: " .. url["group"])
        print("\tPolicy parser: " .. url["parser"])
        print("\tPolicy state: " .. url["state"])
        print("\tPolicy description:") 
        print("\t\t" .. url["desc"])
        print("\t---------------------------")
    end

end

function LIBCONFIG_MODULE.get_effective_urls(url_config)
    local effective_urls = {}

    if url_config == nil then
        print("Error: an empty url configuration!")
        return effective_urls
    end

    effective_urls["name"] = url_config["name"]
    effective_urls["ref"] = url_config["ref"]

    local url_tbl = url_config["urls"]

    local eut = {}

    for idx, url in pairs(url_tbl) do
        if url["state"] == "used" then
            eut[#eut + 1] = url_tbl[idx]
        end
    end

    effective_urls["urls"] = eut

    return effective_urls

end

return LIBCONFIG_MODULE
