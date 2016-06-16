--To add a new task for maintaining a filtering list

local URL_CONFIG_MODULE = {}

local bogon_filter_list = {
    ["name"] = "Bogon",
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

local spamhaus_drop_all_traffic_advisory = {
    ["name"] = "Spamhaus’ drop-all-traffic advisory",
    ["urls"] = {
        --The Spamhaus DROP (Don't Route Or Peer) lists
        {
            ["link"] = "https://www.spamhaus.org/drop/drop.txt",
            ["interval"] = 1,
            ["group"] = 1,
            ["parser"] = "spamhaus",
            ["state"] = "used",
            ["desc"] = [[The Spamhaus DROP (Don't Route Or Peer) lists are advisory "drop all traffic" lists, consisting of netblocks that are "hijacked" or leased by professional spam or cyber-crime operations (used for dissemination of malware, trojan downloaders, botnet controllers).
            ]],
        },

        --Spamhaus Extended DROP List (EDROP)
        {
            ["link"] = "https://www.spamhaus.org/drop/edrop.txt",
            ["interval"] = 1,
            ["group"] = 1,
            ["parser"] = "spamhaus",
            ["state"] = "unused",
            ["desc"] = [[EDROP is an extension of the DROP list that includes suballocated netblocks controlled by spammers or cyber criminals. EDROP is meant to be used in addition to the direct allocations on the DROP list.]],
        },
    },
}

local URLs = {
    spamhaus_drop_all_traffic_advisory,
    bogon_filter_list,
}

function URL_CONFIG_MODULE.init()
    local effective_urls = {}
    return effective_urls
end

return URL_CONFIG_MODULE
