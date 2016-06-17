package.path = package.path .. ";/Users/mengxiang/Documents/XIA/DDoS-system/Grantor/template/Common/?.lua;"

local urlop = require "liburl_ops"
local bogon_url = "http://www.team-cymru.org/Services/Bogons/bogon-bn-agg.txt"
local spamhaus_url = "https://www.spamhaus.org/drop/drop.txt"

--bogon list from cotent
local content = urlop.url2file(bogon_url, "bogon.txt")
local bogon_rules = urlop.parse(content, "default_ipv4_parser", 1)
urlop.print_parsed_rules(bogon_rules)

--spamhaus list from file
urlop.url2file(spamhaus_url, "drop.txt")
local spamhaus_rules = urlop.parse("drop.txt", "default_ipv4_parser_from_file", 1)
urlop.print_parsed_rules(spamhaus_rules)
