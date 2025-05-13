#include <iostream>
#include <cstdlib> // For system() command

using namespace std;

int main() {
    // Your exact ASCII art - completely unchanged
    cout << "\033[38;2;255;0;0m";
    cout << R"(░█████╗░██╗░░░░░░█████╗░██╗░░░██╗██████╗░███████╗███╗░░░███╗░█████╗░██████╗░░██████╗
██╔══██╗██║░░░░░██╔══██╗██║░░░██║██╔══██╗██╔════╝████╗░████║██╔══██╗██╔══██╗██╔════╝
██║░░╚═╝██║░░░░░███████║██║░░░██║██║░░██║█████╗░░██╔████╔██║██║░░██║██║░░██║╚█████╗░
██║░░██╗██║░░░░░██╔══██║██║░░░██║██║░░██║██╔══╝░░██║╚██╔╝██║██║░░██║██║░░██║░╚═══██╗
╚█████╔╝███████╗██║░░██║╚██████╔╝██████╔╝███████╗██║░╚═╝░██║╚█████╔╝██████╔╝██████╔╝
░╚════╝░╚══════╝╚═╝░░░░░░╚═════╝░╚═════╝░╚══════╝╚═╝░░░░░╚═╝░╚════╝░╚═════╝░╚═════╝░)" << endl;
    cout << "\033[38;2;0;255;255m";
    cout << "claudemods Apex Installer v1.0\n\n";
    
    // Display installer options
    cout << "\033[38;2;255;255;0m"; // Yellow color for options
    cout << "1. BTRFS Installer \n";
    cout << "2. EXT4 Installer \n";
    cout << "\033[0m"; // Reset color
    
    // Get user choice
    int choice;
    cout << "\nEnter your choice (1 or 2): ";
    cin >> choice;
    
    // Execute the appropriate command
    switch(choice) {
        case 1:
            system("sudo /opt/Arch-Systemtool/btrfsrsncps2");
            break;
        case 2:
            system("sudo /opt/Arch-Systemtool/ext4ps2");
            break;
        default:
            cout << "\033[38;2;255;0;0mInvalid choice!\033[0m\n";
            return 1;
    }
    
    return 0;
}
