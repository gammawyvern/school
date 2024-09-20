## Total Number Of Gates:
* FullAdder = 5
* Multiplexer = 4
* OverflowDetector = 5
* 4bit-Ripple = (FullAdder * 4) = 20 
* 4bit-CarrySelect = (4bit-Ripple * 2) + (Multiplexer * 5) = 40 + 20 = 60 
* Signed-Adder = (4bit-CarrySelect * 4) + OverflowDetector = 240 + 5 = 245

Altogether:
* Our SignedAdder_16bit.jls has 245 gates

## Crital Path Timing
* The data goes immediately from InputA/B to each Carry Select Adder, to
	each 4bit ripple adder, to each FA (there are no gates). 
* Each full adder then runs the first set of gates in parallel (10).
* Each carry in each full adder in the 4bit ripple adder then ripples through (4*10).
* Now each 4bit ripple adder is finished, so next the carry select bit must ripple through
	each multiplexor, and there are 4 4bit carry select adders (4*10).
* Last, the output sum must finish going through the overflow detector (20).

Altogether:
* (10) + (4*10) + (4*10) + (20) = 110
* Our SignedAdder_16bit.jls has a critical path of 110
