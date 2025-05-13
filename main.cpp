#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Structure to hold menu info including hidden URL
struct MenuOption {
    int number;
    string name;
    string url; // URL is stored but not shown
};

// Hardcoded menu options with URLs
vector<MenuOption> getEmbeddedMenuOptions() {
    vector<MenuOption> options;

    options.push_back({1, "Apex CKGE Minimal v1.03",
        "https://drive.usercontent.google.com/download?id=1KRNTauNrNuPneSWcmdGtl8b3vWK-jM_a&export=download&authuser=0&confirm=t&uuid=726cb7ac-665d-4897-8e0c-3a577a707467&at=ALoNOglBQclbSmXHW_l3yY_tF83t "});

    options.push_back({2, "Apex CKGE Full v1.03",
        "https://drive.usercontent.google.com/download?id=1Wq4zfk0u_DJx39TYc4RUGvyYvHcMku1q&export=download&authuser=0&confirm=t&uuid=14098db1-fb45-4fad-a954-199a6f13ca06&at=ALoNOglpu2_sXwcLA7JRx34fVnEv "});

    options.push_back({3, "Apex CKHE Minimal v1.0",
        "https://drive.usercontent.google.com/download?id=1Ci16sZZKzwrr3kJbhnOLSwrqXpDUv_7h&export=download&authuser=0&confirm=t&uuid=f194bd17-d9be-4d32-8c75-619bd03a7b9f&at=ALoNOglUgCArlPV-tTMlXRl29jV6 "});

    return options;
}

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

// Display menu options
auto options = getEmbeddedMenuOptions();

for (const auto& option : options) {
    cout << option.number << " " << option.name << endl;
}
cout << "\033[0m";

return 0;
}
