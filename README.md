# JC3248W525EN Random AI Artwork Frame

> An autonomous AI art gallery powered by ESP32, ComfyUI, and SDXL - generating unique artwork every 2 minutes on a touchscreen display.

## Features

- **Automatic Generation**: Creates new AI artwork every 2 minutes
- **Touch Interaction**: Tap anywhere to instantly generate new art
- **Randomized Prompts**: Combines 125+ elements (subjects, locations, colors, atmospheres, styles)
- **Memory Efficient**: RGB565 rendering with smart cleanup
- **SPIFFS Storage**: Temporary image caching for smooth display
- **320x480 Display**: Optimized for common ESP32 touchscreens

## Hardware Requirements

- JC3248W525EN

## Dependencies

### Arduino Libraries
```
- chinScreen (display driver)
- WiFi
- HTTPClient
- ArduinoJson
- SPIFFS
```

### Server Stack
- **ComfyUI** (port 8188) - Image generation backend
- **RGB565 Converter Server** (port 8090) - Format conversion
- **Ollama** (port 11434) - Optional: Advanced prompting

## Installation

### 1. Clone Repository
```bash
git clone https://github.com/yourusername/esp32-ai-display.git
cd esp32-ai-display
```

### 2. Configure Settings

Edit `config.h`:

```cpp
// WiFi Configuration
const char* WIFI_SSID = "YourNetwork";
const char* WIFI_PASSWORD = "YourPassword";

// Server Settings
const char* SERVER_IP = "192.168.1.100";
const char* COMFYUI_PORT = "8188";

// Image Settings
const int IMAGE_WIDTH = 320;
const int IMAGE_HEIGHT = 480;
```

### 3. Set Up Server

#### ComfyUI Setup
```bash
# Install ComfyUI
git clone https://github.com/comfyanonymous/ComfyUI
cd ComfyUI
pip install -r requirements.txt

# Download SDXL model
# Place sdxl_complete.safetensors in ComfyUI/models/checkpoints/

# Run ComfyUI
python main.py --listen 0.0.0.0 --port 8188
```

#### RGB565 Converter Server

Create `rgb565_server.py`:

```python
from flask import Flask, send_file
from PIL import Image
import struct
import os
import glob

app = Flask(__name__)

COMFYUI_OUTPUT = "/path/to/ComfyUI/output"
WIDTH, HEIGHT = 320, 480

@app.route('/image.rgb565')
def get_rgb565():
    # Find latest image
    files = glob.glob(os.path.join(COMFYUI_OUTPUT, "*.png"))
    latest = max(files, key=os.path.getctime)
    
    # Load and resize
    img = Image.open(latest)
    img = img.resize((WIDTH, HEIGHT), Image.Resampling.LANCZOS)
    img = img.convert('RGB')
    
    # Convert to RGB565
    rgb565_data = bytearray()
    for pixel in img.getdata():
        r, g, b = pixel
        rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
        rgb565_data.extend(struct.pack('<H', rgb565))
    
    # Save and serve
    output_path = 'latest.rgb565'
    with open(output_path, 'wb') as f:
        f.write(rgb565_data)
    
    return send_file(output_path, mimetype='application/octet-stream')

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8090)
```

Install and run:
```bash
pip install flask pillow
python rgb565_server.py
```

### 4. Upload to ESP32

1. Open `testai-working.ino` in Arduino IDE
2. Select your ESP32 board
3. Install required libraries via Library Manager
4. Click Upload

## Usage

### First Boot
The device will:
1. Initialize SPIFFS
2. Connect to WiFi
3. Generate first random prompt
4. Request image from ComfyUI
5. Download and display artwork

### Interaction
- **Touch Screen**: Generate new image immediately
- **Wait 2 Minutes**: Auto-refresh with new artwork
- **Serial Monitor**: View detailed logs at 115200 baud

### Prompt Randomization

The system creates unique prompts by combining:
- **25 Subjects**: glass bottle, crystal sphere, mystical sword, etc.
- **20 Locations**: mystical forest, mountain peak, crystal cave, etc.
- **20 Colors**: purple galaxy, golden sunset, emerald green, etc.
- **20 Atmospheres**: ethereal lighting, dramatic shadows, soft glow, etc.
- **20 Styles**: photorealistic, fantasy art, oil painting, etc.

**Total combinations**: 4,000,000+ unique prompts

## Configuration Options

### Timing Constants
```cpp
const unsigned long AUTO_REFRESH_INTERVAL_MS = 120000;  // 2 minutes
const unsigned long API_TIMEOUT_MS = 90000;             // 90 seconds
const unsigned long WIFI_CONNECT_TIMEOUT_MS = 15000;    // 15 seconds
```

### Image Settings
```cpp
const int IMAGE_WIDTH = 320;
const int IMAGE_HEIGHT = 480;
const int SAMPLER_STEPS = 20;
const char* NEGATIVE_PROMPT = "blurry, low quality, watermark, text";
```

### Model Configuration
```cpp
const char* CHECKPOINT = "sdxl_complete.safetensors";
const char* OLLAMA_MODEL = "mistral";  // If using Ollama
```

## Troubleshooting

### WiFi Connection Failed
- Verify SSID and password in `config.h`
- Check 2.4GHz network (ESP32 doesn't support 5GHz)
- Ensure network allows device connections

### ComfyUI Timeout
- Increase `API_TIMEOUT_MS` for slower hardware
- Reduce `SAMPLER_STEPS` for faster generation
- Check ComfyUI logs for errors

### Memory Issues
- Ensure SPIFFS is properly initialized
- Monitor free heap with `ESP.getFreeHeap()`
- Reduce image dimensions if needed

### Display Issues
- Verify RGB565 converter is serving correct format
- Check image dimensions match display resolution
- Test display with known-good images

## Performance

- **Image Generation**: 60-120 seconds (depends on hardware)
- **Download Time**: 2-5 seconds (local network)
- **Memory Usage**: ~300KB per image
- **WiFi Range**: Standard 2.4GHz range

## Architecture

```
┌─────────────┐         ┌──────────────┐
│   ESP32     │◄───────►│   ComfyUI    │
│  (Display)  │  WiFi   │   (SDXL)     │
└─────────────┘         └──────────────┘
       │                        │
       │                        ▼
       │                ┌──────────────┐
       └───────────────►│  RGB565      │
          WiFi          │  Converter   │
                        └──────────────┘
```



## Acknowledgments

- [ComfyUI](https://github.com/comfyanonymous/ComfyUI) - Powerful image generation interface
- [LVGL](https://lvgl.io/) - Embedded graphics library
- [Stability AI](https://stability.ai/) - SDXL model



---

**Made with ☕ and 🎨 by [Your Name]**
