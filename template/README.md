#Policies
This repository implements the Bogon policy, CloudFlare policy, and the Spamhaus' drop-all-traffic advisory policy.

#Policy Template
```
local libpolicy = require "libpolicy" --the library for maintaining the policy

local policy_config = {
    ["name"] = "Bogon", --the name of the policy
    ["ref"] = "http://www.team-cymru.org/bogon-reference.html", --the website that maintains the policy
    ["raw_dir"] = "./raw",  --the directory to store the download filtering list files
    ["rule_dir"] = "./rule", --the directory to store the extracted policy rules
    ["urls"] = { --the filtering lists need to be maintained
        --The Text Bogon List, Aggregated
        {
            ["link"] = "http://www.team-cymru.org/Services/Bogons/bogon-bn-agg.txt", --the link to update the filtering list
            ["interval"] = 4, --the time interval with hour unit to update the filtering list
            ["group"] = 1, --the group that the filtering list belongs to, which specifies the bandwidth limit, expiration time, etc.
            ["proto"] = "IPv4", --the protocol of the filtering list, "IPv4" or "IPv6"
            ["parser"] = "default_ipv4_parser", --the parser used to parse the download filtering list file, which extracts the LPM rules
            ["state"] = "used", --indicate whether the filtering list is used or not
            ["filename"] = "bogon-bn-agg", --the filename prefix
            ["desc"] = [[This is the list of bit notation bogons, aggregated, in text format.
            Updated as IANA allocations and special prefix reservations are made]], --the description of the filtering list
        },
    },
}

libpolicy.policy_start(policy_config) --function to start the policy
```

#TODO List

*   Add parsers for IPv6
*   Integrate with the DPDK LPM component, and the Interactive Configuration component

#Known Issues

*   The CloudFlare policy script cannot download the file, which always returns status code 301 without any filtering list content. This may be caused by the facts using *https*. However, the Spamhaus policy also use *https* protocol, and it can download the content correctly. (Solved by libcurl)
