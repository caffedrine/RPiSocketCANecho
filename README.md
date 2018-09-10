## Socket CAN Echo

Simple application which replay with echo to data received on can port.

Tested on Raspbian Stretch with RPi 3B+. The CAN adapter used: MCP2515_CAN

Module required some modifications:
https://vimtut0r.com/2017/01/17/can-bus-with-raspberry-pi-howtoquickstart-mcp2515-kernel-4-4-x/

SPIdev configuration:

On **/boot/config.txt**:

```bash
dtparam=spi=on
dtoverlay=mcp2515-can0,oscillator=16000000,interrupt=25
dtoverlay=spi1-1cs
```

Restart then:

```bash
# Set up interface
sudo ip link set can0 up type can bitrate 500000

# Check settings
ifconfig can0
```

To test link use:

```bash
# Send a message with ID 127 on can0
cansend can0 127#AABBCCDDEEFFAABB

# See traffing on the bus
candump can0
```