#!/bin/bash

# file manager
alias ll='ls -hAgG --time-style="+[%d/%m/%y %H:%M]"'

cgit () {
  case $1 in
    up)
      if ! git add .; then
        echo "There is no changes to upload. Exit">&2
        exit 1
      fi
      if [[ -n $2 ]]; then
        local gitm="$2";
      else
        local gitm="cGit upload";
      fi
      if ! git commit -m "$gitm" || ! git push; then
        echo "Error on commit/push, details $?">&2
      fi
      ;;
    * | help)
      echo "Custom git command sets"
      echo "Usage: cgit <command> [option]"
      echo ""
      echo " COMMAND    Description"
      echo "help or *   show this message."
      echo "up          upload changes git."
      ;;
  esac
}
cpkg () {
  case $1 in
    li)
      shift 1
      local p=""
      local h=10
      while [[ $# -gt 0 ]]; do
        case $1 in
          -p)
            p=$2
            shift 2
            ;;
          -h)
            h=$2
            shift 2
            ;;
          -*)
            echo "$1 option is not supported.">&2
            shift 1
            ;;
          *)
            echo "$1 need '-' for valid option.">&2
            shift 1
            ;;
        esac
      done
      dpkg-query -f='${Installed-Size}\t${Package}\t${Status}\n' -W $p | sort -nr | awk -f ext/cpkg-li.awk -v N=$h | column -t
      ;;
    up)
      echo "Update packages"
      if ! apt-get update -y; then
        echo "Error on update, details: $? .Checking dependencies...">&2
        if ! apt-get --fix-broken install; then
          echo "Failed to fix broken dependencies. details: $?">&2
          exit 1
        elif ! apt-get update -y; then
          echo "Error on update again, details: $?">&2
          exit 1
        fi
      fi
      echo "Packages updated!"
      if ! apt-get full-upgrade -y; then
        echo "Error on upgrade, details: $?">&2
        exit 1
      fi
      echo "Packages upgraded!"
      ;;
    * | help)
      echo "Custom packages manager"
      echo "Usage: cpkg <command>"
      echo ""
      echo " COMMAND       Description"
      echo "help | *     show this message."
      echo "li           show list of installed packages."
      echo " -p <*>        list of installed packages by pattern."
      echo " -h <0-9>      list of installed packages limit by number (default 10) line."
      echo "up           do update and upgrade all installed packages."
      ;;
  esac
}
cuse () {
  case $1 in
    ram)
      local type='p'
      if [[ -n $2 ]]; then
        type=$2
      fi
      while read -r /proc/meminfo line && [[ -n $line ]]; do
        echo line
      done
      case $2 in
        * | "" | 'p')
          ;;
        'u')
          ;;
      esac
      echo $type
      
      ;;
    * | help)
      echo "Custom memory view command sets"
      echo "Usage: cmem <command> [option]"
      echo ""
      echo " COMMAND    Description"
      echo "help or *   show this message."
      echo "ram         show current usage of RAM."
      echo "  p           show as percentage."
      echo "  u           show as nearest memory byte unit."
      ;;
  esac
}

wellcome_start() {
  date "+%B %d, %Y"
  echo
  echo "To Do : "
  echo " Write .basrc"
}

wellcome_start