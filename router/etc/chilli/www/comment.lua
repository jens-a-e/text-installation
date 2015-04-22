JSON = (loadfile "JSON.lua")()

local handle = io.popen("env")
local result = handle:read("*a")
handle:close()
os.execute("logger -t \"kommentar.env\" \"".. result .. "\"")

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

local data = {}
local query = ""

local env = lines(result)
for i,v in pairs(env) do
  i,j = string.find(v, "QUERY_STRING=")
  if i == 1 then
    query = string.sub(v,j,string.len(v))
  end
end

data["comment"] = unescape(query)
data["query"] = query
data["env"] = env

local header = [[HTTP/1.0 200 OK
Content-Type: application/json
Cache: none
Connection: close

]]

local body = JSON:encode_pretty(data)
local response = header .. body .. '\n\n'

os.execute("logger -t \"kommentar.body\" \"" .. body.. "\"")
os.execute("logger -t \"kommentar\" \"" .. response.. "\"")
print(response .. '\n')