import org.junit.Assert;
import org.junit.Test;

import static edu.gvsu.dlunit.DLUnit.*;

public class MyUnsigned16BitAdderTest {

	protected void verify(long a, long b, boolean carryIn) {
		long expected = a + b + (carryIn ? 1 : 0);
		boolean overflow = expected > 65535;

		setPinUnsigned("InputA", a);
		setPinUnsigned("InputB", b);
		setPin("CarryIn", carryIn);
		run();

		String message = "of " + a + " + " + b + " with " + (carryIn ? "a " : " no ") + " carry in";

		Assert.assertEquals("Output " + message, (expected % 65536), readPinUnsigned("Output"));
		Assert.assertEquals("CarryOut " + message, overflow, readPin("CarryOut"));
	}

	@Test
	public void testAll() {
		for (long a=0; a < Math.pow(2, 16); a++) {
			for (long b=0; b < Math.pow(2, 16); b++) {
				verify(a, b, true);
				verify(a, b, false);
			}
			
			System.out.printf("%f%% Done", a/Math.pow(2, 16));
		}
	
		System.out.println("testAll ran ALL TESTS");
  	}
}
