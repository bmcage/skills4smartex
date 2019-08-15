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
a design, or use transfer vinyl paper, or transfer paper, or embroidery. Make sure in your design it is clear where the LED-Lights
go. A single small coincell battery can power maximally 6 LED in this set up
3. Conductive thread or conductive fabric. You need a high conductive (low resistance) fabric or yarn. Some options:
    1. Silver coated fabric with conductive adhesive on one side. We only require a small band like from [Mission Control Lab](https://missioncontrollab.com/collections/technologist-and-teacher-supply/products/space-tape)
    2. Conductive fabric which needs to be sewed on your base fabric, eg from [adafruit](https://www.adafruit.com/product/1168)
    3. Stainless steel yarn. The cheapest option. Many resellers of [adafruit Stainless Thin Conductive Thread](https://www.adafruit.com/product/640), for example [Mouser](https://www.mouser.be/ProductDetail/Adafruit/603?qs=sGAEpiMZZMu%252BmKbOcEVhFQfi8wYXkauJFoZL2xpCq%252BVRONBlSbRF3w%3D%3D)
    4. Silver plated yarn. The highest conductivity. Madeira has a range for embroidery that can be bought from every Madeira reseller: [HC 12 150m SMALL CONE](https://shop.madeira.co.uk/hc-12-150m-sample-(high-conductive)_hc12-smp-xxx-xxx.htm)
4. LED lights. You can use standard ones (cheapest) or specially designed (brighter)
    1. Sewable LED lights with integrated resistor, eg from [Kitronik](https://www.kitronik.co.uk/2746-electro-fashion-sewable-5mm-led-holder-pack-of-10.html)
    2. Standard 3V LED packs, eg from [amazon.de](https://www.amazon.de/APTWONZ-2-poligen-Diffuse-Leuchtdioden-Elektronikkomponenten/dp/B06X3VT6TD)
    3. In combination with above standard LED, you can 3D print a small holder making it more easy to attach the LED to textile. Use [this design](https://www.thingiverse.com/thing:265121) then
5. Battery holder for coincell battery. We use a 3.3V coincell. You can buy a normal coincell holder, one specifically designed for textile, or 3D print one.
    1. Sewable battery holder without switch, eg from [Kitronik](https://www.kitronik.co.uk/2701-sewable-coin-cell-holder.html)
    2. 3V battery holder standard with holes that allow sewing, eg [BA2032SM](https://www.enrgtech.co.uk/buy/product/ET14129119/BA2032SM)
    4. CR2032 battery. This is the standard coincell battery you can buy everywhere for 1 to 2 Eur.
    5. 3D print 3V battery holder, eg [this design](https://www.thingiverse.com/thing:265116) or [this one](https://www.thingiverse.com/thing:250503)
6. Pressure sensitive fabric 
    1. You can make your own, but this is complex. We'll add a guide on making your own in the future
    2. EeonTex Pressure Sensing Fabric, from [Eeonyx](https://eeonyx.com/)
    3. Velostat/Linqstat conductive sheet, eg from [Adafruit](https://www.adafruit.com/product/1361). Not really a fabric, but easy to integrate in fabric. The black plastic to protect electrical components might behave and the same way and be used as pressure sensor too.

## Demonstrator 05 Construction

We will make a demonstrator that reads in pressure applied to a fabric, and react to it by showing a light scale.

### Step 01 - Design your product: What will you make?

We want to make a textile input pad that can be squeezed/pressed, the size of a PC mice. 
