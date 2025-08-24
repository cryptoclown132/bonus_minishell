#!/bin/bash

PID=$1   # Prozess-ID als Argument übergeben

if [ -z "$PID" ]; then
  echo "Usage: $0 <PID>"
  exit 1
fi

while true; do
  COUNT=$(lsof -p "$PID" 2>/dev/null | wc -l)
  lsof -p "$PID"
  echo "$(date '+%H:%M:%S') - offene FDs: $COUNT"
  sleep 5
done
