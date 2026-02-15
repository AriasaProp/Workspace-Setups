#!/bin/bash

# file manager
alias ll='ls -hAgG --time-style="+[%d/%m/%y %H:%M]"'

ckit () {
  case $1 in
    gup)
      if ! git add .; then
        echo "There is no changes to upload. Exit">&2
        exit 1
      fi
      local gitm="cGit upload";
      if [[ -n $2 ]]; then
        gitm="$2";
      fi
      if ! git commit -m "$gitm" || ! git push; then
        echo "Error on commit/push, details $?">&2
      fi
      ;;
    mkcd)
      local file
      if [[ -z $3 ]]; then
        echo "Filename: "
        read -r file
        if [[ -z $file ]]; then
          echo "Filename cannot be null."
          exit 1
        fi
      else
        file=$3
      fi
      if ! cat "ext/code_template/$2.template" > $file; then
        echo "Failed to create file program"
      fi
      ;;
    pli)
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
    pup)
      if ! apt-get update -y && ! apt-get --fix-broken install && ! apt-get update -y; then
        echo "Error on update, details: $?">&2
        exit 1
      fi
      echo "Packages updated!"
      if ! apt-get full-upgrade -y; then
        echo "Error on upgrade, details: $?">&2
        exit 1
      fi
      echo "Packages upgraded!"
      ;;
    * | help)
      echo "Custom kit command sets"
      echo "Usage: ckit <command> [option]"
      echo ""
      echo " COMMAND       Description"
      echo "help | *       show this message."
      echo "gup            upload changes git."
      echo "pli            show list of installed packages."
      echo "  -p <*>          list of installed packages by pattern."
      echo "  -h <0-9>        list of installed packages limit by number (default 10) line."
      echo "pup            do update and upgrade all installed packages."
      echo "mkcd <l> <f>   make 'Hello World' template base <l> programming language."
      ;;
  esac
}

date "+%d/%m/%Y"
