#!/bin/bash

# ChoreTracker Clean Script

echo "🧹 Cleaning ChoreTracker Build Artifacts"

# Clean ESP32 build files
rm -rf esp32/.pio/
rm -rf esp32/build/
rm -f esp32/*.bin
rm -f esp32/*.elf
echo "✅ ESP32 build files cleaned"

# Clean Python cache
find . -type d -name "__pycache__" -exec rm -rf {} + 2>/dev/null || true
find . -name "*.pyc" -delete 2>/dev/null || true
rm -rf homeassistant/.pytest_cache/
rm -rf .pytest_cache/
echo "✅ Python cache cleaned"

# Clean test artifacts
rm -rf tests/reports/
rm -rf htmlcov/
rm -f .coverage
echo "✅ Test artifacts cleaned"

echo "🎉 Cleanup completed!" 