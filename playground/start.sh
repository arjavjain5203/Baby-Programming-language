#!/bin/bash
set -e

echo "Starting production server..."

# Move to project root (safe even if already there)
cd "$(dirname "$0")"

# Start backend only
cd server
node index.js
