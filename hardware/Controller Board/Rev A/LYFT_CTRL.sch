EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 4
Title "LYFT (Controller Board)"
Date "Dec. 2021"
Rev "A"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 1000 1500 2000 1000
U 612B66A0
F0 "LYFT_Power" 50
F1 "LYFT_Power.sch" 50
$EndSheet
$Sheet
S 3500 1500 2000 1000
U 612B6716
F0 "LYFT_Controller" 50
F1 "LYFT_Controller.sch" 50
$EndSheet
$Sheet
S 6000 1500 2000 1000
U 612B67A7
F0 "LYFT_Peripheral" 50
F1 "LYFT_Peripheral.sch" 50
$EndSheet
Wire Notes Line
	1000 3500 3000 3500
Wire Notes Line
	3000 3500 3000 4500
Wire Notes Line
	3000 4500 1000 4500
Wire Notes Line
	1000 4500 1000 3500
Text Notes 1000 3450 0    50   ~ 0
Mechanics
$Comp
L Mechanical:MountingHole H1
U 1 1 612B6FCC
P 1200 3700
F 0 "H1" H 1300 3746 50  0000 L CNN
F 1 "MountingHole" H 1300 3655 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 1200 3700 50  0001 C CNN
F 3 "~" H 1200 3700 50  0001 C CNN
	1    1200 3700
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H3
U 1 1 612B79D8
P 1200 4000
F 0 "H3" H 1300 4046 50  0000 L CNN
F 1 "MountingHole" H 1300 3955 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 1200 4000 50  0001 C CNN
F 3 "~" H 1200 4000 50  0001 C CNN
	1    1200 4000
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H4
U 1 1 612B7C55
P 1200 4300
F 0 "H4" H 1300 4346 50  0000 L CNN
F 1 "MountingHole" H 1300 4255 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 1200 4300 50  0001 C CNN
F 3 "~" H 1200 4300 50  0001 C CNN
	1    1200 4300
	1    0    0    -1  
$EndComp
$Comp
L LYFT:Logo G1
U 1 1 612B8173
P 7350 6750
F 0 "G1" H 7628 6796 50  0000 L CNN
F 1 "Logo" H 7628 6705 50  0000 L CNN
F 2 "LYFT:LYFT-Logo_10x4" H 7350 6750 50  0001 C CNN
F 3 "" H 7350 6750 50  0001 C CNN
	1    7350 6750
	1    0    0    -1  
$EndComp
Text Notes 1100 1850 0    50   ~ 0
+3V3 Board Supply
Text Notes 3600 1650 0    50   ~ 0
SAM-D21 Controller
Text Notes 3600 1750 0    50   ~ 0
Cortex Debug Interface
Text Notes 3600 1850 0    50   ~ 0
I2C Pull-up
Text Notes 6100 1650 0    50   ~ 0
LIN Driver
Text Notes 6100 1750 0    50   ~ 0
USB Communication
Text Notes 6100 1850 0    50   ~ 0
Audio Transducer
Wire Notes Line
	3500 4500 3500 3500
Text Notes 3500 3450 0    50   ~ 0
Terminals
$Comp
L Connector_Generic:Conn_01x03 J1
U 1 1 612BE684
P 4050 4000
F 0 "J1" H 4050 3800 50  0000 C CNN
F 1 "Conn_EXT" V 4150 4000 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 4050 4000 50  0001 C CNN
F 3 "~" H 4050 4000 50  0001 C CNN
	1    4050 4000
	-1   0    0    -1  
$EndComp
$Comp
L power:+24V #PWR01
U 1 1 612BFB76
P 4450 3900
F 0 "#PWR01" H 4450 3750 50  0001 C CNN
F 1 "+24V" V 4450 4100 50  0000 C CNN
F 2 "" H 4450 3900 50  0001 C CNN
F 3 "" H 4450 3900 50  0001 C CNN
	1    4450 3900
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR02
U 1 1 612C0076
P 4450 4000
F 0 "#PWR02" H 4450 3750 50  0001 C CNN
F 1 "GND" V 4450 3800 50  0000 C CNN
F 2 "" H 4450 4000 50  0001 C CNN
F 3 "" H 4450 4000 50  0001 C CNN
	1    4450 4000
	0    -1   -1   0   
$EndComp
Text GLabel 4450 4100 2    50   BiDi ~ 0
LINBUS
Wire Wire Line
	4250 4000 4450 4000
Wire Wire Line
	4250 3900 4350 3900
Wire Wire Line
	4250 4100 4450 4100
$Comp
L Connector_Generic:Conn_01x05 J2
U 1 1 612C260C
P 6050 4000
F 0 "J2" H 6050 3700 50  0000 C CNN
F 1 "Conn_HMI" V 6150 4000 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 6050 4000 50  0001 C CNN
F 3 "~" H 6050 4000 50  0001 C CNN
	1    6050 4000
	-1   0    0    1   
$EndComp
Text GLabel 6450 3800 2    50   Output ~ 0
HMI_IRQ
Text GLabel 6450 3900 2    50   Input ~ 0
I2C_SCL
Text GLabel 6450 4000 2    50   BiDi ~ 0
I2C_SDA
$Comp
L power:GND #PWR03
U 1 1 612C3463
P 6450 4100
F 0 "#PWR03" H 6450 3850 50  0001 C CNN
F 1 "GND" V 6450 3900 50  0000 C CNN
F 2 "" H 6450 4100 50  0001 C CNN
F 3 "" H 6450 4100 50  0001 C CNN
	1    6450 4100
	0    -1   -1   0   
$EndComp
$Comp
L power:+3V3 #PWR04
U 1 1 612C3C6B
P 6450 4200
F 0 "#PWR04" H 6450 4050 50  0001 C CNN
F 1 "+3V3" V 6450 4300 50  0000 L CNN
F 2 "" H 6450 4200 50  0001 C CNN
F 3 "" H 6450 4200 50  0001 C CNN
	1    6450 4200
	0    1    1    0   
$EndComp
Wire Wire Line
	6250 3800 6450 3800
Wire Wire Line
	6450 3900 6250 3900
Wire Wire Line
	6250 4000 6450 4000
Wire Wire Line
	6450 4100 6250 4100
Wire Notes Line
	8000 3500 8000 4500
Wire Notes Line
	3500 3500 8000 3500
Wire Notes Line
	3500 4500 8000 4500
Text Notes 1100 1750 0    50   ~ 0
+12V LIN Supply
Wire Wire Line
	6450 4200 6250 4200
Text Notes 6100 1950 0    50   ~ 0
Safety Pin
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 61602D42
P 4350 3850
F 0 "#FLG0101" H 4350 3925 50  0001 C CNN
F 1 "PWR_FLAG" H 4350 4023 50  0000 C CNN
F 2 "" H 4350 3850 50  0001 C CNN
F 3 "~" H 4350 3850 50  0001 C CNN
	1    4350 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 3850 4350 3900
Connection ~ 4350 3900
Wire Wire Line
	4350 3900 4450 3900
Text Notes 1100 1650 0    50   ~ 0
Power LED
$Comp
L Graphic:Logo_Open_Hardware_Small #LOGO1
U 1 1 61BFAD5A
P 10900 6850
F 0 "#LOGO1" H 10900 7125 50  0001 C CNN
F 1 "Logo_Open_Hardware_Small" H 10900 6625 50  0001 C CNN
F 2 "Symbol:OSHW-Logo2_7.3x6mm_SilkScreen" H 10900 6850 50  0001 C CNN
F 3 "~" H 10900 6850 50  0001 C CNN
	1    10900 6850
	1    0    0    -1  
$EndComp
$EndSCHEMATC
