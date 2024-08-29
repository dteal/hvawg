# High Voltage Arbitrary Waveform Generator

This is the first iteration of a modular arbitrary waveform generator intended to generate possibly tens of separate ±100V 1MHz signals for small capacitive loads, e.g., electrostatic actuators or ion traps.

WARNING: THIS BOARD IS DANGEROUS. IT USES SOMEWHAT HIGH VOLTAGES AT SOMEWHAT HIGH POWER. DO NOT BUILD, TEST, OR USE WITHOUT BOTH CAUTION AND KNOWING WHAT YOU ARE DOING. No guarantees or warranty or support is provided, etc.

![3D render of PCB](/media/pcb-3d.png)

Performance:

![frequency response curve](/media/frequency-response.png)

## Circuit Design

This circuit was made with the following constraints in mind:
- This is primarily intended to drive small capacitive loads, on the order of tens of picofarads with maybe megaohms of resistance. Note that oscilloscope input impedances are also about this much.
- Arbitrary waveform outputs between -100V and +100V and up to 1MHz. Either of those constraints is easy; together they are nontrivial. The ADHV4702 opamp can reach those voltages but only up to about 20kHz, for example.
- In order to have multiple outputs, multiple boards should be able to be daisy chained together. The final design allows up to four boards to be daisy chained and driven off a single 4-wire serial connection (three signals plus GND), for an average of 0.75 input signals per board, to drive as many boards as possible off of a single FPGA or other controller.
- Low cost. If tens of these boards are required for an application, each one should individually be cheap. This resulted in, e.g., five separate power supply inputs. The final part cost per board is around $20. Note a 2-layer PCB was used to decrease manufacturing time for external reasons; switching a 4-layer PCB or better is probably a good idea to allow better EMI mitigation techniques.
- Mass production. The circuit should not rely on manually matched transistors for a carefully-balanced amplifier. Instead, the output uses current mirrors that are cheaply implemented with off-the-shelf silicon power MOSFETs and don't require closed loop control on the output (which is not easily achieved at 1MHz at those voltages due to transistor switching time). (There is still manual tuning of amplifier gain and bias in order to minimize cost and accuracy, but this only has to be done once per board.)
- Robustness. The output can probably survive a short-circuit (untested).

The amplifier looks something like the following:

![frequency response curve](/media/circuit.png)

### Alternatives

Previous but insufficient approaches to similar problems include:
- Using a high-voltage op-amp, e.g., the ADHV4702 <https://www.analog.com/en/products/adhv4702-1.html>. This can achieve the +/-100V range but only up to a bit over 20kHz.
- Resonant circuits. Common in ion traps and particle accelerators, but only generate a single specific frequency.
- Discrete amplifiers of assorted topologies, e.g., <https://doi.org/10.1063/1.1988272> or <https://doi.org/10.1063/1.3622750> or <https://www.radiolocman.com/shem/schematics.html?di=637101>. Note also <https://www.falco-systems.com/High_voltage_amplifier_WMA-300.html>. Commonly used for piezoelectric actuator drives, ultrasound, MEMS, or ion traps. These amplifiers achieve the required voltage and frequency specifications but are expensive and/or complex and rely on obscure components.

### Power input

This board takes five power inputs (in addition to the GND connection, of course):
- +12V (1W)
- -12V (1W)
- +100V (5W)
- -100V (5W)
- 12V above the -100V negative rail (1W)

These should be turned on in the following order:
1. +12V
2. -12V
3. +100V
4. -100V
5. 12V above -100V rail

LEDs indicate when the three 12V power supplies are on.

The +/-100V high voltage supplies are used to drive the output and need to be capable of the full board power draw (potentially 10W each, depending on component selection and voltage output).

The +/-12V supplies are used to drive the DAC and intermediate amplifier opamps. These are not generated from the +/-100V because a linear regulator would be too lossy and a buck regulator would make the board significantly more complex and expensive.
Likewise, the low 12V supply is used to drive the output amplifier opamp, and is not generated from the other supplies for the same efficiency reason.

If more convenience is required, an external board can generate the 12V and 100V supplies however desired.

### DAC

The DAC circuit takes a serial input via the 74HC595 shift register and sends the output to a DAC0909 DAC chip.

The DAC is 8 bits. Four PCBs can be daisy-chained together (requiring 32 bits).

The DAC should theoretically operate up to about 100MHz, allowing four 8-bit daisy-chained boards to be updated at above 1MHz. This upper speed has not been tested yet and may be limited by inferior PCB routing.

The DAC protocol works (via the 74HC595) as follows:
- Input signals at 3.3V or 5V should both work.
- 4 pins are required (GND, store_clock, shift_clock, and serial_in).
- First, data is sent in via serial_in at each rising edge of the shift_clock.
- Next, when store_clock is brought high, the current state of the data is saved and sent to the DAC.

This should work with any 74HC595 code, e.g., many Arduino libraries.

### Level shifter

The level shifter circuit accepts an input +/-1V analog signal, translates this into a current, and sends the result to the negative high voltage rail, where it is converted back into an analog voltage to drive the output opamp.

The level shifter can be disconnected from the DAC via jumper JP3 in order to input an external analog signal (e.g., from a low-voltage waveform generator) for testing purposes.

Potentiometer RV1 controls the gain of the high voltage output and should be adjusted to give a 100x amplification.

NOTE: THE CURRENT PCB HAS AN ERROR (see "Errata" below) where the source and drain of PMOS transistor Q3 are switched. If this is not fixed during assembly, when the high voltage low rail is turned on, significant current will flow through R17, which will burn out due to excessive power draw.

### High side monitor circuit

This subcircuit provides compensation such that the final high-voltage output is independent of the high voltage supply level (i.e., high voltage inputs of +50V, +100V, and +150V should not affect a 50Vpp output signal).

Potentiometer RV2 controls the DC bias of the output and should be adjusted to center the output at GND = 0V.

Resistor R12=110kohm dissipating up to 200mW was specifically chosen to minimize power draw while also maximizing the voltage/current at the input to opamp U8A (which has nonzero input offset current).

### Output amplifier

This circuit takes an input voltage signal near the negative rail (from the level shifter) and converts this into a current signal through Q1 and R21, which form a class-A amplifier to generate the final high-voltage output. The output voltage is determined by the voltage drop of R21 with the given current.

R21 and Q1, as a class-A amplifier, form a resistive divider between the high voltage power rails, and dissipate a large (10W) current even in their quiescent state in order to drive loads with reasonable accuracy. This means they generate significant heat.

R21 was chosen at 10k as a reasonable compromise between quiescent power draw and load drive ability. R21 can be replaced with a 5kohm power resistor to draw more current but double the output load capacity.

### Output

The final output from R21 and Q1 can be connected to any one of outputs 1 through 4 via jumpers JP8 through JP11.

Four daisy-chained boards can thus be attached to separate outputs.

## Bill of materials

### Part choice notes

- All SMD parts are from JLCPCB and specifically chosen from parts with reliable high stock.

- Hand-soldered components were added manually afterward (not through JLCPCB) to reduce order lead time.

### SMD components (JLCPCB)

| Part no.          | QTY | Cost | JLC type | Note                      | URL                                                                    |
|-------------------|-----|------|----------|---------------------------|------------------------------------------------------------------------|
| 74HC595D,118      |   1 | 0.10 | basic    | shift register            | https://jlcpcb.com/partdetail/Nexperia-74HC595D118/C5947               |
| DAC0808LCMX/NOPB  |   1 | 1.80 | extended | dac                       | https://jlcpcb.com/partdetail/TexasInstruments-DAC0808LCMXNOPB/C132006 |
| NE5532DR          |   2 | 0.15 | basic    | opamp                     | https://jlcpcb.com/partdetail/TexasInstruments-NE5532DR/C7426          |
| LM8261M5X/NOPB    |   2 | 2.12 | extended | opamp                     | https://jlcpcb.com/partdetail/TexasInstruments-LM8261M5XNOPB/C7968     |
| L78M05ABDT-TR     |   1 | 0.13 | basic    | 5V regulator              | https://jlcpcb.com/partdetail/Stmicroelectronics-L78M05ABDTTR/C58069   |
| KT-0603W          |   3 | 0.01 | basic    | white 0603 LED            | https://jlcpcb.com/partdetail/Hubei_KentoElec-KT0603W/C2290            |
| RT0402BRE072K74L  |  12 | 0.02 | extended | 2.74kohm 0402 0.1% 62.5mW | https://jlcpcb.com/partdetail/Yageo-RT0402BRE072K74L/C705647           |
| WR20X750JTL       |   1 | 0.02 | extended | 75ohm 2010 200V           | https://jlcpcb.com/partdetail/Walsin_TechCorp-WR20X750JTL/C172257      |
| 1206W4F1004T5E    |   4 | 0.01 | basic    | 1Mohm 1206 200V 250mW     | https://jlcpcb.com/partdetail/18615-1206W4F1004T5E/C17927              |
| UD2D4R7M0810      |   2 | 0.08 | extended | 4.7uF 200V SMD            | https://jlcpcb.com/partdetail/HonorElec-UD2D4R7M0810/C88701            |
| CC0603KRX7R9BB104 |  12 | 0.01 | basic    | 100nF 0603 50V            | https://jlcpcb.com/partdetail/Yageo-CC0603KRX7R9BB104/C14663           |
| CL31A106KBHNNNE   |  13 | 0.03 | basic    | 10uF 1206 50V             | https://jlcpcb.com/partdetail/14236-CL31A106KBHNNNE/C13585             |
| 2010W2J0114T4S    |   1 | 0.01 | extended | 110kohm 2010 200V 500mW   | https://jlcpcb.com/partdetail/21062-2010W2J0114T4S/C20351              |

### Through-hole components (manually added afterward)

| Part no.       | QTY | Cost  | Note                  | URL                                                                                           |
|----------------|-----|-------|-----------------------|-----------------------------------------------------------------------------------------------|
| 3296Y-1-472LF  |   2 | $1.3  | 4.7kohm trimpot       | https://www.digikey.com/en/products/detail/bourns-inc/3296Y-1-472LF/2536016                   |
| IRF9610PBF     |   1 | $1.1  | PMOS power FET        | https://www.digikey.com/en/products/detail/vishay-siliconix/IRF9610PBF/811768                 |
| IRF730PBF      |   1 | $1.1  | NMOS power FET        | https://www.digikey.com/en/products/detail/vishay-siliconix/IRF730PBF/811793                  |
| QPC02SXGN-RC   |   4 | $0.04 | jumper                | https://www.digikey.com/en/products/detail/sullins-connector-solutions/QPC02SXGN-RC/2618262   |
| THS2510KJ      |   1 | $2.5  | 10kohm power resistor | https://www.digikey.com/en/products/detail/te-connectivity-passive-product/THS2510KJ/2366983  |
| PREC018SABN-RC |   3 | $0.25 | 18-pin male header    | https://www.digikey.com/en/products/detail/sullins-connector-solutions/PREC018SABN-RC/2775036 |
| PPPC181LGBN-RC |   3 | $0.82 | 18-pin female header  | https://www.digikey.com/en/products/detail/sullins-connector-solutions/PPPC181LGBN-RC/775951  |

## Errata and Future Improvements

Board error:
- The PMOS transistor source and drain pins are switched on the PCB. This can be fixed when hand-soldering by bending the MOSFET legs to fit the right holes.

EMI issue:
- I suspect this board might sometimes generate nontrivial RF noise. Qualitative tests show unshielded wires on the output (or maybe even the wires connecting the through-hole power resistor) generate an undesired signal. Thus, be very careful with board use. Future board iterations should fix this issue by, e.g., more careful PCB trace design and changing the signal output connector to a shielded RF SMA connector or similar.

Improvements:
- The first iteration used a 25W chassis-mount power resistor, which worked fine without heatsinking but needed manual wiring to the PCB. To avoid the inconvenience of having a component not directly mounted to the PCB, future iterations might consider using a TO-220 package power resistor and heatsinking instead (e.g., AP836 10K J, https://www.digikey.com/en/products/detail/ohmite/AP836-10K-J/5878547 ).
- A 2-layer PCB was used to decrease manufacturing time for external reasons; switching a 4-layer PCB or better is probably a good idea to allow better EMI mitigation techniques.

## Images

### 3D render

![3D render of PCB](/media/pcb-3d.png)

### Assembled PCBs

![Two assembled PCBs](/media/pcb-assembled.jpg)

### Thermal testing

Note the hottest parts of the circuit are the power resistor and MOSFET near the output, as expected.

![Two assembled PCBs](/media/thermal-camera-view.jpg)

### Frequency response testing

An external function generator was used to generate test waveforms by disconnecting the DAC via jumper JP3.

![frequency response test setup](/media/single-board-test.jpg)

### Daisychain test

![Two boards generating 200Vpp 1kHz waves](/media/pcb-daisychain-test.jpg)

### PCB frame concept

Heatsinks, external wires, and control board not included. Note a large flat heatsink is easy to add along the output power resistors and MOSFETs.

![PCB frame concept](/media/frame.png)

### EMI Issue

Several-foot-long unshielded wires connected to the output and a capacitive load interfere with the signal generator. Note the waveform on the oscilloscope only shows half a sinusoid; the lower half was somehow blocked. The test was immediately stopped.

![EMI discovery](/media/antenna.jpg)

Using a shielded cable reduced interference, but that's still not a perfect sinusoid on the oscilloscope. Recommendation: improve PCB to minimize external wires and other possible EMI sources and use a proper RF SMA connector for the output.

![lower EMI with shielded cable](/media/shielded-antenna.jpg)

