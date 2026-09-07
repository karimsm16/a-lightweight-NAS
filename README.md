# ESP32 Lightweight NAS

A lightweight **Network Attached Storage (NAS)** built with an ESP32 and an SD card.

This project allows you to manage files stored on an SD card directly from your web browser without installing any software.

<img width="1042" height="772" alt="Capture d&#39;écran 2026-07-26 184147" src="https://github.com/user-attachments/assets/f9664b5d-aac6-4dda-859a-6d175af194ad" />

---

## Features

- Full file management
  - Upload files
  - Download files
  - Delete files
  - Rename files
  - Create folders

-  Live storage monitoring

- Web-based interface
  - Accessible from any device connected to the same network
  - No software installation required

-  Lightweight and fast
   - Runs entirely on an ESP32
   - Low memory footprint



---

## Hardware

- ESP32 Development Board
- Micro SD Card Module
- Micro SD Card

**[View Bill of Materials (BOM)](./bom.csv)**

### Wiring Diagram

[ESP32 NAS Wiring Diagram](./esp32NAS.pdf)



---

## Software

- PlatformIO
- Arduino Framework


---


## Getting Started

1. Clone this repository.

```bash
git clone https://github.com/karimsm16/a-lightweight-NAS.git
```

2. Open the project using PlatformIO.

3. Configure your own Wi-Fi credentials.
4. <img width="504" height="92" alt="Capture d&#39;écran 2026-08-04 003110" src="https://github.com/user-attachments/assets/970440c1-3b92-4d63-9597-0bbb5afd82e0" />


5. copy the DATA folder files to your SD card 

6. flash the ESP32

7. Open the IP address displayed in the Serial Monitor.



## Why this project?

The goal of this project is to build a lightweight NAS using inexpensive hardware and to abriviate the proccess of  opening , and reopening your phone slot and searching a USB card adapter just to transfer files.


## Demo video :
[Click here to watch the project demo](https://drive.google.com/file/d/15h1bdfyv1-6w5vuqf7vd_GTKVhY_gMv8/view?usp=sharing)
