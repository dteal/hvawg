# High Voltage Arbitrary Waveform Generator

This is the first iteration of a modular arbitrary waveform generator intended to generate possibly tens of separate ±100V 1MHz signals for small loads.

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
- The first iteration used a 25W chassis-mount power resistor, which worked fine without heatsinking but needed manual wiring to the PCB. To avoid the inconvenience of having a component not directly mounted to the PCB, future iterations might consider using a TO-220 package power resistor and heatsinking instead (e.g., AP836 10K J, https://www.digikey.com/en/products/detail/ohmite/AP836-10K-J/5878547 )

