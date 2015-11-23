#!/bin/sh /etc/rc.common

START=99
STOP=99
USE_PROCD=1
QUIET=""
EXTRA_COMMANDS="kill"

DIR="/root/textinstallation"

UHTTPD_BIN="/usr/sbin/uhttpd"
HTTP_ROOT="${DIR}/http"
HTTP_ERROR_URL="/error.html"
HTTP_LISTEN="0.0.0.0:80"
# HTTP_LISTEN="192.168.182.1:80"
DB_HTTP_LISTEN="0.0.0.0:4200"
DB_HTTP_ROOT="${DIR}/db"

DNSMASQ_BIN="/usr/sbin/dnsmasq"
DNSMASQ_CONF="${DIR}/etc/dnsmasq.conf"

start_db_httpd()
{
  echo "Starting DB http instance..."
  procd_open_instance
  procd_set_param respawn ${respawn_threshold:-3600} ${respawn_timeout:-5} ${respawn_retry:-5}

  procd_set_param command "$UHTTPD_BIN" -f

  procd_append_param command -h "${DB_HTTP_ROOT}"
  procd_append_param command -r master
  procd_append_param command -t 60
  procd_append_param command -T 30
  procd_append_param command -k 10
  procd_append_param command -A 1
  procd_append_param command -n 10
  procd_append_param command -N 100
  procd_append_param command -p "${DB_HTTP_LISTEN}"

  procd_close_instance
  sleep 1 && service_check $UHTTPD_BIN
}

start_spot_httpd()
{
  echo "Starting captive http instance..."
  procd_open_instance
  procd_set_param respawn ${respawn_threshold:-3600} ${respawn_timeout:-5} ${respawn_retry:-5}

  procd_set_param command "$UHTTPD_BIN" -f

  procd_append_param command -h "${HTTP_ROOT}"
  procd_append_param command -r master
  procd_append_param command -l ""
  procd_append_param command -L "${DIR}/main.lua"
  procd_append_param command -t 60
  procd_append_param command -T 30
  procd_append_param command -k 10
  procd_append_param command -A 1
  procd_append_param command -n 10
  procd_append_param command -N 100
  # procd_append_param command -E "${HTTP_ERROR_URL}"
  procd_append_param command -p "${HTTP_LISTEN}"

  procd_close_instance
  sleep 1 && service_check $UHTTPD_BIN
  # sleep 1 && service_check $UHTTPD_BIN || \
  #   $UHTTPD_BIN -f $UHTTPD_ARGS
}

start_dnsmasq()
{
  echo "Starting catchall DNS & DHCP server"
  procd_open_instance
  procd_set_param respawn ${respawn_threshold:-3600} ${respawn_timeout:-5} ${respawn_retry:-5}
  procd_set_param command "$DNSMASQ_BIN" -C "$DNSMASQ_CONF"
  procd_close_instance
}

# service_triggers()
# {
#   procd_add_reload_trigger "network"
# }

start_service()
{
  pgrep -f /root/textinstallation
  [ "$?" -eq 0 ] && stop_service
  start_db_httpd
  start_spot_httpd
  start_dnsmasq
}

# reload_service()
# {
#   restart
# }

# restart()
# {
#   stop
#   start_service
# }
# 

kill()
{
  echo "Killing all related services"
  for pid in `pgrep -f /root/textinstallation`; do
    [ "$pid" -ne "$$" ] && kill -9 $pid
  done
  sleep 3
  echo "Related services stopped"
}