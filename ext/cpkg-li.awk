#!/usr/bin/awk

BEGIN {
  I=0
  print "N Package Size"
}
($3=="install" && I<N) { printf "%d %s %.2fM\n", ++I, $2, $1/1024 }
END {}
