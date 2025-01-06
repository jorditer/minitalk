#!/bin/bash

# Start server in background and get its PID directly
./server &
SERVER_PID=$!
sleep 1

# Verify server is running
if ! ps -p $SERVER_PID > /dev/null; then
    echo "Error: Server failed to start"
    exit 1
fi

# Send test messages
./client "$SERVER_PID" "this is a bit of text to test"
./client "$SERVER_PID" "test message this is more and more and more text 1268479109817378"
./client "$SERVER_PID" "test message this is more and more and more text 1268479109817378 more and more!! even moree! we could even put a thousand! 11123334 this approach is not a 100% secure as we don't have bit confirmation, and that would mean to create a struct, handle its data...no way i am doing that if not needed!! The simple wins over the complicated (bueno veremos en la correccion)"

# Wait and cleanup
sleep 1
kill "$SERVER_PID"