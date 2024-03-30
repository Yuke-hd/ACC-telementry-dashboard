# ACC-telemetry-dashboard
Poor man's simrace dashboard for Assetto Corsa Competizione
![img](/img/IMG_2882.jpg)
## See it in action
[![Preview](https://img.youtube.com/vi/b4wcGJOdnoc/0.jpg)](https://youtu.be/b4wcGJOdnoc)

# Overview
![img](/img/overview.jpg)
# Reqirements
1. Python 3.8
2. VScode + PlatformIO extension

## Hardware
1. Arduino Nano compatiable boards
2. TM1638 LED&KEY module ([Link](https://www.aliexpress.com/item/1005006579759157.html?spm=a2g0o.productlist.main.1.2c7c1Lbp1LbpEF&algo_pvid=3174a045-519d-49f8-9d37-3236dceaf910&algo_exp_id=3174a045-519d-49f8-9d37-3236dceaf910-0&pdp_npi=4%40dis%21AUD%219.44%212.74%21%21%2144.12%2112.79%21%402101e58b17112576469118676e7724%2112000037720701785%21sea%21AU%212033487944%21&curPageLogUid=K0M7I2tTwvw8&utparam-url=scene%3Asearch%7Cquery_from%3A))
3. 8 bit ws2812b led module
4. 3D printed case
5. M3/M4 x 20 or longer screws and nuts

## Wiring
Remove the headers on the TM1638 board, solder wires directly on the soldering pads to save space.

![img](/img/wiring.jpg)
This is base on my pin setting in code, 
```
// TM1638 setup
#define STROBE_TM 4     // strobe = GPIO connected to strobe line of module
#define CLOCK_TM 5      // clock = GPIO connected to clock line of module
#define DIO_TM 7        // data = GPIO connected to data line of module
// WS2812 setup
const int ledPin = 2;
```

# Installation
## Python
In `src/main.py` Change `COM_PORT` to the port used to connect to the dashboard

then
```
pip install -r requirements.txt
```
## Arduino
Use VSCode to upload `src/acc_telementry.cpp`

You can also use Arduino IDE the upload the code, but you need to make some small changes to the code ([example](https://stackoverflow.com/questions/55684371/how-do-i-add-cpp-file-to-an-arduino-project))


# Usage
Connect the dashboard to PC
```
python main.py
```

# Customization
TBA