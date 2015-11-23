#!/usr/bin/lua
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

-- Import the JSON en-/decoder
local JSON = (loadfile "/root/textinstallation/JSON.lua")()

function write_header_base()
  -- uhttpd.send("Content-Type: application/json\r\n")
  uhttpd.send("Cache: none\r\n")
  uhttpd.send("Connection: close\r\n\r\n")
end

function handle_request(env)
  exectime = os.clock()
  local renv = {
    CONTENT_LENGTH  = env.CONTENT_LENGTH,
    CONTENT_TYPE    = env.CONTENT_TYPE,
    REQUEST_METHOD  = env.REQUEST_METHOD,
    REQUEST_URI     = env.REQUEST_URI,
    PATH_INFO  = env.PATH_INFO,
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

  local send = uhttpd.send

  if env["PATH_INFO"] == "/comment" then
    handle_comment(env,send,recv)
    return
  end

  uhttpd.send("Status: 200 OK\r\n")
  -- uhttpd.send("Status: 302 Temporary moved\r\n")
  -- uhttpd.send("Location: http://master.text:4200\r\n\r\n")
  write_header_base()
  -- os.execute("logger -t \"faker\" '".. dump(env) .. "'\n")
end

function handle_comment(env,send,recv)
  local isNotPost = (env["REQUEST_METHOD"] ~= "POST")
  local request_body = recv()
  if isNotPost or input ~= nil then
    send("Status: 400 Bad Request\r\n")
    send("Content-Type: text/plain\r\n")
    send("Cache: none\r\n")
    send("Connection: close\r\n\r\n")
    local response = {env = env, request_body = request_body}
    send(JSON:encode_pretty(response))
    return
  end

  local input = JSON:decode(request_body)

  if input == nil then
    input = {}
    input["name"] = ""
    input["comment"] = ""
  end

  -- Fix anonymous visitor
  if input.name == nil or string.len(input.name) == 0 then
    input.name = "Ein Besucher"
  end

  local data = {
    comment = input,
    env = env
  }

  send("Status: 200 OK\r\n")
  uhttpd.send("Content-Type: application/json\r\n")
  write_header_base()

  os.execute("logger -t \"faker\" '".. dump(env) .. "'\n")

  send(JSON:encode_pretty(data))

end
