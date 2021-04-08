EESchema Schematic File Version 4
LIBS:Faraway_PCB-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector_Generic:ESP32_DevKitV1_30PIN_Left J1
U 1 1 606F7ABD
P 4450 2250
F 0 "J1" H 4701 2250 50  0000 L CNN
F 1 "ESP32_DevKitV1_30PIN_Left" H 4700 2205 50  0001 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x15_P2.54mm_Vertical" H 4450 2250 50  0001 C CNN
F 3 "~" H 4450 2250 50  0001 C CNN
	1    4450 2250
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:ESP32_DevKitV1_30PIN_Right J2
U 1 1 606F88B3
P 5300 2250
F 0 "J2" H 5304 3075 50  0000 C CNN
F 1 "ESP32_DevKitV1_30PIN_Right" H 5304 3076 50  0001 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x15_P2.54mm_Vertical" H 5300 2250 50  0001 C CNN
F 3 "~" H 5300 2250 50  0001 C CNN
	1    5300 2250
	-1   0    0    -1  
$EndComp
Text GLabel 4250 2850 0    50   Input ~ 0
AUDIO
Text GLabel 4250 1950 0    50   Input ~ 0
SK
Text GLabel 5500 2450 2    50   Input ~ 0
SDA
Text GLabel 5500 2350 2    50   Input ~ 0
SCL
Text GLabel 5500 1750 2    50   Input ~ 0
PUL
Text GLabel 5500 1850 2    50   Input ~ 0
DIR
Text GLabel 5500 1950 2    50   Input ~ 0
EN
Text GLabel 5500 2050 2    50   Input ~ 0
LED
Text Notes 7100 1750 0    50   ~ 0
PUL
Text Notes 7100 1850 0    50   ~ 0
DIR
Text Notes 7100 1950 0    50   ~ 0
EN
Text Notes 7100 2050 0    50   ~ 0
LED
Text Notes 7100 1550 0    50   ~ 0
5V
$Comp
L power:GND #PWR0101
U 1 1 6070B389
P 6850 1600
F 0 "#PWR0101" H 6850 1350 50  0001 C CNN
F 1 "GND" V 6855 1472 50  0000 R CNN
F 2 "" H 6850 1600 50  0001 C CNN
F 3 "" H 6850 1600 50  0001 C CNN
	1    6850 1600
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0102
U 1 1 6070A8F7
P 6850 1500
F 0 "#PWR0102" H 6850 1350 50  0001 C CNN
F 1 "+5V" V 6865 1628 50  0000 L CNN
F 2 "" H 6850 1500 50  0001 C CNN
F 3 "" H 6850 1500 50  0001 C CNN
	1    6850 1500
	0    -1   -1   0   
$EndComp
Text Notes 7100 1650 0    50   ~ 0
GND
$Comp
L Connector:Conn_01x06_Female RJ45_INPUT1
U 1 1 60706020
P 7050 1700
F 0 "RJ45_INPUT1" H 7078 1630 50  0001 L CNN
F 1 "Conn_01x06_Female" H 7078 1585 50  0001 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x06_P2.54mm_Vertical" H 7050 1700 50  0001 C CNN
F 3 "~" H 7050 1700 50  0001 C CNN
F 4 "" H 7050 1700 50  0001 C CNN "Champ4"
	1    7050 1700
	1    0    0    -1  
$EndComp
Text GLabel 6850 1700 0    50   Input ~ 0
PUL
Text GLabel 6850 1800 0    50   Input ~ 0
DIR
Text GLabel 6850 1900 0    50   Input ~ 0
EN
Text GLabel 6850 2000 0    50   Input ~ 0
LED
Text Notes 6600 1400 0    50   ~ 0
RJ45 Input\nTo Motor Driver + Red Led
Text Notes 4400 1350 0    50   ~ 0
ESP32_DevKitV1_30Pins
$Comp
L Connector:Conn_01x02_Female I2C1
U 1 1 6071FE40
P 7100 2750
F 0 "I2C1" H 7128 2680 50  0000 L CNN
F 1 "Conn_01x02_Female" H 7128 2635 50  0001 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 7100 2750 50  0001 C CNN
F 3 "~" H 7100 2750 50  0001 C CNN
	1    7100 2750
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female ISC2
U 1 1 60722EEB
P 7100 3050
F 0 "ISC2" H 7128 2980 50  0000 L CNN
F 1 "Conn_01x02_Female" H 7128 2935 50  0001 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 7100 3050 50  0001 C CNN
F 3 "~" H 7100 3050 50  0001 C CNN
	1    7100 3050
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female HP1
U 1 1 60723644
P 3450 2750
F 0 "HP1" H 3478 2680 50  0000 L CNN
F 1 "Conn_01x02_Female" H 3478 2635 50  0001 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 3450 2750 50  0001 C CNN
F 3 "~" H 3450 2750 50  0001 C CNN
	1    3450 2750
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female SK1
U 1 1 60723995
P 3000 2200
F 0 "SK1" H 3028 2130 50  0000 L CNN
F 1 "Conn_01x02_Female" H 3028 2085 50  0001 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 3000 2200 50  0001 C CNN
F 3 "~" H 3000 2200 50  0001 C CNN
	1    3000 2200
	1    0    0    -1  
$EndComp
Text GLabel 2800 2200 0    50   Input ~ 0
SK
$Comp
L power:GND #PWR0103
U 1 1 607249FF
P 2800 2300
F 0 "#PWR0103" H 2800 2050 50  0001 C CNN
F 1 "GND" H 2805 2127 50  0000 C CNN
F 2 "" H 2800 2300 50  0001 C CNN
F 3 "" H 2800 2300 50  0001 C CNN
	1    2800 2300
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0104
U 1 1 60725071
P 3250 2750
F 0 "#PWR0104" H 3250 2600 50  0001 C CNN
F 1 "+5V" V 3265 2878 50  0000 L CNN
F 2 "" H 3250 2750 50  0001 C CNN
F 3 "" H 3250 2750 50  0001 C CNN
	1    3250 2750
	1    0    0    -1  
$EndComp
Text GLabel 6900 2750 0    50   Input ~ 0
SCL
Text GLabel 6900 2850 0    50   Input ~ 0
SDA
Text GLabel 6900 3050 0    50   Input ~ 0
SCL
Text GLabel 6900 3150 0    50   Input ~ 0
SDA
Text Notes 6700 2650 0    50   ~ 0
LIDAR_I2C
Text GLabel 2650 3250 0    50   Input ~ 0
AUDIO
$Comp
L power:GND #PWR0105
U 1 1 60703A6D
P 3250 3450
F 0 "#PWR0105" H 3250 3200 50  0001 C CNN
F 1 "GND" H 3255 3277 50  0000 C CNN
F 2 "" H 3250 3450 50  0001 C CNN
F 3 "" H 3250 3450 50  0001 C CNN
	1    3250 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 606FD33B
P 2800 3250
F 0 "R1" V 3007 3250 50  0000 C CNN
F 1 "1k" V 2916 3250 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 2730 3250 50  0001 C CNN
F 3 "~" H 2800 3250 50  0001 C CNN
	1    2800 3250
	0    -1   -1   0   
$EndComp
$Comp
L Transistor_BJT:PN2222A Q1
U 1 1 60702263
P 3150 3250
F 0 "Q1" H 3340 3204 50  0000 L CNN
F 1 "PN2222A" H 3340 3295 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 3350 3175 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/PN/PN2222A.pdf" H 3150 3250 50  0001 L CNN
	1    3150 3250
	1    0    0    1   
$EndComp
$Comp
L Connector:Conn_01x02_Female 5V_BONUS1
U 1 1 60749968
P 2950 1550
F 0 "5V_BONUS1" H 2978 1480 50  0000 L CNN
F 1 "Conn_01x02_Female" H 2978 1435 50  0001 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 2950 1550 50  0001 C CNN
F 3 "~" H 2950 1550 50  0001 C CNN
	1    2950 1550
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0106
U 1 1 6074996E
P 2750 1550
F 0 "#PWR0106" H 2750 1400 50  0001 C CNN
F 1 "+5V" V 2765 1678 50  0000 L CNN
F 2 "" H 2750 1550 50  0001 C CNN
F 3 "" H 2750 1550 50  0001 C CNN
	1    2750 1550
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 60749BEF
P 2750 1650
F 0 "#PWR0107" H 2750 1400 50  0001 C CNN
F 1 "GND" H 2755 1477 50  0000 C CNN
F 2 "" H 2750 1650 50  0001 C CNN
F 3 "" H 2750 1650 50  0001 C CNN
	1    2750 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 2850 3250 3050
Text Notes 2650 2100 0    50   ~ 0
LED_STRIP
Text Notes 2700 2800 0    50   ~ 0
AUDIO
Text Notes 2650 1400 0    50   ~ 0
POWER_BONUS
$Comp
L Connector:Conn_01x03_Female SPARE_PINS1
U 1 1 60763578
P 4050 2450
F 0 "SPARE_PINS1" H 3942 2643 50  0000 C CNN
F 1 "Conn_01x03_Female" H 3942 2644 50  0001 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x03_P2.54mm_Vertical" H 4050 2450 50  0001 C CNN
F 3 "~" H 4050 2450 50  0001 C CNN
	1    4050 2450
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x06_Female RJ45_INPUT2
U 1 1 606EDC89
P 7700 2000
F 0 "RJ45_INPUT2" H 7728 1930 50  0001 L CNN
F 1 "Conn_01x06_Female" H 7728 1885 50  0001 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 7700 2000 50  0001 C CNN
F 3 "~" H 7700 2000 50  0001 C CNN
F 4 "" H 7700 2000 50  0001 C CNN "Champ4"
	1    7700 2000
	1    0    0    -1  
$EndComp
$EndSCHEMATC
