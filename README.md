Arduino Yun Snapshot Camera (Version 2)
==============================================================
Revision 2 of the Amazon Web Services integrated web camera, which now features the ability to return images captured through the recipients email, not to mention, provide web server functionality, allowing the device to be operated using a public IP address.

Source Code
--------------------------------------------------------------
To operate the device under it's new configuration:
1. The updated .ino file found above within the 'snapshotCamera' folder needs to be uploaded to the Arduino, which will allow for it to listen in on incoming motion data, as well as any data sent through the client via the web server hosted on the Yun itself.
2. In addition, a new folder (designated 'www') needs to be added to the root of the microSD card. This folder will contain an additional folder named 'arduino', which will in turn hold the .html file fundamental to hosting the web server abroad on the public Internet.

NOTE: It is also worth mentioning that 'upload_picture.py' and the Temboo Python SDK (https://temboo.com/python) have both been left as before, so no change is required in order to ensure adequate compliance with the rest of the source code.


