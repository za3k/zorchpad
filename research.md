Power Usage

- Power usage will be around 0.3mA @ 3V when active (0.001 W = 0.001 J/s)
- Power usage will be around 1nA @ 3V when asleep (0.000000003 W = 0.000003 J/s)

- On 2024-08-20 I tested an e-ink power supply (Waveshare 8" display) and tried updating one character at a time, watching an oscilloscope to measure power roughly. I calculated that a refresh takes 0.0375J (4ms x 250 mA x 5V) -- affordable once per 37s. I did not test cleanly how much changing different fractions of the screen affected the power.

Continuous Power Sources

- Solar
    - 20-100 lumens per watt ??
    - ~1M lux = 1W/cm^2, ~1000 lux = 1mW/cm^2 ??
    - Outdoor, daylight
        0.138 W/cm^2
    - Outdoor, night
    - Outdoor, night (IR from earth)
        Undeveloped tech
    - Indoor, electric light
        3.5-30 uW per cm^2 [source](https://blog.powerfilmsolar.com/education/the-horizon-blog/2018/8/10/outdoor-vs-indoor-solar-the-key-differences). That's 4,000x-40,000x lower than daylight. Call it 10,000x lower.
- Amorphous silicon (calculator solar cell) - 7% efficient. 0.24W per 5cm square in daylight. 0.024 mW indoors.
- Monocrystaline silicon (standard solar cell) - 15% efficient, 0.5W per 5cm square in daylight. 0.05 mW indoors.
- Keyboard typing power
- Peltier heat differential
- Tritium battery (40 W/kg, half life 12.3 years) or other betavoltaics - Maybe 1μW
- Inductive powerline charging

Batteries

- [Lithium coin cell battery life](https://www.embedded.com/how-much-energy-can-you-really-get-from-a-coin-cell/) by Jack Ganssle, 2014. Mentions internal resistance of batteries go up as they discharge.
    225 mAh = 0.225Ah = (0.225A) * (3600s) * (2-3V) = 1620-2430 J
    Shelf life? Perhaps 10 years?

    **Runtime: 18 days active, 17,000 years inactive**
- 18650 standard lithium ion (lipo) battery.

    [source](https://nathanruffing.com/wtw22/)
    ```
    1 18659 Li-ion cell = 43,074 J
    1 Calorie = 4,180 J
    1 smartphone battery charge: 41,750 J
    1 BTU = 1,060 J
    1 kBTU = 1,000,060 J
    1 watt-hour = 3,600 J
    1 kilowatt-hour = 3,600,000 J
    2,000 Calories = 8,400,000 J
    1 gallon of gas: 120,000,000 J
    ```

    18650 battery: 43,074 J 

    **Runtime: 1.3 years active, 455,000 years inactive**
- Capacitor energy
    Charge (coulombs) is capacitance (farads=coulomb/volt) times volts: Q = F V
    Energy is joules = 0.5 Q V = 0.5 F V V
        where V is voltage and F is capacitance in farads
    The biggest capacitor I have is
        1000uF, rated to 16V. This stores 0.256 J

    Runtime: 4 minutes active, 23 hours deep sleep mode

    Online, I can get a 2.7V, 1F capacitor. This stores 7.29J.
    Runtime: 20 minutes active, 15 days deep sleep mode

    Online, I can get a 25V, 1F capacitor. This stores 625J.
    Cost ≤ $0.50
    **Runtime: 7 day active, 19 year deep sleep mode**
