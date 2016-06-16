--To add a new task for maintaining the bogon filtering list

package.path = package.path .. ";/Users/mengxiang/Documents/XIA/DDoS-system/Grantor/template/Common/?.lua;"
--print(package.path)

local libconfig = require "libconfig"

local bogon_filter_list = {
    ["name"] = "Bogon",
    ["ref"] = "http://www.team-cymru.org/bogon-reference.html",
    ["urls"] = {
        --The Text Bogon List, Unaggregated
        {
            ["link"] = "http://www.team-cymru.org/Services/Bogons/bogon-bn-nonagg.txt",
            ["interval"] = 4,
            ["group"] = 1,
            ["parser"] = "default_ipv4_parser",
            ["state"] = "unused",
            ["desc"] = [[This is the list of bit notation bogons, unaggregated, in text format.
            Updated as IANA allocations and special prefix reservations are made]],
        },

        --The Text Bogon List, Aggregated
        {
            ["link"] = "http://www.team-cymru.org/Services/Bogons/bogon-bn-agg.txt",
            ["interval"] = 4,
            ["group"] = 1,
            ["parser"] = "default_ipv4_parser",
            ["state"] = "used",
            ["desc"] = [[This is the list of bit notation bogons, aggregated, in text format.
            Updated as IANA allocations and special prefix reservations are made]],
        },

        --IPv4 Fullbogons
        {
            ["link"] = "http://www.team-cymru.org/Services/Bogons/fullbogons-ipv4.txt",
            ["interval"] = 4,
            ["group"] = 1,
            ["parser"] = "default_ipv4_parser",
            ["state"] = "unused",
            ["desc"] = [[The traditional bogon prefixes, plus prefixes that have been allocated to RIRs but not yet assigned by those RIRs to ISPs, end-users, etc.
            Updated every four hours.]]
        },

        --IPv6 Fullbogons
        {
            ["link"] = "http://www.team-cymru.org/Services/Bogons/fullbogons-ipv6.txt",
            ["interval"] = 4,
            ["group"] = 1,
            ["parser"] = "default_ipv6_parser",
            ["state"] = "unused",
            ["desc"] = [[IPv6 "fullbogons", all IPv6 prefixes that have not been allocated to RIRs and that have not been assigned by RIRs to ISPs, end-users, etc.
            Updated every four hours.]],
        },
    },
}

libconfig.print_url_config(bogon_filter_list)
