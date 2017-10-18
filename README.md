ofxKuKinectV2
=============

An Kuflex's addon for the Kinect V2 sensor. 
It's fork of original ofxKinectV2 addon https://github.com/ofTheo/ofxKinectV2 by Theo Watson,
which is based on the excellent work by the https://github.com/OpenKinect/libfreenect2 team (@JoshBlake @floe and @christiankerl plus others) 

We made this fork to have addon tuned for our (Kuflex) purposes:
- Easy install and work in Windows
- Allows to disable RGB capture
- Allows to get point cloud
- Uses OpenGL based decoding by default
- Supports multiple kinects. 

Notes:
- Requires a USB 3 port on machine. 


## Install driver for Kinect V2 (in Windows)
Our primary goal it to flawlessly use Kinect V2 in Windows. So we explain in details how to install driver in Windows.
This instructions are adopted from the guide at https://github.com/OpenKinect/libfreenect2, please consult it for more details on other OS,
as well as original addon's instructions https://github.com/ofTheo/ofxKinectV2.

For using a Kinect using this addon in Windows, you need set up the appropriate driver for each Kinect device 
by follow this steps 

1. Plug-in Kinect V2.
2. Run zadig-2.3.exe (located in install_win folder)
3. Check menu's checkbox Options -> List All Devices
4. Uncheck menu's checkbox Options -> Ignore Hubs or Composite Parents
5. Select in main window's list of USB devices "Xbox NUI Sensor (Composite Parent)"
6. Select in the list above "Install Driver / Replace Driver" driver "libusbK (v.3.0.7.0)"
7. Click the button "Replace Driver / Install Driver". (Sometimes we need to do it twice, at first Install the Replace)

This steps set up libusbK driver for plugged Kinect V2 device.

You need to repeat them for each connected device to work with addon.

That's all, now you could use addon and apps made using it.

Engoy. For any questions contact us: kukuflex@gmail.com
