# B-Hive-code

**COMBINED.INO**

This is the Arduino code for the B-Hive prototype. The code is used to animate the 2 circular ledstrips. Each ledstrip has 33 leds.
There is also a part to connect with ProtoPie

The different animations are put into different functions. Every function has a couple of arguments.

on(int increment, int brightness0, int color)

off(int increment, int color)

pulse(int repeat, int increment, int color)

sonar(int repeat, int pause, int color)

locate(int repeat, int pause, int opening)


locatecolor(int repeat, int pause, int opening, int color)*
*This Function does not work yet.

Colors can be added in the array in row 381.

There is also a pauze() function, that divides a delay into very small delays of 10ms. After each small delay, the arduino reads in the sensors or messages sent from protopie. 


Currently, the code starts with a dim white light. When the user wants to connect with bluetooth, a blue sonar effect starts. If the bluetoothconnection was successfull, a green pulse starts. 
Now we enter the loop(). It starts off with a dim white light, whilst waiting for a message from Protopie. As soon as the message with the tracker location has been received, the next animation starts. This animation points towards the received location.


![image](https://github.com/wibeel/B-Hive-code/assets/125655641/abb796a9-62f0-4561-91aa-dab66c60314b)


**PROTOPIE** 

test controle.pie 
This is the control app used to send commands to the B-Hive and the app.

b-hive nieuw.pie 
This is the actual app, to be opened on the phone of the user.



Some pictures and videos:
![IMG_20221231_102528](https://github.com/wibeel/B-Hive-code/assets/125655641/5f4e09c3-970f-4ae1-b46b-60173c3e04ce)

https://github.com/wibeel/B-Hive-code/assets/125655641/df5d643e-796e-4aac-bd45-9e44f262a641



