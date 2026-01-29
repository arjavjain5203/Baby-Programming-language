#!/bin/bash

# Kill any existing processes on ports 3001 or 5173
fuser -k 3001/tcp
fuser -k 5173/tcp


# Ensure we are in the correct directory (relative to the script having been run from project root)
cd "$(dirname "$0")"

echo "Starting Backend Server..."
cd server
node index.js &
SERVER_PID=$!
cd ..

echo "Starting Frontend Client..."
cd client
npm run dev &
CLIENT_PID=$!

echo "Playground is running!"
echo "Backend: http://localhost:3001"
echo "Frontend: http://localhost:5173"
echo "Press Ctrl+C to stop."

trap "kill $SERVER_PID $CLIENT_PID; exit" INT
wait
