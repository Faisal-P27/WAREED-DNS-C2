#!/bin/bash

# Cleans the database by invoking WareedServer with a clean option.
dbClean() {
    if ./WareedServer -c; then
        echo "Database cleaned successfully."
    else
        echo "Failed to clean the database."
        exit 1
    fi
}

# Compiles the TeamServer component, ensuring the executable is moved appropriately.
compileTeamServer() {
    if [ -d "./TeamServer" ]; then
        (cd "./TeamServer" && make) && mv "./TeamServer/WareedServer" .
        echo "TeamServer compiled successfully."
    else
        echo "TeamServer directory does not exist."
        exit 1
    fi
}

# Compiles the Client component, including cmake configuration and cleanup after compilation.
compileClient() {
    if [ -d "./Client" ]; then
        (cd "./Client" && cmake . && make) && mv "./Client/Ather" .
        (cd "./Client" && make clean)
        echo "Client compiled successfully."
    else
        echo "Client directory does not exist."
        exit 1
    fi
}

# Compiles the Agent and checks for the executable in the expected directory.
compileAndCheckAgent() {
    if [ -d "./Agent" ]; then
        (cd "./Agent" && make BUILD_TYPE=check)
        if [ -f "./Agent/bin/Hams.exe" ]; then
            echo "Agent compilation OK!"
            rm "./Agent/bin/Hams.exe"
        else
            echo "Agent compilation failed: Hams.exe not found."
            exit 1
        fi
    else
        echo "Agent directory does not exist."
        exit 1
    fi
}

# Prepares the system by installing necessary dependencies, then compiles both the TeamServer and Client.
setup() {
    # Check if running as root
    if [ "$(id -u)" != "0" ]; then
        echo "This script must be run as root or with sudo."
        exit 1
    fi

    apt-get update
    apt-get install -y git build-essential apt-utils cmake libfontconfig1 libglu1-mesa-dev libgtest-dev libspdlog-dev libboost-all-dev libncurses5-dev libgdbm-dev libssl-dev libreadline-dev libffi-dev libsqlite3-dev libbz2-dev mesa-common-dev qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools libqt5websockets5 libqt5websockets5-dev qtdeclarative5-dev golang-go qtbase5-dev libqt5websockets5-dev python3-dev libboost-all-dev mingw-w64 nasm
    echo "System setup completed."
    
    compileTeamServer
    compileClient
    compileAndCheckAgent
    echo "Compilation completed. You can now run /Client/Ather."
}

# Main switch case to handle options.
case "$1" in
    dbclean)
        dbClean
        ;;
    compile)
        compileTeamServer
        compileClient
        compileAndCheckAgent
        ;;
    setup)
        setup
        ;;
    checkAgent)
        compileAndCheckAgent
        ;;
    *)
        echo -e "Usage: $0 OPTION\n\nOptions:\n  dbclean\tCleans the database using WareedServer.\n  compile\tCompiles both the TeamServer and Client components.\n  setup\t\tPrepares the system by installing dependencies and then compiles all components.\n  checkAgent\tCompiles the Agent and checks for the executable Hams.exe."
        exit 1
        ;;
esac

exit 0
