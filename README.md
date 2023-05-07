# ChacalMQTT

![image](https://user-images.githubusercontent.com/72506988/236503946-a294c78b-9646-4410-84c8-7109cb17a051.png)


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
