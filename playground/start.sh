#!/bin/bash
set -e

echo "Starting fullstack app..."

# Go to project root
cd "$(dirname "$0")"

echo "Building frontend..."
cd client
npm install
npm run build
cd ..

echo "Installing backend deps..."
cd server
npm install

echo "Starting backend (serves frontend too)..."
node index.js
