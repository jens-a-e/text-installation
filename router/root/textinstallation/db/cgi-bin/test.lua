#!/usr/bin/lua

local path = "/root/textinstallation/db/cgi-bin/state"

print("Content-Type: text/plain; charset=utf-8")
print("")

function printTable( t, prefix)
  if prefix == nil then prefix = "" end
  for key,value in pairs(t) do
    if type(value) == "table" then
      print(prefix .. key..":")
      printTable(value,'\t')
    else
      print(prefix .. key, value)
    end
  end
end

function split(source, delimiters)
  local elements = {}
  local pattern = '([^'..delimiters..']+)'
  string.gsub(source, pattern, function(value) elements[#elements + 1] =     value;  end);
  return elements
end

-- local env = {}
-- for line in io.popen("set"):lines() do
--   envName = line:match("^[^=]+")
--   env[envName] = os.getenv(envName)
-- end
-- printTable(env)

client = os.getenv("REMOTE_ADDR")


function loadState(path)
  if path == nil or path == "" then return end
  local state = {}
  file = io.open(path,"r")
  while true do
    local line = file:read("*line")
    if line == nil then break end
    fields = split(line, ",")
    if table.getn(fields) > 0 then
      state[fields[1]] = {tonumber(fields[2]),tonumber(fields[3])}
    end
  end
  file:close()
  return state
end

function saveState( state,path )
  -- write the state back to the file
  if state == nil or path == nil or path == "" then return end
  file = io.open(path,"w")
  for c,stats in pairs(state) do
    file:write(c,",",tostring(stats[1]),",",tostring(stats[2]),"\n")
    file:flush()
  end
  print("saved state")
  file:close()
end

function updateClient(client)
  local cite, comment = client[1], client[2]

  if comment+cite >= 7 then
    -- reset
    cite,comment = 0,0
  end

  math.randomseed( os.time() )

  local choice = true

  if comment >= 1 then
    choice = true
  elseif cite >= 6 then
    choice = false
  else
    choice = math.random() > 0.5
  end

  if choice then
    -- pick a cite
    cite = cite + 1
  else
    -- pick a comment
    comment = comment + 1
  end

  client[1] = cite
  client[2] = comment
end



state = loadState(path)

print("state")
printTable(state)

if state[client] == nil then
  print("client not known")
  state[client] = {0,0}
end

updateClient(state[client])

print("state")
printTable(state)

saveState(state,path)

-- print('"Wenn ich groß bin, möchte ich ein fliegendes Auto haben.",Luc,,2015,comment')