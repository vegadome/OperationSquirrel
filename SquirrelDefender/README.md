# Description

This folder contains the code for the Squirrel Defender and most of its dependencies.  Some dependencies for the Jetson Nano or other companion computer are found elsewhere, but instructions for how to set those up will be provided in a different file.  This readme will explain how to compile and run this code as well as provide an outline for how the code is structured.

## Follow the instructions in the scripts folder to set up swap and install the needed dependencies first

## Before compiling (specifically if linking ORB_SLAM)

- Follow the instructions in [Install-dependencies](https://github.com/crose72/OperationSquirrel/blob/dev/scripts/Install-dependencies.md) to copy the contents of `jetson/usr-orin-nano` or the specific library files for your device to the correct path.
- Update the shared library cache with `sudo ldconfig -v` (should be part of the steps for the bullet point above)
- If you need to update the linker path for some reason <https://stackoverflow.com/questions/480764/linux-error-while-loading-shared-libraries-cannot-open-shared-object-file-no-s>

## CMake instructions

First you will want to update CMake to 3.28, the version we are currently using.  The Jetson Nano is arm64 or aarch64 so the appropriate file has been selected.  You only need to do this once.

```
# Remove old version of cmake and install a newer one
	sudo apt-get remove cmake  # or your package manager's equivalent command

# Replace <version> with the version number you downloaded
	wget https://cmake.org/files/v3.28/cmake-3.28.0-linux-aarch64.sh
	chmod +x cmake-3.28.0-linux-aarch64.sh
	sudo ./cmake-3.28.0-linux-aarch64.sh --prefix=/usr/local --exclude-subdir
```

The CMake file allows you to choose the platform you are compiling for.  Currently the supported options are BLD_JETSON_B01 and WSL.  Enable one option at a time:
- `option(BLD_JETSON_B01 "Enable Jetson Nano specific features" ON)`
- `option(WSL "Enable WSL specific features" OFF)`

You must also specify a Debug or Release build.  Debug enables all print statements.  Release disables most print statements to save throughtput for the program:
- `cmake -DCMAKE_BUILD_TYPE=Release ..`

#### Other preprocessing directives will be added to configure the code to enable or disable other features

## How to compile and run the program

1. Configure the CMakeLists.txt file
2. Execute `mkdir build` to create a build directory
3. Execute `cd build` to go to the build folder
4. Execute `cmake ..` to generate build files (debug or release type)
5. Execute `make -j$(nproc)` for fast builds or `make` for slow builds
6. Execute `sudo ./squirreldefender` to run the program
7. Execute `./unit_tests` to run the unit tests
8. If the video doesn't display on your monitor, try executing `sudo systemctl restart nvargus-daemon`

## Automatically run program on Jetson Nano by creating a systemd service

    # Create a .service file (one is already included in this folder so you can just copy that)
    sudo nano /etc/systemd/system/squirrel_defender.service

    # Here's what the file looks like anyways:
        [Unit]
        Description=Squirrel Defender program
        After=network.target nvargus-daemon.service

        [Service]
        Type=simple
        ExecStartPre=/bin/systemctl restart nvargus-daemon.service
        ExecStart=<path-to-exe>/squirreldefender
        WorkingDirectory=<path-to-build-folder>//SquirrelDefender/build
        StandardOutput=journal
        StandardError=journal
        Restart=always
        User=root

        [Install]
        WantedBy=multi-user.target                
    
    # Save the file and reload the daemon
    sudo systemctl daemon-reload

    # Enable the service
    sudo systemctl enable squirrel_defender.service

    # Start the service
    sudo systemctl start squirrel_defender.service
    
    # Restart the service
    sudo systemctl restart squirrel_defender.service

    # Check status of the service
    sudo systemctl status squirrel_defender.service

## Folder structure

- `appsrc` - source code files
- `apphdr` - source header files
- `inc` - header files for external libraries (currently Mavlink is the only one that actually needs to be in here as the rest are installed on the jetson in `/usr/local/include/`, but they are there for reference)
- `lib` - external compiled libraries
- `tests` - header files used to run flight tests, whatever behavior you want to hard code the drone to do (especially useful when testing in WSL with virtual jetson)
- `UnitTests` - all the code for unit tests, including helper files
- `params.json` - parameters defined here are adjustable at runtime (not recommended to use with real vehicle, hardcode all of the parameters when using real vehicle)
- `.editorconfig` - should automatically format the code when saving (if it doesn't then check your settings in vs code)
