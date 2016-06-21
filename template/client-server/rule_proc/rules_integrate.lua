--import the policy data

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
