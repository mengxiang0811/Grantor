--[[
    Example Lua module to parse a given Bogon ipv4 list file
    http://lua-users.org/wiki/FileInputOutput
--]]

local parser_mod = {}

-- see if the file exists
local function file_exists(file)
    local f = io.open(file, "r")
    if f then f:close() end
    return f ~= nil
end

-- get all lines from a file, returns an empty
-- list/table if the file does not exist
local function lines_from(file)
    if not file_exists(file) then return {} end
    lines = {}
    for line in io.lines(file) do 
        lines[#lines + 1] = line
    end
    return lines
end

--[[
    the groupid means the group of bandwidth limit and expiration time
    the function returns the LPM rules table with the last element specifying
    the groupid
--]]
function parser_mod.parse(file, groupid)

    local lpm_rules = {}
    local lines = lines_from(file)

    for k,v in pairs(lines) do
        print(v)
        local o1,o2,o3,o4,len = v:match("(%d%d?%d?)%.(%d%d?%d?)%.(%d%d?%d?)%.(%d%d?%d?)%/(%d+)")
        local ip = 2^24*o1 + 2^16*o2 + 2^8*o3 + o4

        lpm_rules[#lpm_rules + 1] = {["ip"] = ip, ["prefix"] = len}
    end

    lpm_rules["group"] = groupid
    return lpm_rules
end

return parser_mod
