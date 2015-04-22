#!/bin/sh
. ./config.sh
logger -t "test comment" "lkjkfjhkdjfhk kjhfkj hkjh"

response=`lua ./comment.lua`
cat <<EOF
$response
EOF
