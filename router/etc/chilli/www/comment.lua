-- textinstallation - comment receiver script
-- jens alexander ewald, lea.io, 2015

local db = "/root/textinstallation/db/comments.csv"

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
-- from http://lua-users.org/wiki/CsvUtils
-- Used to escape "'s by toCSV
function escapeCSV (s)
  if string.find(s, '[,"]') then
    s = '"' .. string.gsub(s, '"', '""') .. '"'
  end
  return s
end
-- Convert from CSV string to table (converts a single line of a CSV file)
function fromCSV (s)
  s = s .. ','        -- ending comma
  local t = {}        -- table to collect fields
  local fieldstart = 1
  repeat
    -- next field is quoted? (start with `"'?)
    if string.find(s, '^"', fieldstart) then
      local a, c
      local i  = fieldstart
      repeat
        -- find closing quote
        a, i, c = string.find(s, '"("?)', i+1)
      until c ~= '"'    -- quote not followed by quote?
      if not i then error('unmatched "') end
      local f = string.sub(s, fieldstart+1, i-1)
      table.insert(t, (string.gsub(f, '""', '"')))
      fieldstart = string.find(s, ',', i) + 1
    else                -- unquoted; find next comma
      local nexti = string.find(s, ',', fieldstart)
      table.insert(t, string.sub(s, fieldstart, nexti-1))
      fieldstart = nexti + 1
    end
  until fieldstart > string.len(s)
  return t
end
-- Convert from table to CSV string
function toCSV (tt)
  local s = ""
  -- ChM 23.02.2014: changed pairs to ipairs
  -- assumption is that fromCSV and toCSV maintain data as ordered array
  for _,p in ipairs(tt) do
    s = s .. "," .. escapeCSV(p)
  end
  return string.sub(s, 2)      -- remove first comma
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

if input == nil then
  input = {}
  input["name"] = ""
  input["comment"] = ""
end

-- Fix anonymous visitor
if input.name == nil or string.len(input.name) == 0 then
  input.name = "Ein Besucher"
end

data["comment"] = input

-- add the comment to the DB
local f,err,status = io.open(db, "a")

-- Zitat,Name,Position,Jahr,Anlass(which is 'comment' here)
local entry = escapeCSV(input.comment)..","..escapeCSV(input.name)..",,"..os.date("%Y")..",comment\n"
os.execute("logger -t \"kommentar.user.comment.csv\" '" .. entry .. "'\n")

f:write(entry)
f:close()

-- notify clients to get new DB
require 'socket'
host = "192.168.123.255"
port = 3332
udp, err = socket.udp()
if not udp then print(err) os.exit() end
udp:setoption('broadcast', true)
os.execute("logger -t \"kommentar.network\" 'Sending update beacon to clients'\n")
udp:sendto("update", host, port)

-- pick one client to show the current comment next
local clients = {
  "192.168.123.225",
  "192.168.123.162",
  "192.168.123.155",
  "192.168.123.146",
  "192.168.123.210",
  "192.168.123.193",
}

local clientID = math.floor(math.random() * 5.9)
udp:sendto("showUserComment", clients[clientID], port)
-- also send to jens' machine for debugging
udp:sendto("showUserComment", "192.168.123.207", port)



-- respond back with the current comment
local body = JSON:encode_pretty(data)

os.execute("logger -t \"kommentar.user.comment\" '" .. body .. "'\n")


-- Respond with OK and current comment
local header = [[HTTP/1.0 200 OK
Content-Type: application/json
Cache: none
Connection: close

]]
print(header .. body .. '\n\n')