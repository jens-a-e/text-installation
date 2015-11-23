#!/bin/sh
. ./config.sh

response=`lua ./comment.lua`
cat <<EOF
$response
EOF
