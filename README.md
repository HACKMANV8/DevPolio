
#  Void: Cross-Platform Secure Data Wiping Solution

 **Official Website:** [https://devpoliooo.vercel.app/](https://devpoliooo.vercel.app/)

---

##  Overview

**Void** is a secure, cross-platform data wiping solution designed to **permanently erase user data** from storage devices including **HDDs, SSDs, NVMe drives, USB devices, and Android phones**.

Built on a **Tiny Core Linux (64-bit)** base, Void ensures **speed, portability, and compatibility** even on older hardware.

It complies with **NIST SP 800-88** standards and generates **tamper-proof, verifiable wipe certificates**, promoting **safe recycling** and supporting **India’s circular economy initiatives**.

---

##  Key Features

-  **Full Disk Wipe** — ATA Secure Erase for HDDs, NVMe Secure Erase for SSDs

-  **Android Wipe** — Automated ADB + Fastboot reset for complete data removal

-  **USB Wipe** — File-system signature wiping using the `dd` protocol

-  **Tamper-Proof Certificates** — Verifiable proof of secure erasure

-  **Offline Operation** — Works entirely without internet access

-  **Lightweight** — Tiny Core Linux base for minimal system resource usage

---

##  System Requirements

| Component | Minimum Requirement |

|----------------|-----------------------------|

| Architecture | 64-bit |

| RAM | 1 GB (2 GB recommended) |

| Storage | 1 GB free space |

| Boot Mode | Legacy BIOS or UEFI |

---

##  Installation & Usage Guide

###  For Windows Users

#### Step 1 — Download Files

1. Visit the [official Void website](https://devpoliooo.vercel.app/).

2. Download the latest **Void ISO** file.

#### Step 2 — Create a Bootable USB with Rufus

1. Download and install [**Rufus**](https://rufus.ie).

2. Insert a USB drive (minimum 4 GB).

3. Open **Rufus** and configure:

- **Device:** Select your USB drive

- **Boot selection:** Choose the downloaded `Void.iso`

- **Partition scheme:** Choose `MBR` (Legacy BIOS) or `GPT` (UEFI)

4. Click **Start** and wait for Rufus to finish creating the bootable USB.

#### Step 3 — Boot into Void

1. Insert the USB drive into the target computer.

2. Restart and open the boot menu (`F12`, `Esc`, or `Del` depending on the system).

3. Select your USB device to boot into **Void**.

#### Step 4 — Start Wiping

Once the Void interface loads, select an option:

-  **Wipe Internal Disk (HDD/NVMe)**

-  **Wipe USB Devices**

-  **Wipe Android Devices**

Follow the on-screen prompts.

After completion, Void automatically generates a **wipe certificate**.

---

###  For Linux Users

#### Option 1 — Using the ISO File

1. Download the `Void.iso` file.

2. Identify your USB drive (e.g., `/dev/sdb`).

3. Use the `dd` command to make a bootable USB:

```bash

sudo dd if=Void.iso of=/dev/sdX bs=4M status=progress conv=fsync

 Replace /dev/sdX with your actual USB device path (not a partition like /dev/sdb1).



Reboot your system and select the USB device from the boot menu.



Option 2 — Using the .deb Package



Download the void.deb package from the official website.



Install it using:



sudo dpkg -i void.deb

sudo apt-get install -f





Launch Void with:



void





Follow the interface to securely wipe supported devices.

 Wipe Certificates



After each successful operation, Void creates a digital wipe certificate that includes:



Device name and serial number



Wipe timestamp



Wipe method (ATA, NVMe, ADB, or dd)



Verification checksum



Operator ID (if configured)



These certificates act as tamper-proof audit records, providing verifiable proof of data erasure for organizations and compliance audits.



 Important Notes



 All data will be permanently erased. Back up any critical data before proceeding.



Use Void only on devices you own or are authorized to wipe.



For Android devices:



Enable USB Debugging in Developer Options before connecting.



Ensure the phone is properly recognized by ADB.



Some older HDDs may not support ATA Secure Erase commands.
```
