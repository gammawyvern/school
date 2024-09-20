import org.junit.Assert;
import org.junit.Test;

import static edu.gvsu.dlunit.DLUnit.*;

public class Signed16BitAddSubTests {

	// Helper method that runs a test for a given pair of integers and an operation (`false` for add, `true` for subtract)
	private void verify(long a, long b, boolean op) {

		/*******************************
		 * Compute the expected outputs
		 ******************************/
		
		long expected = op ? a - b : a + b;  // add or subtract as specified by `op`
    		boolean expectedOverflow = ((expected >= (1 << 15)) || (expected < -(1 << 15)));

		// Output "wraps around" if there is an overflow
		if (expectedOverflow && expected > 0) {
			expected -= 65536;
		} else if (expectedOverflow && expected < 0) {
			expected += 65536;
		}

		/*******************************
		 * Configure and simulate
		 * the circuit
		 ******************************/
		
		setPinSigned("InputA", a);
		setPinSigned("InputB", b);
		setPin("Op", op);
		run();

		/*******************************
		 * Check the correctness
		 * of the output
		 ******************************/

		String message = "of " + a + (op ? " - " : " + ") + b + ": ";
		Assert.assertEquals("Output " + message, expected, readPinSigned("Output"));
		Assert.assertEquals("Overflow " + message, expectedOverflow, readPin("Overflow"));
	}



	// Critical numbers to test (for 16 bits)
	public static final long testIntegers[] = {
		-32768, -32767, 0, 1, 2, 13, 127, 
		128, 129, 0x5555, 32766, 32767
	};



  	// This is actually rather gross; but, it is an effective way to thoroughly test your circuit without
  	// having to write hundreds of individual methods.
 
	@Test
	public void testAll() {
	for (long a : testIntegers) {
		for (long b : testIntegers) {
				verify(a, b, false);
				verify(a, b, true);
			}
		}
  	}
}
