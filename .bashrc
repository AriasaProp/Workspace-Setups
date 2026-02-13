#!/bin/bash

alias ll='ls -ha'

cgit () {
  case $1 in
    up)
      git add .
      if [[ -n $2 ]]; then
        local gitm="cGit upload";
      else
        local gitm="$2";
      fi
      git commit -m "$gitm"
      git push
      ;;
    * | help)
      echo "Custom git command sets"
      echo "Usage: cgit <command> [option]"
      echo ""
      echo " COMMAND   OPTION  Description"
      echo "help or *          show this message."
      echo "up                 upload changes git."
      ;;
  esac
}
cpkg () {
  case $1 in
    list)
      echo "do nothing for now."
      ;;
    update)
      echo "Updating package lists..."
      if ! apt update -y; then
        echo "Error on update, details: $? .Checking dependencies..."
        if ! apt --fix-broken install; then
          echo "Failed to fix broken dependencies. details: $?"
          exit 1
        elif ! apt update -y; then
          echo "Error on update again, details: $?"
          exit 1
        fi
      fi
      echo "Packages updated!"
      if ! apt upgrade -y; then
        echo "Error on upgrade, details: $?"
        exit 1
      fi
      echo "Package lists upgrade successfully!"
      ;;
    * | help)
      echo "Custom packages manager"
      echo "Usage: cpkg <command> [option]"
      echo ""
      echo " COMMAND   OPTION  Description"
      echo "help or *          show this message."
      echo "list               show list of installed packages."
      echo "update             do update and upgrade all installed packages."
      echo "clean              free cache that generate or use by packages."
      ;;
  esac
}
