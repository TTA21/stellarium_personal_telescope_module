# Stellarium personal telescope controller

This repo is for personal use, but it might give someone a good idea so its left public.
The repo has two projects, a esp32 firmware that controls the motors Azimuth, Altitude, Field Rotator and a Focuser.
The firmware recieves a serial input string.

The module sends strings to the esp32, it has a control panel that sets configurations for the esp32 firmware, and simple nudge and tracking.

Dont forget to put in your bashrc:

export STELROOT=/home/tta21/Documents/stellarium

To build:

rm -rf ~/your_path/stellarium_personal_telescope_module/build
mkdir -p ~/your_path/stellarium_personal_telescope_module/build/unix
cd ~/your_path/stellarium_personal_telescope_module/build/unix
cmake ../..
make -j$(nproc)