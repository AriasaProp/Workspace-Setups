#!/bin/bash

# file manager
alias ll='ls -hAgG --time-style="+[%d/%m/%y %H:%M]"'
# root directory
root=$(dirname "${BASH_SOURCE[0]}")
__ckit_help_msg__ () {
  __log__ "Custom kit command sets"
  __log__ "Usage: ckit <command> [option]"
  __log__ ""
  __log__ " COMMAND       Description"
  __log__ "help | *       show this message."
  __log__ "gup            upload changes git."
  __log__ "mkcd <l> <f>   make 'Hello World' template base <l> programming language."
}
__cpkg_help_msg__ () {
  __log__ "Custom pkg command sets"
  __log__ "Usage: cpkg <command> [option]"
  __log__ ""
  __log__ " COMMAND      Description"
  __log__ "help | *      show this message."
  __log__ "li            show list of packages."
  __log__ "  -p <*>        list by pattern."
  __log__ "  -h <0-9>      show h (default 10) line."
  __log__ "up            update and upgrade packages."
}
__log__ () {
  case $1 in
    E)
      shift 1
      echo -e "\033[0;31m $@\033[0m">&2
      exit 1
      ;;
    e)
      shift 1
      echo -e "\033[0;31m $@\033[0m">&2
      ;;
    w)
      shift 1
      echo -e "\033[1;33m $@\033[0m">&2
      ;;
    *)
      echo "$@"
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
            __log__ w "$1 option is not supported."
            shift 1
            ;;
          *)
            __log__ w "$1 need '-' for valid option."
            shift 1
            ;;
        esac
      done
      local list
      list=$( dpkg-query -f='${Installed-Size}\t${Package}\t${Status}\n' -W $p )
      list=$( sort -nr <<< $list )
      list=$( awk -f ext/cpkg-li.awk -v N=$h <<< $list )
      column -t <<< $list
      ;;
    up)
      if ( apt-get update -y || ( apt-get --fix-broken install && apt-get update -y ) ); then
        __log__ "Packages updated!"
        if apt-get full-upgrade -y; then
          __log__ "Packages upgraded!"
        else
          __log__ e "Error on upgrade, details: $?"
        fi
      else
        __log__ e "Error on update, details: $?"
      fi
      ;;
    help) __cpkg_help_msg__;;
    *)
      if [[ $# -gt 0 ]]; then
        __log__ w "$1 is unknown command in cpkg"
      else
        __log__ w "Need a command"
      fi
      __cpkg_help_msg__
      ;;
  esac
}

ckit () {
  case $1 in
    gup)
      if ! git add .; then
        __log__ E "There is no changes to upload. Exit"
      fi
      local gitm="cGit upload";
      if [[ -n $2 ]]; then
        gitm="$2";
      fi
      if ! git commit -m "$gitm"; then
        __log__ E "Error on commit, details $?"
      fi
      if ! git push; then
        __log__ e "Error on push, details $?"
      fi
      ;;
    mkcd)
      local file
      if [[ -z $3 ]]; then
        read -p "Filename: " -r file
        if [[ -z $file ]]; then
          __log__ E "Filename cannot be null."
        fi
      else
        file=$3
      fi
      if ! cat "$root/ext/$2.template" > $file; then
        __log__ e "Failed to create file program"
      fi
      ;;
    sched) "$root/ext/sched.bash" ;;
    mem) $root/exec/meminf;;
    help) __ckit_help_msg__;;
    *)
      if [[ $# -gt 0 ]]; then
        __log__ w "$1 is unknown command"
      else
        __log__ w "Need at least a command."
      fi
      __ckit_help_msg__
     ;;
  esac
}

clear

date "+%d/%m/%Y"
ckit sched
