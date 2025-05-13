#!/bin/bash

# Check if an image path is provided
if [ -z "$1" ]; then
    echo "Usage: $0 /path/to/image.img"
    exit 1
fi

IMAGE="$1"

# Check if the image file exists
if [ ! -f "$IMAGE" ]; then
    echo "Error: Image file '$IMAGE' not found."
    exit 1
fi

# Create a temporary mount point
MOUNT_POINT="/mnt/img_mount"
mkdir -p "$MOUNT_POINT"

# Automatically determine offset (for single partition images)
OFFSET=$(($(sudo fdisk -l "$IMAGE" 2>/dev/null | grep "Linux" | awk '{print $2}') * 512))
if [ $? -ne 0 ] || [ -z "$(echo $OFFSET)" ]; then
    echo "Could not determine partition offset. Assuming raw filesystem or single-partition image without partition table."
    OFFSET=0
fi

echo "Mounting image with offset $OFFSET..."

# Mount the image using the offset
sudo mount -o loop,rw,offset=$OFFSET "$IMAGE" "$MOUNT_POINT"

if [ $? -ne 0 ]; then
    echo "Failed to mount image."
    exit 1
fi

echo "Image mounted at $MOUNT_POINT"

# Execute the desired command inside the mounted system
# You may need to bind system directories for chroot to work properly
sudo mount --bind /dev "$MOUNT_POINT/dev"
sudo mount --bind /dev/pts "$MOUNT_POINT/dev/pts"
sudo mount --bind /proc "$MOUNT_POINT/proc"
sudo mount --bind /sys "$MOUNT_POINT/sys"
sudo mount --bind /run "$MOUNT_POINT/run"

echo "Executing /opt/Arch-Systemtool/btrfsrsync in chroot..."
sudo chroot "$MOUNT_POINT" /bin/env PATH=/bin:/usr/bin /opt/Arch-Systemtool/btrfsrsyncps2

# Unmount all bind mounts
sudo umount "$MOUNT_POINT/dev/pts"
sudo umount "$MOUNT_POINT/dev"
sudo umount "$MOUNT_POINT/proc"
sudo umount "$MOUNT_POINT/sys"
sudo umount "$MOUNT_POINT/run"

# Unmount the image
sudo umount "$MOUNT_POINT"

echo "Unmounted and cleaned up."
