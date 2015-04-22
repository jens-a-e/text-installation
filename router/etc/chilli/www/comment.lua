-- textinstallation - comment receiver script
-- jens alexander ewald, lea.io, 2015

-- Import the JSON en-/decoder
JSON = (loadfile "JSON.lua")()

-- Helper Functions
local hex_to_char = function(x)
  return string.char(tonumber(x, 16))
end
local unescape = function(url)
  return url:gsub("%%(%x%x)", hex_to_char)
end
local lines = function(str)
  local t = {}
  local function helper(line) table.insert(t, line) return "" end
  helper((str:gsub("(.-)\r?\n", helper)))
  return t
end
-- END Helper Functions

local data = {}
local query = ""

-- Get the current `env`
local handle = io.popen("env")
local result = handle:read("*a")
handle:close()
local env = lines(result)
-- os.execute("logger -t \"kommentar.env\" \"".. result .. "\"\n")

-- Extract the QUERY_STRING (chilli specific)
for i,v in pairs(env) do
  i,j = string.find(v, "QUERY_STRING=")
  if i == 1 then
    query = string.sub(v,j+1,string.len(v))
    query = unescape(query)
  end
end

-- Decode the JSON from the found query
local input = JSON:decode(query)

data["comment"] = input

-- add the comment to the DB

-- notify clients to get new DB


-- respond back with the current comment
local body = JSON:encode_pretty(data)

os.execute("logger -t \"kommentar.body\" '" .. body .. "'\n")


-- Respond with OK and curretn comment
local header = [[HTTP/1.0 200 OK
Content-Type: application/json
Cache: none
Connection: close

]]
print(header .. body .. '\n\n')