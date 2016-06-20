package.path = package.path .. ";/Users/mengxiang/Documents/XIA/DDoS-system/Grantor/template/Common/?.lua;"
--print(package.path)

local libpolicy = require "libpolicy"
local bogon_config = require "bogon_config".get_config()


libpolicy.print_policy_config(bogon_config)
local bogon_effective_config = libpolicy.get_effective_policy(bogon_config)
libpolicy.print_policy_config(bogon_effective_config)
