# software components
# managing switch/pot state
the circuit includes 5 2-way switches and 4 potentiometers. the readings of these components are stored as global state in the `switches` struct to determine the display's behavior. 
### storing state
- each switch's state is stored in a bitfield 
    - BIT 0: power switch
    - BIT 1: mode switch
    - BIT 2: reverse switch
    - BIT 3: div switch
    - BIT 4: rgb switch
- the brightness and speed pots are not stored as state - these use the PWM channels to get the reading & set the appropriate brightness/speed
- the 'div' pot uses the analog reading to pass different values to the `num_sr` parameter in the `chaser` function. this modifies the number of lights lit at once
    - the function that reads this pot returns a 1, 2, or 3. its value is stored as state in `BITS 5 and 6` of the state byte