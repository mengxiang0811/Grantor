--import the policy data
--the results can be called by the LPM management lua scripts

local pl_num = 0
local policies = {}
local prefixes = {}  -- a set to collect prefixes

function Entry (e)
    if type(e) == "table" then
        pl_num = pl_num + 1
    else
        return
    end

    policies[pl_num] = e
    prefixes[pl_num] = e["raw"]
end

--we only need to do the dynamically generated lua script 
--to load all the policies
dofile("rules_preproc.lua")

for idx, rule in ipairs(prefixes) do print(rule) end

print("----------------------------------------")
print("----------------------------------------")
print("----------------------------------------")

for idx,policy in ipairs(policies) do
    local gid = policy["nh"]
    print(policy["raw"] .. " with group " .. gid)
    --print_serialize(groups[gid], 0)
end
