EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 2
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 3800 2700 950  700 
U 5EB613C9
F0 "DUT" 50
F1 "../voltage_divider/voltage_divider.sch" 50
F2 "Vcc" U R 4750 2800 50 
F3 "Gnd" U R 4750 3300 50 
F4 "out" U R 4750 3050 50 
$EndSheet
$Comp
L Simulation_SPICE:VSIN V1
U 1 1 5EB61A71
P 5650 3050
F 0 "V1" H 5780 3141 50  0000 L CNN
F 1 "VSIN" H 5780 3050 50  0000 L CNN
F 2 "" H 5650 3050 50  0001 C CNN
F 3 "~" H 5650 3050 50  0001 C CNN
F 4 "Y" H 5650 3050 50  0001 L CNN "Spice_Netlist_Enabled"
F 5 "V" H 5650 3050 50  0001 L CNN "Spice_Primitive"
F 6 "sin(0 1 1k)" H 5780 2959 50  0000 L CNN "Spice_Model"
	1    5650 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 2850 5650 2800
Wire Wire Line
	5650 2800 4750 2800
Wire Wire Line
	5650 3250 5650 3300
Wire Wire Line
	5650 3300 4750 3300
$Comp
L power:GND #PWR01
U 1 1 5EB61DF2
P 5650 3300
F 0 "#PWR01" H 5650 3050 50  0001 C CNN
F 1 "GND" H 5655 3127 50  0000 C CNN
F 2 "" H 5650 3300 50  0001 C CNN
F 3 "" H 5650 3300 50  0001 C CNN
	1    5650 3300
	1    0    0    -1  
$EndComp
Connection ~ 5650 3300
Text Label 5200 3050 0    50   ~ 0
out
Wire Wire Line
	5200 3050 4750 3050
Text Label 5650 2700 1    50   ~ 0
in
Wire Wire Line
	5650 2700 5650 2800
Connection ~ 5650 2800
$EndSCHEMATC
