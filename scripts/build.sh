#!/bin/bash

# ChoreTracker Build Script
# Builds both ESP32 firmware and validates Home Assistant integration

set -e

echo "🔨 Building ChoreTracker Project"
echo "==============================="

# Colours for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

print_status() {
	echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
	echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
	echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
	echo -e "${RED}[ERROR]${NC} $1"
}

# Build ESP32 firmware
print_status "Building ESP32 firmware..."
if command -v pio &> /dev/null; then
	cd esp32
	
	# Clean previous builds
	pio run -t clean
	
	# Build firmware
	pio run
	
	# Check if binary was created
	if [ -f ".pio/build/esp32/firmware.bin" ]; then
		print_success "ESP32 firmware built successfully"
		print_status "Firmware size: $(du -h .pio/build/esp32/firmware.bin | cut -f1)"
	else
		print_error "ESP32 firmware build failed"
		exit 1
	fi
	
	cd ..
else
	print_warning "PlatformIO not available - skipping ESP32 build"
fi

# Validate Home Assistant integration
print_status "Validating Home Assistant integration..."
if [ -d "homeassistant" ]; then
	cd homeassistant
	
	# Check if virtual environment exists
	if [ -d ".venv" ]; then
		source .venv/bin/activate
		
		# Install dependencies if requirements.txt exists
		if [ -f "requirements.txt" ]; then
			pip install -r requirements.txt > /dev/null 2>&1
		fi
		
		# Run basic syntax validation
		python -m py_compile custom_components/choretracker/*.py
		print_success "Home Assistant integration syntax validated"
		
		# Run tests if they exist
		if [ -d "tests" ] && [ -f "tests/test_*.py" ]; then
			python -m pytest tests/ -v --tb=short || print_warning "Some HA tests failed"
		fi
		
		deactivate
	else
		print_warning "Python virtual environment not found - create with: python3 -m venv .venv"
	fi
	
	cd ..
else
	print_warning "Home Assistant directory not found"
fi

# Validate shared components
print_status "Validating shared components..."
if [ -f "shared/api/choretracker_api.yaml" ]; then
	# Basic YAML syntax check
	python3 -c "import yaml; yaml.safe_load(open('shared/api/choretracker_api.yaml'))" 2>/dev/null
	if [ $? -eq 0 ]; then
		print_success "API specification is valid"
	else
		print_error "API specification has syntax errors"
	fi
else
	print_warning "API specification not found"
fi

echo
print_success "🎉 Build process completed!"
echo
echo "📦 Build Artifacts:"
echo "=================="
if [ -f "esp32/.pio/build/esp32/firmware.bin" ]; then
	echo "✅ ESP32 Firmware: esp32/.pio/build/esp32/firmware.bin"
fi
echo "✅ HA Integration: homeassistant/custom_components/choretracker/"
echo "✅ API Docs: shared/api/"
echo
echo "🚀 Next Steps:"
echo "============="
echo "1. Flash firmware: cd esp32 && pio run -t upload"
echo "2. Install HA integration: Copy homeassistant/custom_components/choretracker to HA"
echo "3. Configure devices: Update configuration files" 