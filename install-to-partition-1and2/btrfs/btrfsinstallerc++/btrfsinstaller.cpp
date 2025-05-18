#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/stat.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdint>

using namespace std;

#define COLOR_CYAN "\033[38;2;0;255;255m"
#define COLOR_RED "\033[31m"
#define COLOR_RESET "\033[0m"

string exec(const char* cmd) {
    char buffer[128];
    string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw runtime_error("popen() failed!");
    while (fgets(buffer, sizeof buffer, pipe) != NULL) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

void execute_command(const string& cmd) {
    cout << COLOR_CYAN;
    fflush(stdout);
    string full_cmd = "sudo " + cmd;
    int status = system(full_cmd.c_str());
    cout << COLOR_RESET;
    if (status != 0) {
        cerr << COLOR_RED << "Error executing: " << full_cmd << COLOR_RESET << endl;
        exit(1);
    }
}

bool is_block_device(const string& path) {
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf) != 0) return false;
    return S_ISBLK(statbuf.st_mode);
}

void display_header() {
    cout << COLOR_RED;
    cout << R"(
░█████╗░██╗░░░░░░█████╗░██╗░░░██╗██████╗░███████╗███╗░░░███╗░█████╗░██████╗░░██████╗
██╔══██╗██║░░░░░██╔══██╗██║░░░██║██╔══██╗██╔════╝████╗░████║██╔══██╗██╔══██╗██╔════╝
██║░░╚═╝██║░░░░░███████║██║░░░██║██║░░██║█████╗░░██╔████╔██║██║░░██║██║░░██║╚█████╗░
██║░░██╗██║░░░░░██╔══██║██║░░░██║██║░░██║██╔══╝░░██║╚██╔╝██║██║░░██║██║░░██║░╚═══██╗
╚█████╔╝███████╗██║░░██║╚██████╔╝██████╔╝███████╗██║░╚═╝░██║╚█████╔╝██████╔╝██████╔╝
░╚════╝░╚══════╝╚═╝░░░░░░╚═════╝░╚═════╝░╚══════╝╚═╝░░░░░╚═╝░╚════╝░╚═════╝░╚═════╝░
)" << endl;
cout << COLOR_CYAN << "Btrfs System Installer v1.0 Build 18-05-2025" << COLOR_RESET << endl << endl;
}

void show_post_install_menu(const string& root_part, const string& efi_part) {
    while(true) {
        cout << COLOR_RED;
        cout << R"(
╔══════════════════════════════════════╗
║        Post-Install Menu             ║
╠══════════════════════════════════════╣
║ 1. Chroot into installed system      ║
║ 2. Reboot                            ║
║ 3. Exit                              ║
╚══════════════════════════════════════╝
)" << endl;
cout << COLOR_CYAN << "Select option (1-3): " << COLOR_RESET;

int choice;
cin >> choice;
cin.ignore();

switch(choice) {
    case 1:
        execute_command("mount -o subvol=@root " + root_part + " /mnt/root");
        execute_command("mount " + efi_part + " /mnt/root/boot/efi");
        execute_command("mount --bind /dev /mnt/root/dev");
        execute_command("mount --bind /dev/pts /mnt/root/dev/pts");
        execute_command("mount --bind /proc /mnt/root/proc");
        execute_command("mount --bind /sys /mnt/root/sys");
        execute_command("mount --bind /run /mnt/root/run");
        execute_command("chroot /mnt/root /bin/bash");
        break;
    case 2:
        execute_command("umount -a");
        execute_command("reboot");
        return;
    case 3:
        return;
    default:
        cout << COLOR_RED << "Invalid option!" << COLOR_RESET << endl;
}
    }
}

int main() {
    display_header();

    // Get drive input
    string drive;
    cout << COLOR_CYAN << "Enter drive (e.g., /dev/sda): " << COLOR_RESET;
    getline(cin, drive);

    // Get username
    string username;
    cout << COLOR_CYAN << "Enter username: " << COLOR_RESET;
    getline(cin, username);

    // Auto-create partitions
    string efi_part = drive + "1";
    string root_part = drive + "2";

    if (!is_block_device(drive)) {
        cerr << COLOR_RED << "Error: " << drive << " is not a valid block device" << COLOR_RESET << endl;
        return 1;
    }

    // Create partition table
    cout << COLOR_CYAN << "Creating partition table..." << COLOR_RESET << endl;
    execute_command("wipefs --all " + drive);
    execute_command("parted " + drive + " mklabel gpt");
    execute_command("parted " + drive + " mkpart primary fat32 1MiB 551MiB");
    execute_command("parted " + drive + " mkpart primary btrfs 551MiB 100%");
    execute_command("parted " + drive + " set 1 esp on");
    execute_command("partprobe " + drive);

    if (!is_block_device(efi_part) || !is_block_device(root_part)) {
        cerr << COLOR_RED << "Error: Failed to create partitions" << COLOR_RESET << endl;
        return 1;
    }

    // Format partitions
    cout << COLOR_CYAN << "Formatting EFI partition..." << COLOR_RESET << endl;
    execute_command("mkfs.vfat -F32 " + efi_part);

    cout << COLOR_CYAN << "Creating Btrfs filesystem..." << COLOR_RESET << endl;
    execute_command("mkfs.btrfs -f -L ROOT " + root_part);

    // Mount and create subvolumes
    execute_command("mount " + root_part + " /mnt");
    execute_command("btrfs subvolume create /mnt/@");
    execute_command("btrfs subvolume create /mnt/@home");
    execute_command("btrfs subvolume create /mnt/@root");
    execute_command("btrfs subvolume create /mnt/@srv");
    execute_command("btrfs subvolume create /mnt/@cache");
    execute_command("btrfs subvolume create /mnt/@tmp");
    execute_command("btrfs subvolume create /mnt/@log");
    execute_command("mkdir -p /mnt/@/var/lib");
    execute_command("btrfs subvolume create /mnt/@/var/lib/portables");
    execute_command("btrfs subvolume create /mnt/@/var/lib/machines");
    execute_command("umount -l /mnt");

    // Mount subvolumes
    execute_command("mount -o subvol=@ " + root_part + " /mnt");
    execute_command("mkdir -p /mnt/{home,root,srv,tmp,var/{cache,log},var/lib/{portables,machines}}");
    execute_command("mount -o subvol=@home " + root_part + " /mnt/home");
    execute_command("mount -o subvol=@root " + root_part + " /mnt/root");
    execute_command("mount -o subvol=@srv " + root_part + " /mnt/srv");
    execute_command("mount -o subvol=@cache " + root_part + " /mnt/var/cache");
    execute_command("mount -o subvol=@tmp " + root_part + " /mnt/tmp");
    execute_command("mount -o subvol=@log " + root_part + " /mnt/var/log");

    // Create system snapshot
    cout << COLOR_CYAN << "Creating system snapshot..." << COLOR_RESET << endl;
    execute_command("btrfs subvolume snapshot / /home/" + username + "/fullsystem");
    execute_command("sudo btrfs property set -ts /home/" + username + "/fullsystem ro true");
    execute_command("mksquashfs /home/" + username + "/fullsystem /mnt/root/system.sfs -no-duplicates -no-recovery -always-use-fragments -wildcards -xattrs");
    execute_command("unsquashfs -f -d /mnt/root /mnt/root/system.sfs");
    execute_command("rm -rf /mnt/root/system.sfs");

    // Home snapshot
    execute_command("btrfs subvolume delete /home/" + username + "/fullsystem");
    execute_command("rsync -aHAxSr --numeric-ids --info=progress2 /mnt/root/home /mnt/");
    execute_command("rm -rf /mnt/root/home");
    execute_command("chown $USER /mnt/home");
    execute_command("mount " + efi_part + " /mnt/root/boot/efi");

    // GRUB installation
    cout << COLOR_CYAN << "Installing GRUB..." << COLOR_RESET << endl;
    execute_command("mount -o subvol=@root " + root_part + " /mnt/root");
    execute_command("mount " + efi_part + " /mnt/root/boot/efi");
    execute_command("mount --bind /dev /mnt/root/dev");
    execute_command("mount --bind /dev/pts /mnt/root/dev/pts");
    execute_command("mount --bind /proc /mnt/root/proc");
    execute_command("mount --bind /sys /mnt/root/sys");
    execute_command("mount --bind /run /mnt/root/run");

    // ONLY FIXED SECTION - FSTAB GENERATION
    cout << COLOR_CYAN << "Generating fstab..." << COLOR_RESET << endl;
    string fstab_cmd =
    "if ! genfstab -U /mnt > /mnt/root/etc/fstab 2>/dev/null; then "
    "EFI_UUID=$(blkid -s UUID -o value " + efi_part + "); "
    "ROOT_UUID=$(blkid -s UUID -o value " + root_part + "); "
    "cat > /mnt/root/etc/fstab <<'EOF'\n"
    "UUID=${EFI_UUID}  /root/boot/efi  vfat  umask=0077 0 2\n"
    "UUID=${ROOT_UUID}  /          btrfs  subvol=@,compress=zstd 0 0\n"
    "UUID=${ROOT_UUID}  /home      btrfs  subvol=@home,compress=zstd 0 0\n"
    "UUID=${ROOT_UUID}  /root      btrfs  subvol=@root,compress=zstd 0 0\n"
    "UUID=${ROOT_UUID}  /srv       btrfs  subvol=@srv,compress=zstd 0 0\n"
    "UUID=${ROOT_UUID}  /var/cache btrfs  subvol=@cache,compress=zstd 0 0\n"
    "UUID=${ROOT_UUID}  /tmp       btrfs  subvol=@tmp,compress=zstd 0 0\n"
    "UUID=${ROOT_UUID}  /var/log   btrfs  subvol=@log,compress=zstd 0 0\n"
    "EOF\n"
    "fi";
execute_command(fstab_cmd);

execute_command("chroot /mnt/root /bin/bash -c \""
"if ! mountpoint -q /boot/efi; then "
"   echo 'ERROR: /boot/efi not mounted!'; "
"   exit 1; "
"fi; "
"grub-install --target=x86_64-efi "
"--efi-directory=/boot/efi "
"--bootloader-id=GRUB "
"--recheck || { "
"   echo 'GRUB install failed, trying fallback...'; "
"   grub-install --target=x86_64-efi "
"   --efi-directory=/boot/efi "
"   --bootloader-id=GRUB "
"   --removable; "
"}; "
"if command -v efibootmgr >/dev/null; then "
"   efibootmgr --create "
"   --disk " + drive + " "
"   --part 1 "
"   --loader /EFI/GRUB/grubx64.efi "
"   --label 'GRUB'; "
"fi; "
"grub-mkconfig -o /boot/grub/grub.cfg; "
"mkinitcpio -P\"");

// Cleanup
cout << COLOR_CYAN << "Cleaning up..." << COLOR_RESET << endl;
execute_command("umount -l /mnt/home");
execute_command("umount -l /mnt/root");
execute_command("umount -l /mnt/srv");
execute_command("umount -l /mnt/var/cache");
execute_command("umount -l /mnt/tmp");
execute_command("umount -l /mnt/var/log");
execute_command("umount -l /mnt");

// Show post-install menu
show_post_install_menu(root_part, efi_part);

return 0;
}
