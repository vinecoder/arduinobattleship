# arduinobattleship
An experimental battleship game running on arduino uno.


# INSTALL

# Install Python
sudo apt-get install python scons

# install the latest version of PlatformIO
pip install platformio

# then run from the current directory
platformio run

# EXTRAS

# if you don't have enabled auto-uploading, then you can upload firmware manually
platformio run --target upload

# to clean cached/compiled files
platformio run --target clean


# BOARD CONFIGURATION:

  We have two Led matrix, the first is connected on the second, and the first have the configuration: 
    - data ate the port 6
    - cls at the port 5
    - clock at port 6
  Button A 
    - at the port 8
  Button B 
    - at the port 9
  Button C 
    - at the port 10