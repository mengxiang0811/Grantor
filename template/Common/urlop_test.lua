package.path = package.path .. ";/Users/mengxiang/Documents/XIA/DDoS-system/Grantor/template/Common/?.lua;"

local urlop = require "liburl_ops"

local bogon_url = "http://www.team-cymru.org/Services/Bogons/bogon-bn-agg.txt"
local spamhaus_url = "https://www.spamhaus.org/drop/drop.txt"
local cloudflare_url = "https://www.cloudflare.com/ips-v4"

--bogon list from cotent
print("####################################")
local bogon_content = urlop.url2file(bogon_url, "bogon.txt")
local bogon_rules = urlop.parse(bogon_content, "default_ipv4_parser", 1)
urlop.print_parsed_rules(bogon_rules)
urlop.rules2file(bogon_rules, "bogon-format.data")

--spamhaus list from file
print("####################################")
urlop.url2file(spamhaus_url, "drop.txt")
local spamhaus_rules = urlop.parse("drop.txt", "default_ipv4_parser_from_file", 2)
urlop.print_parsed_rules(spamhaus_rules)

--cloudflare list from content
print("####################################")
local cloudflare_content = urlop.url2file(cloudflare_url, "cloudflare.txt")
local cloudflare_rules = urlop.parse(cloudflare_content, "default_ipv4_parser", 3)
urlop.print_parsed_rules(cloudflare_rules)
urlop.rules2file(cloudflare_rules, "cloudflare-format.data")
