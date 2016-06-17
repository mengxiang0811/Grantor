package.path = package.path .. ";/Users/mengxiang/Documents/XIA/DDoS-system/Grantor/template/Common/?.lua;"

local urlop = require "liburlop"
local url = "http://www.team-cymru.org/Services/Bogons/bogon-bn-agg.txt"
--local url = "https://www.spamhaus.org/drop/drop.txt"

--urlop.url2file(url, "default_ipv4_parser", "drop.txt")
urlop.url2file(url, "", "bogon.txt")
