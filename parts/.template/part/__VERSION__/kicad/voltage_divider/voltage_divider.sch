EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
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
L Device:R R1
U 1 1 5EB5A4B7
P 5500 3050
F 0 "R1" H 5430 3004 50  0000 R CNN
F 1 "1k" H 5430 3095 50  0000 R CNN
F 2 "" V 5430 3050 50  0001 C CNN
F 3 "~" H 5500 3050 50  0001 C CNN
	1    5500 3050
	-1   0    0    1   
$EndComp
$Comp
L Device:R R2
U 1 1 5EB5A7C0
P 5500 3550
F 0 "R2" H 5430 3504 50  0000 R CNN
F 1 "1k" H 5430 3595 50  0000 R CNN
F 2 "" V 5430 3550 50  0001 C CNN
F 3 "~" H 5500 3550 50  0001 C CNN
	1    5500 3550
	-1   0    0    1   
$EndComp
Wire Wire Line
	5500 2700 5500 2900
Wire Wire Line
	5500 3200 5500 3300
Wire Wire Line
	5500 3700 5500 3900
Wire Wire Line
	5500 3300 5800 3300
Connection ~ 5500 3300
Wire Wire Line
	5500 3300 5500 3400
Text HLabel 5500 2700 1    50   UnSpc ~ 0
Vcc
Text HLabel 5500 3900 3    50   UnSpc ~ 0
Gnd
Text HLabel 5800 3300 2    50   UnSpc ~ 0
out
$EndSCHEMATC
