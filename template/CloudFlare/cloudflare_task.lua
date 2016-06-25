--To add a new task for maintaining the CloudFlare filtering list

package.path = package.path .. ";/Users/mengxiang/Documents/XIA/DDoS-system/Grantor/template/Common/?.lua;"
--print(package.path)

local libpolicy = require "libpolicy"

local policy_config = {
    ["name"] = "CloudFlare IP addresses",
    ["ref"] = "https://www.cloudflare.com/ips/",
    ["raw_dir"] = "./raw",
    ["rule_dir"] = "./rule",
    ["urls"] = {
        --IPv4
        {
            --["link"] = "https://www.cloudflare.com/ips/",
            ["link"] = "https://www.cloudflare.com/ips-v4",
            ["interval"] = 1,
            ["group"] = 2,
            ["proto"] = "IPv4",
            ["parser"] = "default_ipv4_parser",
            ["state"] = "used",
            ["filename"] = "cloudflare-ips-v4",
            ["desc"] = [[The definitive source of CloudFlare’s current IPv4 ranges.]],
        },
        
        --IPv6
        {
            ["link"] = "https://www.cloudflare.com/ips-v6",
            ["interval"] = 1,
            ["group"] = 2,
            ["proto"] = "IPv6",
            ["parser"] = "default_ipv6_parser",
            ["state"] = "used",
            ["filename"] = "cloudflare-ips-v6",
            ["desc"] = [[The definitive source of CloudFlare’s current IPv6 ranges.]],
        },
    },
}

libpolicy.policy_start(policy_config)
