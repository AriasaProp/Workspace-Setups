#!/bin/bash

export ll
alias ll='ls -ha'

gitup () {
  git add .
  git commit -m "$1"
  git push
}
