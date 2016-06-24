--[[
    This file contains the basic file operations, which may be used by several other modules.
--]]

local LIBFOP_MODULE = {}

--to see if the file exists
function LIBFOP_MODULE.file_exists(file)
    local f = io.open(file, "rb")

    if f then f:close() end

    return f ~= nil
end

--extract the file content as a string
function LIBFOP_MODULE.content_from(file)
    local content = ""

    if not LIBFOP_MODULE.file_exists(file) then return content end

    local f = io.open(file, "rb")
    content = f:read("*all")
    f:close()

    return content
end

return LIBFOP_MODULE
