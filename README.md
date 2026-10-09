Microcontroller-Based Voltage Protector Device
==============================================

An automated single-phase AC mains monitoring and overvoltage protection system built around an ATmega328P microcontroller (Arduino Uno). The system continuously measures real-time AC voltage, displays parameters on a 16x2 LCD, allows user adjustments via step-up/down pushbuttons, and instantly trips a relay to protect connected loads against voltage surges.

Features
--------
* **Real-Time RMS Monitoring:** Steps down and samples AC voltage safely via a transformer and rectification/filtering circuit.
* **Automatic Overvoltage Cutoff:** Instantly de-energizes the relay when mains voltage exceeds the user-defined limit.
* **Adjustable Trip Threshold:** Dedicated Increment and Decrement buttons allow adjusting the cutoff limit in $\pm 5\text{V}$ steps.
* **EEPROM Persistence:** Saves custom voltage threshold settings across power resets.
* **Visual Status & LCD Feedback:** Live tracking of line voltage, trip limits, and relay state on an LM016L 16x2 LCD.
## System Running Preview
![System Running](test_evidence/System%20running.PNG)
System Requirements
-------------------
* **Simulation Software:** Proteus Design Suite (v8.0 or newer recommended)
* **Development Environment:** Arduino IDE (v1.8.x or v2.x)
* **Required Arduino Libraries:** 
  * ``LiquidCrystal`` (built-in)
  * ``EEPROM`` (built-in)

Getting Started: Arduino IDE Setup
----------------------------------
1. **Clone or Download Source Code:** Copy the firmware script into your Arduino IDE.
2. **Select Board:** Open Arduino IDE, go to **Tools > Board**, and select **Arduino Uno**.
3. **Select Port:** Connect your hardware programmer or board via **Tools > Port**.
4. **Compile Code:** Click the **Verify** button to ensure there are no compilation errors. Export the compiled binary (``.hex`` file) via **Sketch > Export Compiled Binary** to load it into the ATmega328P model in Proteus.

How to Run the Proteus Simulation
---------------------------------
1. **Open the Project File:** Launch Proteus Design Suite and open your circuit schematic file.
2. **Load Firmware into MCU:** 
   * Double-click the **Arduino Uno (``UNO1``)** component.
   * In the **Program File** field, browse and select the ``.hex`` file.
   * Ensure the clock frequency is set to **16 MHz**.
3. **Run Simulation:** Click the **Play** button at the bottom-left corner of Proteus.
4. **Operational Verification:**
   * Observe the LCD startup screen, followed by the real-time voltage display.
   * Adjust the AC voltage source (``V1``) or trimmer potentiometer (``RV2``).
   * Press the **Increment** or **Decrement** buttons to modify the overvoltage trip threshold.
   * Verify that when input voltage equals or exceeds the threshold, the relay disconnects load ``L1`` and displays ``Relay:OFF (TRIP)``.