# Level 3: Pressure Sensitive Textile

## Introduction

In this level, we use pressure sensitive textile to intereact with a program running on a microprocessor.
This could be used for:

1. Create an interactive game where pushes of different strength must be used
2. Detect how long somebody is sitting on a chair to give feedback to be active
3. Create an insole that detects your step pattern

What makes this circuit different is that we use textile elements and electronical components 
adapted for use on textiles.

## BOM (Bill of Materials)

1. Base textile: a T-shirt, a baseball cap, a fabric. Select the textile you want to add lights to!
    1. Fruit of the Loom t-shirts can be bought for less than 2 Eur/T-shirt. Google it!
    2. Baseball caps can be bought for 1 to 2 Eur. 
2. Design: Your final product will be nicer if there is a design on your base textile. You can use other fabrics to make
a design, or use heat transfer vinyl, or textile transfer paper, or embroidery. Make sure in your design it is clear where the LED-Lights
go. A single small coincell battery can power maximally 6 LED's in this set up
3. Conductive thread or conductive fabric. You need a high conductive (low resistance) fabric or yarn. Some options:
    1. Silver coated fabric with conductive adhesive on one side. We only require a small band like the one from [Mission Control Lab](https://missioncontrollab.com/collections/technologist-and-teacher-supply/products/space-tape)
    2. Conductive fabricto sew on your base fabric, eg from [adafruit](https://www.adafruit.com/product/1168)
    3. Stainless steel yarn. The cheapest option. There are many resellers of [adafruit Stainless Thin Conductive Thread](https://www.adafruit.com/product/640), for example [Mouser](https://www.mouser.be/ProductDetail/Adafruit/603?qs=sGAEpiMZZMu%252BmKbOcEVhFQfi8wYXkauJFoZL2xpCq%252BVRONBlSbRF3w%3D%3D)
    4. Silver plated yarn. The highest conductivity. Madeira has a range for embroidery that can be bought from every Madeira reseller: [HC 12 150m SMALL CONE](https://shop.madeira.co.uk/hc-12-150m-sample-(high-conductive)_hc12-smp-xxx-xxx.htm)
4. LED lights. You can use standard ones (cheapest) or specially designed (brighter)
    1. Sewable LED lights with integrated resistor, eg from [Kitronik](https://www.kitronik.co.uk/2746-electro-fashion-sewable-5mm-led-holder-pack-of-10.html)
    2. Standard 3V LED packs, eg from [amazon.de](https://www.amazon.de/APTWONZ-2-poligen-Diffuse-Leuchtdioden-Elektronikkomponenten/dp/B06X3VT6TD)
    3. In combination with the above standard LED's, you can 3D print a small holder making it more easy to sew the LED. Use [this design](https://www.thingiverse.com/thing:265121).
5. Battery holder for coincell battery. We use a 3.3V coincell. You can buy a normal coincell holder, one specifically designed for textile, or 3D print one.
    1. Sewable battery holder without switch, eg from [Kitronik](https://www.kitronik.co.uk/2701-sewable-coin-cell-holder.html)
    2. 3V battery holder standard with holes that allow sewing, eg [BA2032SM](https://www.enrgtech.co.uk/buy/product/ET14129119/BA2032SM)
    4. CR2032 battery. This is the standard coincell battery you can buy everywhere for 1 to 2 Eur.
    5. 3D print 3V battery holder, eg [this design](https://www.thingiverse.com/thing:265116) or [this one](https://www.thingiverse.com/thing:250503)
6. Pressure sensitive fabric 
    1. You can make your own, but this is complex. We'll add a guide on making your own in the future
    2. EeonTex Pressure Sensing Fabric, from [Eeonyx](https://eeonyx.com/)
    3. Velostat/Linqstat conductive sheet, eg from [Adafruit](https://www.adafruit.com/product/1361). Not really a fabric, but easy to integrate in fabric. The black plastic to protect electrical components in shipping could behave like velostat and might be an alternative. 
    
## Demonstrator 05 Construction

We will make a demonstrator that reads in pressure applied to a fabric, and react to it by showing a light scale.

### Step 01 - Design your product: What will you make?

We want to make a textile input pad that can be squeezed/pressed, the size of a PC mice. 

### Step 02 - Construction of the pressure pad

Start with collecting your fabrics, and cut them in the size you want. The backing and top material:

![L05 materials 01](L05_step01.jpg)

and the pressure sensitive fabric:

![L05 materials 02](L05_step02.jpg)

With the conductive yarn, make a pattern on the backing material that will form the electrode in contact with the pressure sensitive fabric

![L05 electrode 01](L05_step03.jpg) 

Add a conductive snap button where the pressure pad will connect to your connector. 

![L05 snap button](L05_step04.jpg)

Create a good connection with the snap button 

![L05 snap button connection](L05_step05.jpg)

Repeat this step with the top fabric, in such a way that when the electrodes are facing each other the snap button connectors are in different positions so they cannot create a short circuit

![L05 two electrodes](L05_step06.jpg)

Now add the pressure sensitive material in the middle

![L05 pressure material](L05_step07.jpg)

If there is a danger of short circuit between the electrodes, add some cover fabric as in the image below. Non conductive fabric and yarn are used for this. 

![L05 protection material](L05_step08.jpg)

Once satisfied, sew everything together to obtain a thin pad.

![L05 materials 02](L05_step09.jpg)

Test the pressure pad to see that different pressures give different values of the resistivity. Use a multimeter for this. In our case, we measure on no pressure 15.5 kOhm

![L05 materials 02](L05_step10.jpg)

When applying pressure this drops, as in the following figure where the pressure is 1.8 kOhm

![L05 materials 02](L05_step11.jpg)

The pressure pad can already be used now, but we want a more robust larger pad for our application. Adding extra foam will spread out the pressure better causing less variation. It will also make the pad larger which is more suitable for smaller children, allowing for a squeezing action. We add foam at bottom and top, and close everything in a felt cover

![L05 materials 02](L05_step12.jpg)

The final pressure pad seen from top and bottom is:

![L05 materials 02](L05_step13.jpg) ![L05 materials 02](L05_step14.jpg)

### Step 3 - Reading in the pressure data

A variable pressure data is only usefull if we read this in with a microprocessor, and react to the pressure level. One could add a screen to show pressure data, or create a game that reacts to the amount of pressure. To show the principle, we will read in the data, and react with light depending on the pressure level, going from green light on no pressure, blue on medium, and red on high pressure.

We still need to add for this the following parts:

1. A connector from our pressure pad to the shirt/fabric/surface where the reaction is
2. A battery. As a microprocessor consumes more power, AAA batteries will be used
3. A microprocessor to read in the sensor and react to it. 
4. An RGB LED that can show multiple colors. 

We select following components:

1. Ingegno 3 wire elastic fabric 

    ![L05 connector](L05_step15.jpg)

2. xx battery holder
3. Neopixel LED
4. Wemos ESPxx board

#### Creating the connector

The connector is explained here

#### Creating the actuator: neopixel LED on fabric

Adding neopixel to a fabric

#### Adding the WeMos and connections to sensor and actuator

Connecting sensor and neopixel to the WeMos and placing the WeMos on the fabric

### Step 4 - Testing and Finishing touches

Pictures of the final result
