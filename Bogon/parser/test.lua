local parser = require "parser"

local file_location = "./bogon-bn-agg.txt"

local results = parser.parse(file_location, 0)

for k,v in pairs(results) do
    if k == "group" then
        print(k .. "=" .. v)
    else
        print(string.format("%x", v["ip"]) .. "---" .. v["prefix"])
    end
end
