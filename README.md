# Pacman_LPC1768_ASM_C

## Overview
A Pacman game implementation for the NXP LPC1768 microcontroller, developed using a combination of ARM Assembly and C programming languages.

## Description
This project implements a classic Pacman game on the LPC1768 microcontroller platform. It utilizes various peripherals including timers, buttons, joystick, ADC, and a graphical LCD display to create an interactive gaming experience.

## Prerequisites
- Keil µVision IDE (Version 5.41.0.0)
- ARM Compiler 6 (Version 6.22)
- NXP LPC1700 Device Family Pack (Version 2.7.2)
- CMSIS (Version 6.1.0)
- LPC1768 development board

## Folder Structure
- `/Source`: Main source code files
    - `/button_EXINT`: Button handling and external interrupt code
    - `/GLCD`: Graphical LCD interface code
    - `/RIT`: Repetitive Interrupt Timer code
    - `/timer`: Timer functionality
    - `/joystick`: Joystick interface code
    - `/speaker`: Audio output code
    - `/CMSIS_core`: Core ARM Cortex-M3 definitions
- `/Objects`: Compiled object files and build logs
- `/Listings`: Assembly listings and debugging information

## Hardware Features
- LPC1768 ARM Cortex-M3 microcontroller
- Button inputs for game control
- Joystick interface for movement
- Speaker output for game sounds
- Graphical LCD for display

## Developer
- Name: Erik
- Surname: Scolaro
- Email: erik.scolaro.dev@gmail.com
- Course: Computer Architectures, 1st year of the Master's in Computer Engineering, AI track.