--import the policy data
local pl_num = 0
local policies = {}
local prefixes = {}  -- a set to collect prefixes

function print_serialize (o, level)
    local pre_str = string.rep("\t", level)
    if type(o) == "number" then
        io.write(pre_str .. o .. ",")
    elseif type(o) == "string" then
        io.write(pre_str .. string.format("%q", o) .. ",\n")
    elseif type(o) == "boolean" then
        if o == true then
            io.write(pre_str .. "true,\n")
        else
            io.write(pre_str .. "false,\n")
        end
    elseif type(o) == "table" then
        io.write(pre_str .. "{\n")
        for k,v in pairs(o) do
            io.write(pre_str .. "  ", k, " = ")
            print_serialize(v, level + 1)
        end
        io.write(pre_str .. "}\n")
    else
        io.write(pre_str .. "cannot serialize print a " .. type(o))
    end
end

function Entry (e)
    if type(e) == "table" then
        pl_num = pl_num + 1
    else
        return
    end

    policies[pl_num] = e
    prefixes[e.ip] = true 
end

dofile("policy_data")

for ip in pairs(prefixes) do print(ip) end

for idx,policy in ipairs(policies) do
    local gid = policy.group
    print(policy.ip .. " with group " .. gid)
    print("group" .. gid .. "= ")
    print_serialize(groups[gid], 0)
end

--[[
local p1 = "187.73.32.0/27 (TCP, 22<-), (TCP, 80<-), (TCP, 443<-), (TCP, 43<-), (TCP, 700<-), (TCP, 5672<-), (TCP, 25672<-), (TCP, 4369<-),(TCP, ->11371), (UDP, 1194<-), (UDP, 694<-)"

print(p1)

local str = "127.0.0.1"
local o1,o2,o3,o4 = str:match("(%d%d?%d?)%.(%d%d?%d?)%.(%d%d?%d?)%.(%d%d?%d?)" )
local num = 2^24*o1 + 2^16*o2 + 2^8*o3 + o4

print(o1)
print(o2)
print(o3)
print(o4)
print(num)
--]]
