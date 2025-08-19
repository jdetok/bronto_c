# installs (mac) necessary to build source & upload
- should just need avr toolchain to run & upload from command line - no arduino IDE/framework necessary
- once installed, use the [build documentation](./build.md) to build & uplaod the code to arduino
# commands to install avr
```
brew install avrdude
brew tap osx-cross/avr
brew install avr-gcc
```
# make c_cpp_properties file for vscode
- necessary so debug recognizes the atmega register names etc
```json
{
    "configurations": [
        {
            "name": "AVR",
            "includePath": [
                "${workspaceFolder}",
                "/opt/homebrew/Cellar/avr-gcc/13.2.0/avr/include",
                "/opt/homebrew/Cellar/avr-gcc/13.2.0/lib/gcc/avr/13/include"
            ],
            "defines": [
                "__AVR_ATmega328P__",
                "F_CPU=16000000UL"
            ],
            "compilerPath": "/opt/homebrew/bin/avr-gcc",
            "cStandard": "c11",
            "intelliSenseMode": "gcc-x64"
        }
    ],
    "version": 4
}
```