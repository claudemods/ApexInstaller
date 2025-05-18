#!/bin/bash

# Function to display colored ASCII art
display_ascii() {
    echo -e "\033[38;2;255;0;0m"  # Set text color to red
    echo -e "░█████╗░██╗░░░░░░█████╗░██╗░░░██╗██████╗░███████╗███╗░░░███╗░█████╗░██████╗░░██████╗"
    echo -e "██╔══██╗██║░░░░░██╔══██╗██║░░░██║██╔══██╗██╔════╝████╗░████║██╔══██╗██╔══██╗██╔════╝"
    echo -e "██║░░╚═╝██║░░░░░███████║██║░░░██║██║░░██║█████╗░░██╔████╔██║██║░░██║██║░░██║╚█████╗░"
    echo -e "██║░░██╗██║░░░░░██╔══██║██║░░░██║██║░░██║██╔══╝░░██║╚██╔╝██║██║░░██║██║░░██║░╚═══██╗"
    echo -e "╚█████╔╝███████╗██║░░██║╚██████╔╝██████╔╝███████╗██║░╚═╝░██║╚█████╔╝██████╔╝██████╔╝"
    echo -e "░╚════╝░╚══════╝╚═╝░░╚═╝░╚═════╝░╚═════╝░╚══════╝╚═╝░░░░░╚═╝░╚════╝░╚═════╝░╚═════╝░"
    echo -e "\033[38;2;0;255;255m"  # Set text color to cyan
    echo -e "claudemods Apex CKGE Installer v1.0"
    echo -e "\033[0m"  # Reset text color
}

# Clear screen and display ASCII art
clear
display_ascii

# Menu options
echo -e "\033[38;2;255;255;0m"  # Set text color to yellow
echo "Select an installation option:"
echo -e "\033[38;2;0;255;0m"    # Set text color to green
echo "1) Default Install To Drive 3 and 4"
echo "2) Btrfs Install To Drive 1 and 2"
echo -e "\033[0m"               # Reset text color

# Read user input
read -p "Enter your choice (1 or 2): " choice

# Process user choice
case $choice in
    1)
        echo -e "\033[38;2;0;255;0m"  # Set text color to green
        echo "Starting Default Install To Drive 3 and 4..."
        echo -e "\033[0m"             # Reset text color
        cd /opt/btrfs-installer/drive3and4 && ./btrfsinstaller
        ;;
    2)
        echo -e "\033[38;2;0;255;0m"  # Set text color to green
        echo "Starting Btrfs Install To Drive 1 and 2..."
        echo -e "\033[0m"             # Reset text color
        cd /opt/btrfs-installer/drive1and2 && ./btrfsinstaller
        ;;
    *)
        echo -e "\033[38;2;255;0;0m"  # Set text color to red
        echo "Invalid option. Please choose 1 or 2."
        echo -e "\033[0m"             # Reset text color
        exit 1
        ;;
esac

exit 0
