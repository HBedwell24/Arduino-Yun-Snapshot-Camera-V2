Arduino Yun Snapshot Camera (Version 2)
==============================================================
Revision 2 of the Amazon Web Services integrated web camera, which now features the ability to return images captured through the recipients email, not to mention, provide web server functionality, allowing the device to be operated using a public IP address.

Materials Checklist
------------------------------------------------------------------
To assemble the device, the hardware for assembly will require that one has possession of an Arduino Yun Rev 2, a Lynxmotion pan and tilt kit, a full sized breadboard, a microSD with a variable amount of memory (the size is up to the user), a microSD to SD converter, a UVC compatible camera (for our project, we utilized a Logitech C270), a PIR motion sensor with 3 outward facing female headers, a 5V battery pack, 4 rechargeable batteries, at least a dozen Female to Male Jumper Wires (in order to fulfill the connections between the motion sensor and the Yun microcontroller), and a least a dozen of Male to Male cables (which will serve as the connection between the Arduino to the full sized breadboard, not to mention help other components such as the battery pack provide amperage needed to power servos found in the pan and tilt kit). It would also be beneficial to have a type B micro USB cable or a 5V DC 2A wall adapter (also Micro-USB type B) on hand for when the sketch upload takes place to the Arduino, as well as for installing updates on the microcontroller via SSH. 

Configuring the Device
------------------------------------------------------------------
After one has acquired the various materials needed to start on building the device, it is probably most easiest to start with connecting to the Arduino Yun in order to get setting it up out of the way. To complete this process, first connect the small end of the micro USB cable into the respective port on the Arduino Yun, and the other end into a computer. After this, make sure to wait at least 60 seconds for the Arduino Yun to boot up. This event will be indicated by the blue WLAN LED turning on and/or flashing. In addition, the WiFi network will display a new network, bearing the syntax of Arduino-YUN90XXXXXX. This is the Arduino Yun’s WiFi hotspot, which will be active if the microcontroller has not been previously connected to WiFi (by factory default). Connect to the hotspot, open a browser, and visit arduino.local. If this doesn’t work, then visit 192.168.240.1. Once the landing page has been reached, proceed to the configure button in the top right. It is here where the Arduino Yun’s name, password, and/or network connection will be altered, and will be vital to the moment in time when SSH is needed to run updates on the devices MAC Address. In the context of this project, it is recommended that one connects to the same network that their computer is on, to make matters easier later. As an added note, it will also be handy to have explicit access to the router control panel on that particular network, since it will make port forwarding a breeze if you are planning on utilizing WiFi to access the device globally. With this in mind, make all necessary changes, and then proceed to opening up a terminal of choice on the computer. 

In the command line, enter ssh root@arduino.local (or root@yourarduinoname.local if the name of the Yun was changed in an earlier step), followed by Yes to add the Yun as a known host. The program will ask for a password, in which one would pass in the credentials found earlier on the Yun’s configure page. Once successfully SSHed, the user is now in a position to run the necessary updates in order to ensure the projects’ successful operation. These commands can be found as follows:

package manager update:
```sh
opkg update
```

UVC drivers:
```sh
opkg install kmod-video-uvc
```

Python-openssl package:
```sh
opkg install python-openssl
```

fswebcam utility:
```sh
opkg install fswebcam
```

mjpg streaming library:
```sh
opkg install mjpg-streamer
```
 
After all of the necessary software-side upgrades to the Yun have been taken care of, it is of the utmost importance that one checks that the necessary ports have been declared in the uhttpd file in order for the device to be accessible anywhere. To access this file, simply type ‘cd /etc/config/’, followed by ‘vi uhttpd’. Listed at the top of the page should be the listen address(es) that the Arduino Yun has been configured to monitor for traffic. Press the key ‘i’ on your keyboard to switch over to insert mode in the vi editor, and then make your way below the existing port listings. Using the same formatting found above, insert a new http listen address which will correspond to your desired port (for our project, we went with port 8000, though results can vary based on your network environment). Once this file has been edited to your liking, press ‘ESC’ to leave insert mode, and then type ‘:wq!’ to save the file. 

After this feat has been accomplished, one can then proceed to the control panel of their router, to which the device will then need to be assigned a static private IP. Since the onboard WiFi module located on the Yun comes with a MAC address by default, the router can be configured to recognize the Yun everytime it is configured to the same network. This is important, for the reason that ports can be forwarded to the private IP address of choice without any inconsistencies occuring in the near future. The port, once forwarded to the address, can allow the device to be accessed on the router’s public IP, which allows for multiple devices to be hosted on the same public IP, especially if many devices are competing for a wall jack that has more lax rules as far as port restrictions go.

Device Construction
------------------------------------------------------------
It is at this point that the construction of the device can begin to occur.  Using the libraries that were available within the Fritzing application, an illustration was created (as seen below) to elaborate on some of the connections between the breadboard and the various components of the device (including the servos of the Lynxmotion pan tilt kit, the Arduino Yun itself, and the 5V battery pack). Not pictured are the Logitech C270 web camera and the PIR motion sensor, which were excluded for the reason that libraries did not exist for these parts. However, to connect these two entities, one only needs to plug the USB camera into the USB slot located on the Arduino, and connect digital pin 7 to the OUT pin on the PIR motion sensor, with GND  going to GND, and 5V to VCC. For the battery pack, make sure to use 4 AA rechargeable batteries, since alkaline batteries will push the voltage amount to an intolerable level. Once this has been accomplished, one can optionally mount the components down to a board for transportation, though this is not something that is required.

![Fritzing Diagram](fritzing-diagram.png?raw=true "Facial Recognition Web Enabled Camera Wiring Diagram")

Source Code
--------------------------------------------------------------
To operate the device under it's new configuration:
1. The updated .ino file found above within the 'snapshotCamera' folder needs to be uploaded to the Arduino, which will allow for it to listen in on incoming motion data, as well as any data sent through the client via the web server hosted on the Yun itself.
2. In addition, a new folder (designated 'www') needs to be added to the root of the microSD card. This folder will contain an additional folder named 'arduino', which will in turn hold the .html file fundamental to hosting the web server abroad on the public Internet.

NOTE: It is also worth mentioning that 'upload_picture.py' and the Temboo Python SDK (https://temboo.com/python) have both been left as before, so no change is required in order to ensure adequate compliance with the rest of the source code.


