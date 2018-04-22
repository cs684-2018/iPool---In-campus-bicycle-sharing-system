# Description
Assumptions and explanation of each program"

## 1. Car headlight controller
### Assumption
Mode is given as 2 bit binary input (mode_bit_1 and mode_bit_0).
In flashing mode, light is hi for 10 cycles then goes low and stays low until next mode change.
### Execution command
`luciole-rif lus2lic car.lus -node headlight -exec`

## 2. Traffic light controller
### Assumption
Road and traffic flow is as shown in diagram. Traffic flows only from West to East and from South to North.
       | ^ |
       | | |
       | | |
_______| | |_______
------->    ------>
_______     _______
       | ^ |
       | | |
       | | |
       | | |
variable definitions: lane_SN is the lane going from South to North. lane_WE is the lane going from West to East
* Inputs
- lane_SN_sensor: Input when sensor at lane_SN is triggered
- lane_WE_sensor: Input when sensor at lane_WE is triggered
* Outputs
- lane_SN_R: lane_SN red signal
- lane_SN_G: lane_SN green signal
- lane_WE_R: lane_WE red signal
- lane_WE_G: lane_WE green signal
### Execution command
`luciole-rif lus2lic traffic.lus -node signal -exec`

## 3. Mod12 counter using JK flipflop
### Assumption
2 nodes have been created 
* For JK flip flop with J, K and clock as input and Q, and Q_(Q bar) as output
* For mod12 counter with only clock as input and Q0,Q1,Q2,Q3 as output.

### Execution command
`luciole-rif lus2lic flipflop.lus -node modulo12counter -exec`

## 4. Mealy Machine
### Assumption
One node has been created which takes boolean input and gives next state and output.

### Execution command
`luciole-rif lus2lic mealy.lus -node mealy -exec`

