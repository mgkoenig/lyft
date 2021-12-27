EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "LYFT (HMI Board)"
Date "Dec. 2021"
Rev "A"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Interface_Expansion:AS1115-BSST U1
U 1 1 611A8897
P 3300 2850
F 0 "U1" H 2850 4000 50  0000 C CNN
F 1 "AS1115-BSST" H 3600 1700 50  0000 C CNN
F 2 "Package_SO:QSOP-24_3.9x8.7mm_P0.635mm" H 3300 2850 50  0001 C CNN
F 3 "https://ams.com/documents/20143/36005/AS1115_DS000206_1-00.pdf/3d3e6d35-b184-1329-adf9-2d769eb2404f" H 3300 2850 50  0001 C CNN
	1    3300 2850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 611C2D1E
P 2450 1850
F 0 "R1" H 2520 1896 50  0000 L CNN
F 1 "13k" H 2520 1805 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 2380 1850 50  0001 C CNN
F 3 "~" H 2450 1850 50  0001 C CNN
	1    2450 1850
	-1   0    0    1   
$EndComp
Text GLabel 3000 6400 0    50   BiDi ~ 0
I2C_SDA
Text GLabel 3000 6500 0    50   Output ~ 0
I2C_SCL
$Comp
L power:+3V3 #PWR0101
U 1 1 611CA0C2
P 2850 5400
F 0 "#PWR0101" H 2850 5250 50  0001 C CNN
F 1 "+3V3" H 2865 5573 50  0000 C CNN
F 2 "" H 2850 5400 50  0001 C CNN
F 3 "" H 2850 5400 50  0001 C CNN
	1    2850 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 611CA896
P 2850 5700
F 0 "#PWR0102" H 2850 5450 50  0001 C CNN
F 1 "GND" H 2855 5527 50  0000 C CNN
F 2 "" H 2850 5700 50  0001 C CNN
F 3 "" H 2850 5700 50  0001 C CNN
	1    2850 5700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 611CB030
P 2000 1850
F 0 "C1" V 1748 1850 50  0000 C CNN
F 1 "10u" V 1839 1850 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 2038 1700 50  0001 C CNN
F 3 "~" H 2000 1850 50  0001 C CNN
	1    2000 1850
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW1
U 1 1 611CD0A9
P 8400 3600
F 0 "SW1" H 8400 3985 50  0000 C CNN
F 1 "TL1105" H 8400 3894 50  0000 C CNN
F 2 "LYFT:SW_TH_Tactile_Omron_B3F-10xx_Center" H 8400 3900 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 8400 3900 50  0001 C CNN
	1    8400 3600
	1    0    0    -1  
$EndComp
Text GLabel 4150 1850 2    50   Output ~ 0
SEG_A
Text GLabel 4100 3750 2    50   Input ~ 0
KEY_A
Text GLabel 7350 3500 0    50   Input ~ 0
SEG_D
Text GLabel 9200 4750 2    50   Output ~ 0
KEY_A
NoConn ~ 8200 3600
NoConn ~ 8600 3600
Text GLabel 2450 2450 0    50   Input ~ 0
I2C_SCL
Text GLabel 2450 2550 0    50   BiDi ~ 0
I2C_SDA
Text GLabel 2450 2750 0    50   Output ~ 0
HMI_IRQ
$Comp
L power:+3V3 #PWR05
U 1 1 61227320
P 3300 1400
F 0 "#PWR05" H 3300 1250 50  0001 C CNN
F 1 "+3V3" H 3315 1573 50  0000 C CNN
F 2 "" H 3300 1400 50  0001 C CNN
F 3 "" H 3300 1400 50  0001 C CNN
	1    3300 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 2250 2450 2000
Wire Wire Line
	2450 2250 2700 2250
Wire Wire Line
	3300 1650 3300 1400
$Comp
L power:+3V3 #PWR03
U 1 1 6122E6AB
P 2450 1400
F 0 "#PWR03" H 2450 1250 50  0001 C CNN
F 1 "+3V3" H 2465 1573 50  0000 C CNN
F 2 "" H 2450 1400 50  0001 C CNN
F 3 "" H 2450 1400 50  0001 C CNN
	1    2450 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 1400 2450 1700
$Comp
L power:+3V3 #PWR01
U 1 1 612368DD
P 2000 1400
F 0 "#PWR01" H 2000 1250 50  0001 C CNN
F 1 "+3V3" H 2015 1573 50  0000 C CNN
F 2 "" H 2000 1400 50  0001 C CNN
F 3 "" H 2000 1400 50  0001 C CNN
	1    2000 1400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 61237A39
P 2000 2100
F 0 "#PWR02" H 2000 1850 50  0001 C CNN
F 1 "GND" H 2005 1927 50  0000 C CNN
F 2 "" H 2000 2100 50  0001 C CNN
F 3 "" H 2000 2100 50  0001 C CNN
	1    2000 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 2100 2000 2000
Wire Wire Line
	2000 1400 2000 1700
Wire Wire Line
	2700 2450 2450 2450
Wire Wire Line
	2450 2550 2700 2550
Wire Wire Line
	2700 2750 2450 2750
Wire Wire Line
	4100 3750 3900 3750
Text GLabel 4150 1950 2    50   Output ~ 0
SEG_B
Text GLabel 4150 2050 2    50   Output ~ 0
SEG_C
Text GLabel 4150 2250 2    50   Output ~ 0
SEG_E
Text GLabel 4150 2150 2    50   Output ~ 0
SEG_D
Text GLabel 4150 2350 2    50   Output ~ 0
SEG_F
Text GLabel 4150 2450 2    50   Output ~ 0
SEG_G
Text GLabel 4150 2650 2    50   Output ~ 0
SEG_DP
Text GLabel 4150 2850 2    50   Input ~ 0
DIG_0
Text GLabel 4150 2950 2    50   Input ~ 0
DIG_1
Text GLabel 4150 3050 2    50   Input ~ 0
DIG_2
Wire Wire Line
	3900 1850 4150 1850
Wire Wire Line
	4150 1950 3900 1950
Wire Wire Line
	3900 2050 4150 2050
Wire Wire Line
	4150 2150 3900 2150
Wire Wire Line
	3900 2250 4150 2250
Wire Wire Line
	4150 2350 3900 2350
Wire Wire Line
	3900 2450 4150 2450
Wire Wire Line
	4150 2650 3900 2650
Wire Wire Line
	3900 2850 4150 2850
Wire Wire Line
	4150 2950 3900 2950
Wire Wire Line
	3900 3050 4150 3050
$Comp
L power:GND #PWR06
U 1 1 61244681
P 3300 4300
F 0 "#PWR06" H 3300 4050 50  0001 C CNN
F 1 "GND" H 3305 4127 50  0000 C CNN
F 2 "" H 3300 4300 50  0001 C CNN
F 3 "" H 3300 4300 50  0001 C CNN
	1    3300 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 4300 3300 4050
NoConn ~ 3900 3250
NoConn ~ 3900 3350
NoConn ~ 3900 3450
NoConn ~ 3900 3550
NoConn ~ 3900 3850
Text GLabel 7300 1450 1    50   Input ~ 0
SEG_A
Text GLabel 7500 1450 1    50   Input ~ 0
SEG_B
Text GLabel 7700 1450 1    50   Input ~ 0
SEG_C
Text GLabel 7900 1450 1    50   Input ~ 0
SEG_D
Text GLabel 8100 1450 1    50   Input ~ 0
SEG_E
Text GLabel 8300 1450 1    50   Input ~ 0
SEG_F
Text GLabel 8500 1450 1    50   Input ~ 0
SEG_G
Text GLabel 8700 1450 1    50   Input ~ 0
SEG_DP
Wire Wire Line
	7300 1450 7300 1650
Wire Wire Line
	7500 1650 7500 1450
Wire Wire Line
	7700 1450 7700 1650
Wire Wire Line
	7900 1650 7900 1450
Wire Wire Line
	8100 1450 8100 1650
Wire Wire Line
	8300 1650 8300 1450
Wire Wire Line
	8500 1650 8500 1450
Wire Wire Line
	8700 1650 8700 1450
Text GLabel 9200 2250 2    50   Output ~ 0
DIG_0
Text GLabel 9200 2350 2    50   Output ~ 0
DIG_1
Text GLabel 9200 2450 2    50   Output ~ 0
DIG_2
Wire Wire Line
	9000 2250 9200 2250
Wire Wire Line
	9200 2350 9000 2350
Wire Wire Line
	9000 2450 9200 2450
$Comp
L Switch:SW_MEC_5E SW2
U 1 1 612687B5
P 8400 4100
F 0 "SW2" H 8400 4485 50  0000 C CNN
F 1 "TL1105" H 8400 4394 50  0000 C CNN
F 2 "LYFT:SW_TH_Tactile_Omron_B3F-10xx_Center" H 8400 4400 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 8400 4400 50  0001 C CNN
	1    8400 4100
	1    0    0    -1  
$EndComp
Text GLabel 7350 4000 0    50   Input ~ 0
SEG_DP
NoConn ~ 8200 4100
$Comp
L Switch:SW_MEC_5E SW3
U 1 1 6126BB48
P 8400 4600
F 0 "SW3" H 8400 4985 50  0000 C CNN
F 1 "TL1105" H 8400 4894 50  0000 C CNN
F 2 "LYFT:SW_TH_Tactile_Omron_B3F-10xx_Center" H 8400 4900 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 8400 4900 50  0001 C CNN
	1    8400 4600
	1    0    0    -1  
$EndComp
Text GLabel 7350 4500 0    50   Input ~ 0
SEG_E
$Comp
L Switch:SW_MEC_5E SW4
U 1 1 6127B3D5
P 8400 5100
F 0 "SW4" H 8400 5485 50  0000 C CNN
F 1 "TL1105" H 8400 5394 50  0000 C CNN
F 2 "LYFT:SW_TH_Tactile_Omron_B3F-10xx_Center" H 8400 5400 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 8400 5400 50  0001 C CNN
	1    8400 5100
	1    0    0    -1  
$EndComp
Text GLabel 7350 5000 0    50   Input ~ 0
SEG_C
NoConn ~ 8200 5100
$Comp
L Switch:SW_MEC_5E SW5
U 1 1 612804EE
P 8400 5600
F 0 "SW5" H 8400 5985 50  0000 C CNN
F 1 "TL1105" H 8400 5894 50  0000 C CNN
F 2 "LYFT:SW_TH_Tactile_Omron_B3F-10xx_Center" H 8400 5900 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 8400 5900 50  0001 C CNN
	1    8400 5600
	1    0    0    -1  
$EndComp
Text GLabel 7350 6000 0    50   Input ~ 0
SEG_A
NoConn ~ 8200 5600
$Comp
L Switch:SW_MEC_5E SW6
U 1 1 612965DB
P 8400 6100
F 0 "SW6" H 8400 6485 50  0000 C CNN
F 1 "TL1105" H 8400 6394 50  0000 C CNN
F 2 "LYFT:SW_TH_Tactile_Omron_B3F-10xx_Center" H 8400 6400 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 8400 6400 50  0001 C CNN
	1    8400 6100
	1    0    0    -1  
$EndComp
Text GLabel 7350 5500 0    50   Input ~ 0
SEG_F
NoConn ~ 8600 6100
Wire Wire Line
	9200 4750 8900 4750
Connection ~ 8900 4750
Wire Wire Line
	8200 4000 7950 4000
Wire Wire Line
	8600 4000 8700 4000
Connection ~ 8900 4000
Wire Wire Line
	7350 4000 7650 4000
Wire Wire Line
	8900 4000 8900 4500
Wire Wire Line
	7350 3500 7650 3500
Wire Wire Line
	7950 3500 8200 3500
Wire Wire Line
	7350 4500 7650 4500
Connection ~ 8900 4500
Wire Wire Line
	8900 4500 8900 4750
Wire Wire Line
	7350 5000 7650 5000
Wire Wire Line
	7650 5500 7350 5500
Wire Wire Line
	7650 6000 7350 6000
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 613565AE
P 3200 5400
F 0 "#FLG0101" H 3200 5475 50  0001 C CNN
F 1 "PWR_FLAG" H 3200 5573 50  0000 C CNN
F 2 "" H 3200 5400 50  0001 C CNN
F 3 "~" H 3200 5400 50  0001 C CNN
	1    3200 5400
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 61359AF2
P 3200 5700
F 0 "#FLG0102" H 3200 5775 50  0001 C CNN
F 1 "PWR_FLAG" H 3200 5873 50  0000 C CNN
F 2 "" H 3200 5700 50  0001 C CNN
F 3 "~" H 3200 5700 50  0001 C CNN
	1    3200 5700
	-1   0    0    1   
$EndComp
Wire Wire Line
	3450 5500 3200 5500
Wire Wire Line
	3200 5500 3200 5400
Wire Wire Line
	2850 5400 2850 5500
Wire Wire Line
	2850 5500 3200 5500
Connection ~ 3200 5500
Wire Wire Line
	3450 6400 3000 6400
Wire Wire Line
	3000 6500 3450 6500
Wire Wire Line
	3450 6600 3000 6600
Wire Wire Line
	3450 5600 3200 5600
Wire Wire Line
	3200 5600 3200 5700
Wire Wire Line
	2850 5700 2850 5600
Wire Wire Line
	2850 5600 3200 5600
Connection ~ 3200 5600
NoConn ~ 8200 6100
NoConn ~ 8200 4600
Wire Wire Line
	8600 5500 8700 5500
Connection ~ 8900 5500
Wire Wire Line
	8900 5500 8900 6000
Wire Wire Line
	8900 4750 8900 5000
Wire Wire Line
	8600 5600 8700 5600
Wire Wire Line
	8700 5600 8700 5500
Connection ~ 8700 5500
Wire Wire Line
	8700 5500 8900 5500
Wire Wire Line
	8600 5000 8700 5000
Connection ~ 8900 5000
Wire Wire Line
	8900 5000 8900 5500
Wire Wire Line
	8600 5100 8700 5100
Wire Wire Line
	8700 5100 8700 5000
Connection ~ 8700 5000
Wire Wire Line
	8700 5000 8900 5000
Text GLabel 3000 6600 0    50   Input ~ 0
HMI_IRQ
Wire Wire Line
	8600 6000 8900 6000
$Comp
L Connector_Generic:Conn_01x03 J2
U 1 1 612951AF
P 3650 6500
F 0 "J2" H 3730 6542 50  0000 L CNN
F 1 "Conn_Signal" H 3730 6451 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x03_P2.54mm_Vertical" H 3650 6500 50  0001 C CNN
F 3 "~" H 3650 6500 50  0001 C CNN
	1    3650 6500
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J1
U 1 1 61295E93
P 3650 5500
F 0 "J1" H 3730 5492 50  0000 L CNN
F 1 "Conn_Power" H 3730 5401 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 3650 5500 50  0001 C CNN
F 3 "~" H 3650 5500 50  0001 C CNN
	1    3650 5500
	1    0    0    -1  
$EndComp
Text GLabel 9200 2550 2    50   Output ~ 0
DIG_L
Wire Wire Line
	9000 2550 9200 2550
Text GLabel 4150 3150 2    50   Input ~ 0
DIG_L
Wire Wire Line
	4150 3150 3900 3150
Wire Wire Line
	8600 4100 8700 4100
Wire Wire Line
	8700 4100 8700 4000
Connection ~ 8700 4000
Wire Wire Line
	8700 4000 8900 4000
Wire Wire Line
	8600 3500 8900 3500
Wire Wire Line
	8900 3500 8900 4000
Wire Wire Line
	8600 4600 8700 4600
Wire Wire Line
	7950 4500 8200 4500
Wire Wire Line
	8600 4500 8700 4500
Wire Wire Line
	8700 4600 8700 4500
Connection ~ 8700 4500
Wire Wire Line
	8700 4500 8900 4500
$Comp
L LYFT:HDSP-433G U2
U 1 1 612C8866
P 8000 2200
F 0 "U2" H 7200 1700 50  0000 R CNN
F 1 "HDSP-433G" H 8900 1700 50  0000 R CNN
F 2 "LYFT:HDSP-433" H 8000 2100 50  0001 C CNN
F 3 "" H 8000 2100 50  0001 C CNN
	1    8000 2200
	1    0    0    -1  
$EndComp
$Comp
L LYFT:Logo G1
U 1 1 612E0A6F
P 7300 6750
F 0 "G1" H 7578 6796 50  0000 L CNN
F 1 "Logo" H 7578 6705 50  0000 L CNN
F 2 "LYFT:LYFT-Logo_10x4" H 7300 6750 50  0001 C CNN
F 3 "" H 7300 6750 50  0001 C CNN
	1    7300 6750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7950 5000 8200 5000
Wire Wire Line
	7950 5500 8200 5500
Wire Wire Line
	7950 6000 8200 6000
$Comp
L Diode:1N4148W D1
U 1 1 61B31CA8
P 7800 3500
F 0 "D1" H 7800 3283 50  0000 C CNN
F 1 "1N4148W" H 7800 3374 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-123" H 7800 3325 50  0001 C CNN
F 3 "https://www.vishay.com/docs/85748/1n4148w.pdf" H 7800 3500 50  0001 C CNN
	1    7800 3500
	-1   0    0    1   
$EndComp
$Comp
L Diode:1N4148W D2
U 1 1 61B334CD
P 7800 4000
F 0 "D2" H 7800 3783 50  0000 C CNN
F 1 "1N4148W" H 7800 3874 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-123" H 7800 3825 50  0001 C CNN
F 3 "https://www.vishay.com/docs/85748/1n4148w.pdf" H 7800 4000 50  0001 C CNN
	1    7800 4000
	-1   0    0    1   
$EndComp
$Comp
L Diode:1N4148W D3
U 1 1 61B33C97
P 7800 4500
F 0 "D3" H 7800 4283 50  0000 C CNN
F 1 "1N4148W" H 7800 4374 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-123" H 7800 4325 50  0001 C CNN
F 3 "https://www.vishay.com/docs/85748/1n4148w.pdf" H 7800 4500 50  0001 C CNN
	1    7800 4500
	-1   0    0    1   
$EndComp
$Comp
L Diode:1N4148W D4
U 1 1 61B3469B
P 7800 5000
F 0 "D4" H 7800 4783 50  0000 C CNN
F 1 "1N4148W" H 7800 4874 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-123" H 7800 4825 50  0001 C CNN
F 3 "https://www.vishay.com/docs/85748/1n4148w.pdf" H 7800 5000 50  0001 C CNN
	1    7800 5000
	-1   0    0    1   
$EndComp
$Comp
L Diode:1N4148W D5
U 1 1 61B34E5F
P 7800 5500
F 0 "D5" H 7800 5283 50  0000 C CNN
F 1 "1N4148W" H 7800 5374 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-123" H 7800 5325 50  0001 C CNN
F 3 "https://www.vishay.com/docs/85748/1n4148w.pdf" H 7800 5500 50  0001 C CNN
	1    7800 5500
	-1   0    0    1   
$EndComp
$Comp
L Diode:1N4148W D6
U 1 1 61B355BE
P 7800 6000
F 0 "D6" H 7800 5783 50  0000 C CNN
F 1 "1N4148W" H 7800 5874 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-123" H 7800 5825 50  0001 C CNN
F 3 "https://www.vishay.com/docs/85748/1n4148w.pdf" H 7800 6000 50  0001 C CNN
	1    7800 6000
	-1   0    0    1   
$EndComp
$Comp
L Graphic:Logo_Open_Hardware_Small #LOGO1
U 1 1 61C10EE2
P 10900 6850
F 0 "#LOGO1" H 10900 7125 50  0001 C CNN
F 1 "Logo_Open_Hardware_Small" H 10900 6625 50  0001 C CNN
F 2 "Symbol:OSHW-Logo2_7.3x6mm_SilkScreen" H 10900 6850 50  0001 C CNN
F 3 "~" H 10900 6850 50  0001 C CNN
	1    10900 6850
	1    0    0    -1  
$EndComp
$EndSCHEMATC
