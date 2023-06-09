# ChacalMQTT

![image](https://user-images.githubusercontent.com/72506988/236503946-a294c78b-9646-4410-84c8-7109cb17a051.png)

## Prerequisites
- QT Creator 
- Linux 

## Objectives  
Creation of 2 projects:

### Drone
Create a console program with QT in C++ which loads the image file ```DroneIMG37337.png``` and adds to it, via a steganography algorithm of your choice other than steghide, the following GPS coordinates : 37°42'41.9"S 144°59' 33.0"E.  
The file will be sent to the broker :
- mqtt://broker.emqx.io:1883
- topic/ynov/bordeaux/ChacalMQTT
- QoS 2

### Base
Create a graphical program with QTDesign in C++ which connects to the broker same broker than Drone project.  
Once the file has been retrieved, the image should be displayed in the graphical interface, along with the GPS coordinates encrypted in the image.

---

## Constraints  
- No commented code in release branch
- Respect the imposed PAQL file

--- 

## Images 
- ```DroneIMG37337.png``` in /Drone/img/  
Original image, before encrypting the GPS coordinates
- ```DroneIMG37337_with_gps.png``` in /  
After the encryption of the GPS coordinates, generated and to be sent to the broker by Drone project  
- ```received_image.png``` in /  
Received and saved, read and decrypted by the Base project  

```DroneIMG37337_with_gps.png``` and ```received_image.png``` are not necessary when creating the project since they are generated by the programs. Only ```DroneIMG37337.png``` is required from the start

---

## Demonstrations
![image](https://user-images.githubusercontent.com/72506988/236677713-7503023d-daa0-4197-b80b-403e1490a35e.png)
![image](https://user-images.githubusercontent.com/72506988/236677723-5329eb88-2789-40b7-8831-71d0a5f0cfed.png)