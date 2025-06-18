# Contributing to ChoreTracker

We welcome contributions to the ChoreTracker project! This guide helps you contribute to both ESP32 firmware and Home Assistant integration components.

## Project Structure

ChoreTracker consists of two main components:
- **ESP32 Firmware** (`esp32/`) - Device-side code for the physical tracker
- **Home Assistant Integration** (`homeassistant/`) - Server-side integration

## Getting Started

### Prerequisites

- **Python 3.8+** for Home Assistant development
- **PlatformIO** for ESP32 development
- **Git** for version control
- **ESP32 DevKit** for hardware testing (optional)

### Setup Development Environment

1. **Clone the repository**:
   ```bash
   git clone <your-fork-url>
   cd ChoreTracker
   ```

2. **Run the setup script**:
   ```bash
   ./scripts/setup.sh
   ```

3. **Install PlatformIO** (for ESP32 development):
   ```bash
   pip install platformio
   ```

## Development Workflow

### ESP32 Development

```bash
cd esp32
pio run              # Build firmware
pio test -e native   # Run tests
pio run -t upload    # Upload to device
```

### Home Assistant Development

```bash
cd homeassistant
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
python -m pytest tests/ -v
```

## Coding Standards

- Use **tabs for indentation**
- Write in **British English**
- **Reuse functions** wherever possible
- **Replace hardcoded values** with named constants
- **Include error handling** and logging
- **Write unit tests** for new functionality

## Testing

Run all tests:
```bash
./scripts/test.sh
```

## Pull Request Process

1. Fork the repository and create a feature branch
2. Make your changes following coding standards
3. Test thoroughly
4. Update documentation if needed
5. Create a Pull Request with clear description

## Questions?

Create a new issue with the "question" label.

Thank you for contributing to ChoreTracker! 🏠✨ 