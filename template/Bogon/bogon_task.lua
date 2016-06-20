--To add a new task for maintaining the bogon filtering list

package.path = package.path .. ";/Users/mengxiang/Documents/XIA/DDoS-system/Grantor/template/Common/?.lua;"
--print(package.path)

local libpolicy = require "libpolicy"

local policy_config = {
    ["name"] = "Bogon",
    ["ref"] = "http://www.team-cymru.org/bogon-reference.html",
    ["raw_dir"] = "./raw",
    ["rule_dir"] = "./rule",
    ["urls"] = {
        --The Text Bogon List, Unaggregated
        {
            ["link"] = "http://www.team-cymru.org/Services/Bogons/bogon-bn-nonagg.txt",
            ["interval"] = 4,
            ["group"] = 1,
            ["proto"] = "IPv4",
            ["parser"] = "default_ipv4_parser",
            ["state"] = "unused",
            ["filename"] = "bogon-bn-nonagg",
            ["desc"] = [[This is the list of bit notation bogons, unaggregated, in text format.
            Updated as IANA allocations and special prefix reservations are made]],
        },

        --The Text Bogon List, Aggregated
        {
            ["link"] = "http://www.team-cymru.org/Services/Bogons/bogon-bn-agg.txt",
            ["interval"] = 4,
            ["group"] = 1,
            ["proto"] = "IPv4",
            ["parser"] = "default_ipv4_parser",
            ["state"] = "used",
            ["filename"] = "bogon-bn-agg",
            ["desc"] = [[This is the list of bit notation bogons, aggregated, in text format.
            Updated as IANA allocations and special prefix reservations are made]],
        },

        --IPv4 Fullbogons
        {
            ["link"] = "http://www.team-cymru.org/Services/Bogons/fullbogons-ipv4.txt",
            ["interval"] = 4,
            ["group"] = 1,
            ["proto"] = "IPv4",
            ["parser"] = "default_ipv4_parser",
            ["state"] = "unused",
            ["filename"] = "fullbogons-ipv4",
            ["desc"] = [[The traditional bogon prefixes, plus prefixes that have been allocated to RIRs but not yet assigned by those RIRs to ISPs, end-users, etc.
            Updated every four hours.]]
        },

        --IPv6 Fullbogons
        {
            ["link"] = "http://www.team-cymru.org/Services/Bogons/fullbogons-ipv6.txt",
            ["interval"] = 4,
            ["group"] = 1,
            ["proto"] = "IPv6",
            ["parser"] = "default_ipv6_parser",
            ["state"] = "unused",
            ["filename"] = "fullbogons-ipv6",
            ["desc"] = [[IPv6 "fullbogons", all IPv6 prefixes that have not been allocated to RIRs and that have not been assigned by RIRs to ISPs, end-users, etc.
            Updated every four hours.]],
        },
    },
}

libpolicy.policy_start(policy_config)
