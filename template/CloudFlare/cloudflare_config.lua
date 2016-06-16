--To add a new task for maintaining the CloudFlare filtering list

package.path = package.path .. ";/Users/mengxiang/Documents/XIA/DDoS-system/Grantor/template/Common/?.lua;"

local libconfig = require "libconfig"

local CloudFlare_ips = {
    ["name"] = "CloudFlare IP addresses",
    ["ref"] = "https://www.cloudflare.com/ips/",
    ["urls"] = {
        --IPv4
        {
            ["link"] = "https://www.cloudflare.com/ips-v4",
            ["interval"] = 1,
            ["group"] = 2,
            ["parser"] = "default_ipv4_parser",
            ["state"] = "used",
            ["desc"] = [[The definitive source of CloudFlare’s current IPv4 ranges.]],
        },
        
        --IPv6
        {
            ["link"] = "https://www.cloudflare.com/ips-v6",
            ["interval"] = 1,
            ["group"] = 2,
            ["parser"] = "default_ipv6_parser",
            ["state"] = "unused",
            ["desc"] = [[The definitive source of CloudFlare’s current IPv6 ranges.]],
        },
    },
}

libconfig.print_url_config(CloudFlare_ips)
local cloudflare_config = libconfig.get_effective_urls(CloudFlare_ips)
libconfig.print_url_config(cloudflare_config)
