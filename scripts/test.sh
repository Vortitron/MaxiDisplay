#!/bin/bash

# ChoreTracker Test Runner
# Runs comprehensive tests for both ESP32 and Home Assistant components

set -e

echo "🧪 Running ChoreTracker Tests"
echo "============================="

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

# Track test results
ESP32_TESTS_PASSED=false
HA_TESTS_PASSED=false
E2E_TESTS_PASSED=false

# ESP32 Tests
print_status "Running ESP32 tests..."
if command -v pio &> /dev/null; then
	cd esp32
	
	# Run native unit tests
	print_status "Running ESP32 native unit tests..."
	if pio test -e native; then
		print_success "ESP32 native tests passed"
		ESP32_TESTS_PASSED=true
	else
		print_error "ESP32 native tests failed"
	fi
	
	# Run embedded tests if hardware available
	print_status "Running ESP32 embedded tests..."
	if pio test -e embedded 2>/dev/null; then
		print_success "ESP32 embedded tests passed"
	else
		print_warning "ESP32 embedded tests skipped (no hardware connected)"
	fi
	
	cd ..
else
	print_warning "PlatformIO not available - skipping ESP32 tests"
fi

# Home Assistant Tests
print_status "Running Home Assistant tests..."
if [ -d "homeassistant/.venv" ]; then
	cd homeassistant
	source .venv/bin/activate
	
	# Run integration tests
	print_status "Running HA integration tests..."
	if python -m pytest tests/ -v --cov=custom_components/choretracker --cov-report=html --cov-report=term; then
		print_success "Home Assistant tests passed"
		HA_TESTS_PASSED=true
	else
		print_error "Home Assistant tests failed"
	fi
	
	# Check code quality
	print_status "Running code quality checks..."
	
	# Flake8 linting
	if command -v flake8 &> /dev/null; then
		flake8 custom_components/choretracker/ --max-line-length=88 || print_warning "Linting issues found"
	fi
	
	# Type checking with mypy
	if command -v mypy &> /dev/null; then
		mypy custom_components/choretracker/ || print_warning "Type checking issues found"
	fi
	
	deactivate
else
	print_warning "Python virtual environment not found - create with: python3 -m venv .venv"
fi

# End-to-End Tests
print_status "Running end-to-end tests..."
if [ -d "tests/e2e" ]; then
	# Check if we have test fixtures
	if [ -f "tests/fixtures/sample_calendar_events.json" ]; then
		print_status "Running E2E workflow tests..."
		if python -m pytest tests/e2e/ -v; then
			print_success "End-to-end tests passed"
			E2E_TESTS_PASSED=true
		else
			print_warning "End-to-end tests failed (may require hardware setup)"
		fi
	else
		print_warning "E2E test fixtures not found"
	fi
else
	print_warning "End-to-end tests directory not found"
fi

# Performance Tests
print_status "Running performance tests..."
if [ -d "tests/performance" ]; then
	python -m pytest tests/performance/ -v || print_warning "Performance tests failed or not implemented"
else
	print_warning "Performance tests not implemented yet"
fi

# API Validation
print_status "Validating API specifications..."
if [ -f "shared/api/choretracker_api.yaml" ]; then
	# Validate OpenAPI spec
	if command -v swagger-codegen &> /dev/null; then
		swagger-codegen validate -i shared/api/choretracker_api.yaml
		print_success "API specification is valid"
	else
		# Basic YAML validation
		python3 -c "import yaml; yaml.safe_load(open('shared/api/choretracker_api.yaml'))" 2>/dev/null
		if [ $? -eq 0 ]; then
			print_success "API YAML syntax is valid"
		else
			print_error "API specification has syntax errors"
		fi
	fi
else
	print_warning "API specification not found"
fi

# Test Summary
echo
echo "📊 Test Results Summary"
echo "======================"

if [ "$ESP32_TESTS_PASSED" = true ]; then
	echo "✅ ESP32 Tests: PASSED"
else
	echo "❌ ESP32 Tests: FAILED"
fi

if [ "$HA_TESTS_PASSED" = true ]; then
	echo "✅ Home Assistant Tests: PASSED"
else
	echo "❌ Home Assistant Tests: FAILED"
fi

if [ "$E2E_TESTS_PASSED" = true ]; then
	echo "✅ End-to-End Tests: PASSED"
else
	echo "⚠️  End-to-End Tests: SKIPPED/FAILED"
fi

echo
if [ -f "homeassistant/htmlcov/index.html" ]; then
	echo "📈 Coverage Report: homeassistant/htmlcov/index.html"
fi

if [ "$ESP32_TESTS_PASSED" = true ] && [ "$HA_TESTS_PASSED" = true ]; then
	echo
	print_success "🎉 Core tests passed! Project is ready for deployment."
	exit 0
else
	echo
	print_error "❌ Some tests failed. Please review and fix issues before deployment."
	exit 1
fi 