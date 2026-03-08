#!/usr/bin/awk

BEGIN {
  print "N Package Size"
}
($3=="install" && NR <= N) { printf "%d %s %.2fM\n", NR, $2, $1/1024 }
END {}
