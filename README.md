# Facial Recognition Web Enabled Camera (Version 2)
Revision 2 of the Amazon Web Services integrated web camera, which now features the ability to return images captured through the recipients email, not to mention, provide web server functionality, allowing the device to be operated using a public IP address.

## Table of Contents
* [Materials Checklist](#materials-checklist)
* [Configuring the Device](#configuring-the-device)
    * [Package Manager Update](#package-manager-update)
    * [UVC Drivers](#uvc-drivers)
    * [Python OpenSSL Package](#python-openssl-package)
    * [FSWebcam Utility](#fswebcam-utility)
    * [MJPG Streaming Library](#mjpg-streaming-library)
* [Device Construction](#device-construction)
* [Enabling Cloud Functionality](#enabling-cloud-functionality)
    * [Adding Faces to a Collection](#adding-faces-to-a-collection)
    * [Using a JSON File to Load Attributes](#using-a-json-file-to-load-attributes)
    * [Uploading Image to S3 Bucket](#uploading-image-to-s3-bucket)
    * [Search Collection for Face by Image](#search-collection-for-face-by-image)
    * [List Faces in a Collection](#list-faces-in-a-collection)
    * [Deleting a Face in a Collection](#deleting-a-face-in-a-collection)
* [Wrapping It Up](#wrapping-it-up)

## Materials Checklist
* [x] Arduino Yun Rev 2 (x1)
* [x] Lynxmotion Pan and Tilt Kit (x1)
* [x] Full Sized Breadboard (x1)
* [x] 4GB MicroSD Card (x1)
* [x] MicroSD to SD Converter (x1)
* [x] Logitech C270 (x1)
* [x] PIR Motion Sensor w/ Three Outward Facing Female Headers (x1)
* [x] 5V Battery Pack (x1)
* [x] Rechargeable Batteries (x4)
* [x] Red Female/Male Jumper Wires (x1)
* [x] Black Female/Male Jumper Wires (x1)
* [x] Yellow Female/Male Jumper Wires (x1)
* [x] Red Male/Male Jumper Wires (x3)
* [x] Black Male/Male Jumper Wires (x3)
* [x] Yellwo Male/Male Jumper Wires (x2)
* [x] Type B Micro USB Cable (x1)
* [x] 5V DC 2A Wall Adapter (x1)

## Configuring the Device
After one has acquired the various materials needed to start on building the device, it is probably most easiest to start with connecting to the Arduino Yun in order to get setting it up out of the way. To complete this process, first connect the small end of the micro USB cable into the respective port on the Arduino Yun, and the other end into a computer. After this, make sure to wait at least 60 seconds for the Arduino Yun to boot up. This event will be indicated by the blue WLAN LED turning on and/or flashing. In addition, the WiFi network will display a new network, bearing the syntax of Arduino-YUN90XXXXXX. This is the Arduino Yun’s WiFi hotspot, which will be active if the microcontroller has not been previously connected to WiFi (by factory default). Connect to the hotspot, open a browser, and visit arduino.local. If this doesn’t work, then visit 192.168.240.1. 

Once the landing page has been reached, proceed to the configure button in the top right. It is here where the Arduino Yun’s name, password, and/or network connection will be altered, and will be vital to the moment in time when SSH is needed to run updates on the devices MAC Address. In the context of this project, it is recommended that one connects to the same network that their computer is on, to make matters easier later. As an added note, it will also be handy to have explicit access to the router control panel on that particular network, since it will make port forwarding a breeze if you are planning on utilizing WiFi to access the device globally. With this in mind, make all necessary changes, and then proceed to opening up a terminal of choice on the computer. 

In the command line, enter ```ssh root@arduino.local```, followed by Yes to add the Yun as a known host. The program will ask for a password, in which one would pass in the credentials found earlier on the Yun’s configure page. Once successfully SSHed, the user is now in a position to run the necessary updates in order to ensure the projects’ successful operation. These commands can be found as follows:

### Package Manager Update
```sh
opkg update
```

### UVC Drivers
```sh
opkg install kmod-video-uvc
```

### Python-OpenSSL Package
```sh
opkg install python-openssl
```

### FSWebcam Utility
```sh
opkg install fswebcam
```

### MJPG Streaming Library
```sh
opkg install mjpg-streamer
```
 
After all of the necessary software-side upgrades to the Yun have been taken care of, it is of the utmost importance that one checks that the necessary ports have been declared in the uhttpd file in order for the device to be accessible anywhere. To access this file, simply type ‘cd /etc/config/’, followed by ‘vi uhttpd’. Listed at the top of the page should be the listen address(es) that the Arduino Yun has been configured to monitor for traffic. Press the key ‘i’ on your keyboard to switch over to insert mode in the vi editor, and then make your way below the existing port listings. Using the same formatting found above, insert a new http listen address which will correspond to your desired port (for our project, we went with port 8000, though results can vary based on your network environment). Once this file has been edited to your liking, press ‘ESC’ to leave insert mode, and then type ‘:wq!’ to save the file. 

After this feat has been accomplished, one can then proceed to the control panel of their router, to which the device will then need to be assigned a static private IP. Since the onboard WiFi module located on the Yun comes with a MAC address by default, the router can be configured to recognize the Yun everytime it is configured to the same network. This is important, for the reason that ports can be forwarded to the private IP address of choice without any inconsistencies occuring in the near future. The port, once forwarded to the address, can allow the device to be accessed on the router’s public IP, which allows for multiple devices to be hosted on the same public IP, especially if many devices are competing for a wall jack that has more lax rules as far as port restrictions go.

## Device Construction
It is at this point that the construction of the device can begin to occur.  Using the libraries that were available within the Fritzing application, an illustration was created (as seen below) to elaborate on some of the connections between the breadboard and the various components of the device (including the servos of the Lynxmotion pan tilt kit, the Arduino Yun itself, and the 5V battery pack). Not pictured are the Logitech C270 web camera and the PIR motion sensor, which were excluded for the reason that libraries did not exist for these parts. However, to connect these two entities, one only needs to plug the USB camera into the USB slot located on the Arduino, and connect digital pin 7 to the OUT pin on the PIR motion sensor, with GND  going to GND, and 5V to VCC. For the battery pack, make sure to use 4 AA rechargeable batteries, since alkaline batteries will push the voltage amount to an intolerable level. Once this has been accomplished, one can optionally mount the components down to a board for transportation, though this is not something that is required.

![Fritzing Diagram](arduino/www/img/fritzing-diagram.png?raw=true "Facial Recognition Web Enabled Camera Wiring Diagram")

## Enabling Cloud Functionality
To configure the device to operate within a cloud environment (in this case, we chose Amazon Web Services), the following commands were utilized in order to ensure ends were met.

### Adding Faces to a Collection

This AWS CLI command displays the JSON output for the index-faces CLI operation.
Replace the value of collection-id with the name of the collection you want the face to be stored in. Replace the values of Bucket and Name with the Amazon S3 bucket and image file that you used in step 2. The max-faces parameter restricts the number of indexed faces to 1. Remove or change its value to suit your needs.
```sh
aws rekognition index-faces \
  	--image '{"S3Object":{"Bucket":"bucket","Name":"name.jpg"}}' \
  	--collection-id "collectionid" \
  	--max-faces 1 \
      --quality-filter "AUTO" \
      --detection-attributes "ALL" \
      --external-image-id "ImageID"
```

### Using a JSON File to Load Attributes
```sh
aws rekognition index-faces –-cli-input-json file://jsonFileLocation.json
```
 
### Uploading Image to S3 Bucket

Original image name does not matter. Destination image must be named image01.jpg
```sh
aws s3 cp image03.jpg s3://bucketfacescompare/image01.jpg
```
 
### Search Collection for Face by Image

This AWS CLI command displays the JSON output for the search-faces-by-image CLI operation. Replace the value of Bucket with the S3 bucket that you used in step 2. Replace the value of Name with the image file name that you used in step 2. Replace the value of collection-id with the collection you want to search in.
```sh
aws rekognition search-faces-by-image \
	--image '{"S3Object":{"Bucket":"bucket2","Name":"image01.jpg"}}' \
    --collection-id "chinncollectionfaces"
``` 

### List Faces in a Collection

This AWS CLI command displays the JSON output for the list-faces CLI operation. Replace the value of collection-id with the name of the collection you want to list.
```sh
aws rekognition list-faces \
      --collection-id "collectionid" 
```

### Deleting a Face in a Collection

This AWS CLI command displays the JSON output for the delete-collection CLI operation. Replace the value of collection-id with the name of the collection that contains the face you want to delete. Replace the value of face-ids with an array of face IDs that you want to delete.
```sh 
aws rekognition delete-faces --collection-id "collectionid" --face-ids '["fa8f3068-d23f-4bb4-9fc5-a50340c372d9"]'
```

## Wrapping It Up
In order to see the device in action, simply open up a window in Arduino IDE, and paste the code from sketch.ino in. To guarantee that the imports are fully functional, make sure to have the Temboo Python SDK stored on the root of the SD card (along with ‘upload_picture.py’), before transferring it over to the board. One will also need to have possession of a Temboo account, for the reason that this service is used in order to facilitate the image upload process. The parameters specified to upload_picture.py will vary with respect to what AWS/Temboo account one is on, and hence, have been left as variables for the user to fill in. Furthermore, to satisfy the requirement of making the device web functional, one will also be required to make a folder on the root of the SD card called ‘arduino’, with an inner folder, labeled ‘www’ (where the html file ‘index.html’ will then reside). Once the above conditions have been met, ensure that the correct microcontroller and port are selected under the tools menu, place the microSD card back into the microcontroller, and then proceed to verify/upload the sketch.


