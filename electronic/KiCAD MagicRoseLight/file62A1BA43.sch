EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 3
Title "Magic Rose Light Type 1"
Date "2022-06-06"
Rev "1.0"
Comp "Eike Müller"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L esp32_nodemcu:ESP32_NodeMCU U?
U 1 1 62A2343C
P 6150 3750
AR Path="/62A2343C" Ref="U?"  Part="1" 
AR Path="/62A1BA44/62A2343C" Ref="U2"  Part="1" 
F 0 "U2" H 6125 4365 50  0000 C CNN
F 1 "ESP32_NodeMCU" H 6125 4274 50  0000 C CNN
F 2 "" H 6150 3750 50  0001 C CNN
F 3 "" H 6150 3750 50  0001 C CNN
	1    6150 3750
	1    0    0    -1  
$EndComp
$Comp
L Device:Battery_Cell BT?
U 1 1 62A23442
P 2550 3900
AR Path="/62A23442" Ref="BT?"  Part="1" 
AR Path="/62A1BA44/62A23442" Ref="BT1"  Part="1" 
F 0 "BT1" V 2295 3950 50  0000 C CNN
F 1 "Battery_Cell" V 2386 3950 50  0000 C CNN
F 2 "" V 2550 3960 50  0001 C CNN
F 3 "~" V 2550 3960 50  0001 C CNN
	1    2550 3900
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push_SPDT SW?
U 1 1 62A23448
P 3700 2050
AR Path="/62A23448" Ref="SW?"  Part="1" 
AR Path="/62A1BA44/62A23448" Ref="SW1"  Part="1" 
F 0 "SW1" H 3700 2335 50  0000 C CNN
F 1 "Sliding Switch" H 3700 2244 50  0000 C CNN
F 2 "" H 3700 2050 50  0001 C CNN
F 3 "~" H 3700 2050 50  0001 C CNN
	1    3700 2050
	1    0    0    -1  
$EndComp
$Comp
L Connector:USB_A J?
U 1 1 62A2344E
P 2650 3200
AR Path="/62A2344E" Ref="J?"  Part="1" 
AR Path="/62A1BA44/62A2344E" Ref="J2"  Part="1" 
F 0 "J2" V 2661 3530 50  0000 L CNN
F 1 "USB-A-Connector" V 2752 3530 50  0000 L CNN
F 2 "" H 2800 3150 50  0001 C CNN
F 3 " ~" H 2800 3150 50  0001 C CNN
	1    2650 3200
	0    1    1    0   
$EndComp
Wire Notes Line
	2000 4450 2000 2950
Wire Wire Line
	2850 3500 2850 3900
Wire Wire Line
	2850 3900 2750 3900
Wire Wire Line
	2250 3200 2150 3200
Wire Wire Line
	2150 3200 2150 3900
Wire Wire Line
	2150 3900 2450 3900
Text Notes 2400 4300 0    50   ~ 0
Akku-Pack
Wire Notes Line
	3700 2950 3700 4450
Wire Notes Line
	2000 2950 3700 2950
Wire Notes Line
	2000 4450 3700 4450
$Comp
L Connector:USB_A J?
U 1 1 62A2345E
P 2650 2650
AR Path="/62A2345E" Ref="J?"  Part="1" 
AR Path="/62A1BA44/62A2345E" Ref="J1"  Part="1" 
F 0 "J1" V 2753 2979 50  0000 L CNN
F 1 "USB-A-Plug" V 2662 2979 50  0000 L CNN
F 2 "" H 2800 2600 50  0001 C CNN
F 3 " ~" H 2800 2600 50  0001 C CNN
	1    2650 2650
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2450 2350 2450 2050
Wire Wire Line
	2450 2050 3500 2050
Wire Wire Line
	5100 2050 4600 2050
Wire Wire Line
	3900 2150 4400 2150
Wire Wire Line
	4400 2150 4400 1950
Wire Wire Line
	4400 1950 5100 1950
Wire Wire Line
	4600 4750 5400 4750
Wire Wire Line
	4400 2150 4400 4850
Wire Wire Line
	4400 4850 5400 4850
Connection ~ 4400 2150
$Comp
L Device:R R?
U 1 1 62A2346E
P 4800 3050
AR Path="/62A2346E" Ref="R?"  Part="1" 
AR Path="/62A1BA44/62A2346E" Ref="R1"  Part="1" 
F 0 "R1" H 4870 3096 50  0000 L CNN
F 1 "470" H 4870 3005 50  0000 L CNN
F 2 "" V 4730 3050 50  0001 C CNN
F 3 "~" H 4800 3050 50  0001 C CNN
	1    4800 3050
	1    0    0    -1  
$EndComp
$Comp
L led_ring_type_1:LEDRing U?
U 1 1 62A23474
P 6100 2000
AR Path="/62A23474" Ref="U?"  Part="1" 
AR Path="/62A1BA44/62A23474" Ref="U1"  Part="1" 
F 0 "U1" H 6768 2046 50  0000 L CNN
F 1 "LED-Ring" H 6768 1955 50  0000 L CNN
F 2 "" H 6100 1900 50  0001 C CNN
F 3 "" H 6100 1900 50  0001 C CNN
	1    6100 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 4350 4800 4350
Wire Wire Line
	4800 4350 4800 3200
Wire Wire Line
	4800 2900 4800 1850
Wire Wire Line
	4800 1850 5100 1850
Wire Wire Line
	4600 2050 4600 2250
Wire Wire Line
	3050 2650 3200 2650
Wire Wire Line
	3200 2650 3200 2250
Wire Wire Line
	3200 2250 4600 2250
Connection ~ 4600 2250
Wire Wire Line
	4600 2250 4600 4750
Wire Wire Line
	6850 4850 7050 4850
Wire Wire Line
	7050 4850 7050 5100
$Comp
L power:+3.3V #PWR?
U 1 1 62D40E46
P 7050 5100
F 0 "#PWR?" H 7050 4950 50  0001 C CNN
F 1 "+3.3V" H 7065 5273 50  0000 C CNN
F 2 "" H 7050 5100 50  0001 C CNN
F 3 "" H 7050 5100 50  0001 C CNN
	1    7050 5100
	-1   0    0    1   
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 62D41C1A
P 7350 4100
F 0 "#PWR?" H 7350 3950 50  0001 C CNN
F 1 "+3.3V" H 7365 4273 50  0000 C CNN
F 2 "" H 7350 4100 50  0001 C CNN
F 3 "" H 7350 4100 50  0001 C CNN
	1    7350 4100
	-1   0    0    1   
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 62D46EBF
P 7350 4600
F 0 "#PWR?" H 7350 4450 50  0001 C CNN
F 1 "+3.3V" H 7365 4773 50  0000 C CNN
F 2 "" H 7350 4600 50  0001 C CNN
F 3 "" H 7350 4600 50  0001 C CNN
	1    7350 4600
	-1   0    0    1   
$EndComp
Wire Wire Line
	6850 4450 7350 4450
Wire Wire Line
	7350 4450 7350 4600
Text Notes 7550 4250 0    50   ~ 0
Rosenlicht\nTyp 1
Text Notes 7550 4750 0    50   ~ 0
Hardware\nVersion 2
Wire Notes Line
	6600 4400 6600 4500
Wire Notes Line
	7150 4100 6600 4100
Wire Notes Line
	6600 4100 6600 3900
Wire Notes Line
	6600 3900 7500 3900
Wire Wire Line
	6850 4050 7350 4050
Wire Wire Line
	7350 4050 7350 4100
Wire Notes Line
	7150 4350 7500 4350
Wire Notes Line
	7150 4100 7150 4350
Wire Notes Line
	7500 3900 7500 4350
Wire Notes Line
	6600 4400 7500 4400
Wire Notes Line
	7500 4400 7500 4850
Wire Notes Line
	7500 4850 7150 4850
Wire Notes Line
	7150 4850 7150 4500
Wire Notes Line
	7150 4500 6600 4500
$EndSCHEMATC
