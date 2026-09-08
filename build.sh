#!/usr/bin/env bash
set -e
echo "Creating ./bin directory..."
mkdir -p ./bin

echo "Running RecompModTool..."
./RecompModTool ./mod.toml ./bin

echo "Zipping output file into ./bin..."
zip -j ./bin/dk64_exit_to_isles.zip ./bin/dk64_exit_to_isles.nrm

echo "Complete"