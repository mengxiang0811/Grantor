--[[
This module will take a policy configuration table as input, and then extract
the effective urls for further processing
--]]

local LIBPOLICY_MODULE = {}

local urlop = require "liburl_ops"

function LIBPOLICY_MODULE.print_policy_config(policy_config)

    if policy_config == nil then
        print("######Error: an empty policy configuration!")
        return
    end

    print("************The Policy Configuration************")
    print("Policy name: " .. policy_config["name"])
    print("Policy reference: " .. policy_config["ref"])
    print("Policy raw data directory: " .. policy_config["raw_dir"])
    print("Policy rules directory: " .. policy_config["rule_dir"])

    local url_tbl = policy_config["urls"]

    for idx, url in pairs(url_tbl) do
        print("\t############URL " .. idx .. "############")
        print("\tURL: " .. url["link"])
        print("\tUpdate interval: " .. url["interval"] .. " hour(s)/update")
        print("\tPolicy group: " .. url["group"])
        print("\tPolicy proto: " .. url["proto"])
        print("\tPolicy parser: " .. url["parser"])
        print("\tPolicy state: " .. url["state"])
        print("\tPolicy stored filename: " .. url["filename"])
        print("\tPolicy description:") 
        print("\t\t" .. url["desc"])
        print("\t---------------------------")
    end

end

function LIBPOLICY_MODULE.get_effective_policy(policy_config)
    local effective_policy = {}

    if policy_config == nil then
        print("Error: an empty policy configuration!")
        return effective_policy
    end

    effective_policy["name"] = policy_config["name"]
    effective_policy["ref"] = policy_config["ref"]
    effective_policy["raw_dir"] = policy_config["raw_dir"]
    effective_policy["rule_dir"] = policy_config["rule_dir"]

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


local ffi = require 'ffi'

ffi.cdef[[
void Sleep(int ms);
int poll(struct pollfd *fds, unsigned long nfds, int timeout);
]]

local sleep
if ffi.os == "Windows" then
    function sleep(s)
        ffi.C.Sleep(s*1000)
    end
else
    function sleep(s)
        ffi.C.poll(nil, 0, s*1000)
    end
end


--the loop to update a policy
function LIBPOLICY_MODULE.policy_schedule(policy)

    while true do

        local next_schedule_interval = 2^32 - 1
        local url_tbl = policy["urls"]

        for idx, url in pairs(url_tbl) do
            if url["last_update_time"] == nil then
                url["last_update_time"] = os.time() - url["interval"] * 3600
            end

            local cur_time = os.time()

            --time to update again
            if cur_time - url["last_update_time"] >= (url["interval"] * 3600) then
                print("******update URL: " .. url["link"])

                --download the file specified by the url, and save it to the raw data directory
                --local content = urlop.url2file(url["link"], policy["raw_dir"] .. "/" .. url["filename"] .. ".raw")
                urlop.url2file(url["link"], policy["raw_dir"] .. "/" .. url["filename"] .. ".raw")
                --parse the content to LPM rules
                --local rules = urlop.parse(content, url["parser"], url["group"])
                local rules = urlop.parse(policy["raw_dir"] .. "/" .. url["filename"] .. ".raw", url["parser"], url["group"])
                --print the rules out, and verify the correctness of the LPM rules.
                --for debug purpose
                urlop.print_parsed_rules(rules)
                --save the rules the rule directory
                urlop.rules2file(rules, policy["rule_dir"] .. "/" .. url["filename"] .. ".rule")

                --TODO: send the extracted rules to the server

                --update the last_update_time field
                url["last_update_time"] = os.time()
            end

            --find the nearest update
            local time_to_update = url["interval"] * 3600 - (os.time() - url["last_update_time"])

            if next_schedule_interval > time_to_update then
                next_schedule_interval = time_to_update + 60 --add some delay
            end

        end

        --sleep some time until the next update comes
        sleep(next_schedule_interval)

    end
end

function LIBPOLICY_MODULE.policy_start(policy_config)

    if policy_config == nil then
        print("######Error: empty policy configuration!")
        return
    end

    os.execute("mkdir " .. policy_config["raw_dir"])
    os.execute("mkdir " .. policy_config["rule_dir"])
    
    LIBPOLICY_MODULE.print_policy_config(policy_config)
    local effective_policy_config = LIBPOLICY_MODULE.get_effective_policy(policy_config)
    LIBPOLICY_MODULE.print_policy_config(effective_policy_config)
    LIBPOLICY_MODULE.policy_schedule(effective_policy_config)
end

return LIBPOLICY_MODULE
