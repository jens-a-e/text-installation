#!/usr/bin/lua
-- textinstallation - comment receiver script
-- (c) jens alexander ewald, lea.io, 2015

-------------------- Imports ---------------------------------------------------
local fs = require "nixio.fs"
local JSON = (loadfile "/root/textinstallation/JSON.lua")()
require 'socket'

-------------------- Defaults --------------------------------------------------
local db = "/root/textinstallation/db/comments.db.csv"
local store = "/root/textinstallation/db/comments.csv"
local debugHost = "192.168.123.195"
local maxComments = 20
local debug = false
local commentURL = "http://comment.com/submit"

-- pick one client to show the current comment next
local clients = {
  "192.168.123.225",
  "192.168.123.162",
  "192.168.123.155",
  "192.168.123.146",
  "192.168.123.210",
  "192.168.123.193",
}

local port = 3332
local udp, err = socket.udp()

if not udp then print(err) os.exit() end

-------------------- Helper Functions ------------------------------------------
function dump(o)
   if type(o) == 'table' then
      local s = '{ '
      for k,v in pairs(o) do
         if type(k) ~= 'number' then k = '"'..k..'"' end
         s = s .. '['..k..'] = ' .. dump(v) .. ','
      end
      return s .. '} '
   else
      return tostring(o)
   end
end

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

-------------------- HTTP Helper Functions -------------------------------------
function send_header(mime_type)
  uhttpd.send("Status: 200 OK")
  uhttpd.send("Content-Type: "..mime_type)
  uhttpd.send("\r\n\r\n")
end

function serve_file(path, mime_type)
  send_file("200 OK",path,mime_type)
end

function send_file(status, path, mime_type)
  uhttpd.send("Status: "..status.."\r\n")
  uhttpd.send("Content-Type: "..mime_type.."\r\n")
  uhttpd.send("\r\n\r\n")
  uhttpd.send(fs.readfile(path))
end

function no_cache()
  uhttpd.send("Cache: none\r\n")
  uhttpd.send("Connection: close\r\n\r\n")
end

function send_unauth()
  uhttpd.send("Status: 401 Unauthorized\r\n")
  uhttpd.send("Content-Type: text/plain\r\n\r\n")
  uhttpd.send("Unauthorized!")
end

function bad_request(body)
  uhttpd.send("Status: 400 Bad Request\r\n")
  uhttpd.send("Content-Type: text/plain\r\n")
  no_cache()
  uhttpd.send(body)
end

-------------------- Main Handler ----------------------------------------------

-- Just for testing prupose....
-- function test_handle_request( env )
--   os.execute("logger -t \"text.server\" '".. dump(env) .. "'\n")
--   uhttpd.send("Status: 200 OK\r\n")
--   uhttpd.send("Content-Type: text/plain\r\n")
--   no_cache()
--   uhttpd.send("bla bla...\r\n")
-- end

function handle_request(env)
  -- exectime = os.clock()
  local renv = {
    CONTENT_LENGTH  = env.CONTENT_LENGTH,
    CONTENT_TYPE    = env.CONTENT_TYPE,
    REQUEST_METHOD  = env.REQUEST_METHOD,
    REQUEST_URI     = env.REQUEST_URI,
    PATH_INFO       = env.PATH_INFO,
    SCRIPT_NAME     = env.SCRIPT_NAME:gsub("/+$", ""),
    SCRIPT_FILENAME = env.SCRIPT_NAME,
    SERVER_PROTOCOL = env.SERVER_PROTOCOL,
    QUERY_STRING    = env.QUERY_STRING
  }

  local k, v
  for k, v in pairs(env.headers) do
    k = k:upper():gsub("%-", "_")
    renv["HTTP_" .. k] = v
  end

  local len = tonumber(env.CONTENT_LENGTH) or 0
  local function recv()
    if len > 0 then
      local rlen, rbuf = uhttpd.recv(4096)
      if rlen >= 0 then
        len = len - rlen
        return rbuf
      end
    end
    return nil
  end

  local send = uhttpd.sendc
  local host = env.HTTP_HOST or env.SERVER_NAME or "no-server-found"
  os.execute("logger -t \"text.server\" 'Got Request to: ".. host .. renv.PATH_INFO .. "'\n")
  -- for k,v in pairs(env) do
  --   if type(k) ~= 'number' then k = '"'..k..'"' end
  --   os.execute("logger -t \"text.server.ENV\" '".. k .. ":" .. tostring(v) .. "'\n")
  -- end
  -- for k,v in pairs(env.headers) do
  --   if type(k) ~= 'number' then k = '"'..k..'"' end
  --   os.execute("logger -t \"text.server.HEADERS\" '".. k .. ":" .. tostring(v) .. "'\n")
  -- end
  -- os.execute("logger -t \"text.server\" '".. dump(renv) .. "'\n")

  if renv.PATH_INFO == "/do/comment" then
    return handle_comment(env,send,recv)
  elseif renv.PATH_INFO == "/do/deauth" then
    return send_unauth()
  elseif renv.PATH_INFO == "/jquery.js" then
    return serve_file("/root/textinstallation/http/jquery.js","text/javascript")
  elseif renv.PATH_INFO == "/submit" then
    return serve_file("/root/textinstallation/http/index.html","text/html")
  -- elseif renv.PATH_INFO == "/generate_204" then
    -- os.execute("logger -t \"text.server.debug\" 'Sending 204 ...'\n")
    -- send("Status: 204 No Content\r\n\r\n")
    -- send("Status: 511 Network Authentication Required\r\n")
    -- send("Status: 303 See Other\r\n")
    -- send("Location: http://comments/submit\r\n\r\n")
  else
    -- 511 does not work on Android or Windows...
    -- send("Status: 511 Network Authentication Required\r\n")
    -- 307 is not according to resources found online, but sematically correct
    -- send("Status: 307 Temporary Redirect\r\n")
    -- Seems all clients support 302...
    send("Status: 302 Found\r\n")
    send("Location: "..commentURL.."\r\n\r\n")
  end

  send("Content-Type: text/html\r\n")
  send("Cache: none\r\n")
  send("Connection: Keep-Alive\r\n")
  send("\r\n")
  send("<!doctype html><html><head><meta http-equiv=\"refresh\" content=\"0;url="..commentURL.."\"></head><style>body{background-color:rgb(33,102,137)}</style><body></body></html>")
end

-------------------- Comment Handler -------------------------------------------
function handle_comment(env,send,recv)
  local isNotPost = (env["REQUEST_METHOD"] ~= "POST")

  if isNotPost then
    return bad_request()
  end

  local request_body = recv()
  local input = JSON:decode(request_body or "") or {}
  input["name"]    = input["name"]    or ""
  input["comment"] = input["comment"] or ""

  if input["name"] == "" then
    input["name"] = "Ein Besucher"
  end

  if input["comment"] == "" then
    return bad_request(JSON.encode_pretty({error = "Comment must not be empty"}))
  end

  local status, result = pcall(store_comment,input)

  local data = {
    comment = input
  }

  if debug then
    data["comment"] = env
  end

  send("Status: 200 OK\r\n")
  uhttpd.send("Content-Type: application/json\r\n")
  no_cache()

  send(JSON:encode_pretty(data))

  os.execute("logger -t \"text.server\" 'Got comment".. dump(input) .. "'\n")

end

-------------------- Store Comment ---------------------------------------------
function store_comment(input)

  -- add the comment to the DB
  local f,err,status = io.open(db, "a")

  -- Zitat,Name,Position,Jahr,Anlass(which is 'comment' here)
  local entry = escapeCSV(input.comment)..","..escapeCSV(input.name)..",,"..os.date("%Y")..",comment\n"
  os.execute("logger -t \"kommentar.user.comment.csv\" '" .. entry .. "'\n")

  f:write(entry)
  f:close()

  local commentCount = 0
  for _ in io.lines(db) do
    commentCount = commentCount+1
  end

  os.execute("logger -t \"kommentar.user.comment.csv\" 'Number of comments: " .. commentCount .. "'\n")

  local fout,status,err = io.open(store, "w")
  local line = 0
  local limit = commentCount-maxComments
  for _line in io.lines(db) do
    line = line+1
    if commentCount <= maxComments or line > limit then
      -- copy  data set to new file
      fout:write(_line,"\n")
    end
  end
  fout:write("\n") -- empty line at the end
  fout:close()

  -- udp:setoption('broadcast', true)

  -- pick a random client
  math.randomseed(os.time())
  local clientID = math.floor(math.random() * 5.9)+1

  --- overwrite for testing:
  if debug then
    clientID = 1
  end

  -- send signal to show the last user comment
  for i,client in ipairs(clients) do
    if i == clientID then
      os.execute("logger -t \"kommentar.network\" 'Sending show comment to "..client.."'\n")
      udp:sendto("showUserComment", client, port)
    else
      os.execute("logger -t \"kommentar.network\" 'Sending update beacon to "..client.."'\n")
      udp:sendto("update", client, port)
    end
  end

  -- also send to jens' machine for debugging
  udp:sendto("update", debugHost, port)
  udp:sendto("showUserComment", debugHost, port)
end