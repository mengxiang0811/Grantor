--To add a new task for maintaining Spamhaus filtering list

package.path = package.path .. ";/Users/mengxiang/Documents/XIA/DDoS-system/Grantor/template/Common/?.lua;"

local libconfig = require "libconfig"

local spamhaus_drop_all_traffic_advisory = {
    ["name"] = "Spamhaus’ drop-all-traffic advisory",
    ["ref"] = "https://www.spamhaus.org/bgpf/",
    ["urls"] = {
        --The Spamhaus DROP (Don't Route Or Peer) lists
        {
            ["link"] = "https://www.spamhaus.org/drop/drop.txt",
            ["interval"] = 1,
            ["group"] = 1,
            ["proto"] = "IPv4",
            ["parser"] = "default_ipv4_parser",
            ["state"] = "used",
            ["desc"] = [[The Spamhaus DROP (Don't Route Or Peer) lists are advisory "drop all traffic" lists, consisting of netblocks that are "hijacked" or leased by professional spam or cyber-crime operations (used for dissemination of malware, trojan downloaders, botnet controllers).
            ]],
        },

        --Spamhaus Extended DROP List (EDROP)
        {
            ["link"] = "https://www.spamhaus.org/drop/edrop.txt",
            ["interval"] = 1,
            ["group"] = 1,
            ["proto"] = "IPv4",
            ["parser"] = "default_ipv4_parser",
            ["state"] = "unused",
            ["desc"] = [[EDROP is an extension of the DROP list that includes suballocated netblocks controlled by spammers or cyber criminals. EDROP is meant to be used in addition to the direct allocations on the DROP list.]],
        },

        --Spamhaus IPv6 DROP List (DROPv6)
        {
            ["link"] = "https://www.spamhaus.org/drop/dropv6.txt",
            ["interval"] = 1,
            ["group"] = 1,
            ["proto"] = "IPv6",
            ["parser"] = "spamhaus_ipv6",
            ["state"] = "unused",
            ["desc"] = [[The DROPv6 list includes IPv6 ranges allocated to spammers or cyber criminals. DROPv6 will only include IPv6 netblocks allocated directly by an established Regional Internet Registry (RIR) or National Internet Registry (NIR) such as ARIN, RIPE, AFRINIC, APNIC, LACNIC or KRNIC or direct RIR allocations.]],
        },

        --Spamhaus ASN DROP List (ASN-DROP)
        {
            ["link"] = "https://www.spamhaus.org/drop/asndrop.txt",
            ["interval"] = 1,
            ["group"] = 1,
            ["proto"] = "IPv4",
            ["parser"] = "default_ipv4_parser",
            ["state"] = "unused",
            ["desc"] = [[ASN-DROP contains a list of Autonomous System Numbers controlled by spammers or cyber criminals, as well as "hijacked" ASNs. ASN-DROP can be used to filter BGP routes which are being used for malicious purposes.]],
        },
    },
}

libconfig.print_url_config(spamhaus_drop_all_traffic_advisory)
local spamhaus_config = libconfig.get_effective_urls(spamhaus_drop_all_traffic_advisory)
libconfig.print_url_config(spamhaus_config)
