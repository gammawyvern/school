import org.junit.Assert;
import org.junit.Test;

import static edu.gvsu.dlunit.DLUnit.*;
import static edu.gvsu.dlunit.DLUnit.readPin;

public class Slt16BitTests {

	public void verify(long a, long b, boolean signed) {
		if (signed) {
			setPinSigned("InputA", a);
			setPinSigned("InputB", b);
		} else {
			setPinUnsigned("InputA", a);
			setPinUnsigned("InputB", b);
		}

		setPin("Signed", signed);
		run();

		/*******************************
		 * Check the correctness of
		 * the output
		 ******************************/

		String message = String.format("%d < %d (%s): ", a, b, signed ? "signed" : "unsigned");
		Assert.assertEquals("Output " + message, a < b, readPin("LessThan"));
	}


  	/*******************************
   	* My own tests with many
	* critical numbers
	*******************************/

	// Critical signed numbers (for 16 bits)
	private int[] signedNums = {
		-32768, 32767, 
		-32767, 32766, 
		-1, 0, 1,
		452, -827, -64, 64,
		128, 129, 127,
		-128, -129, -127
	};

	// Critical unsigned numbers (for 16 bits)
	private int[] unsignedNums = {
		0, 64, 7248, 128,
		65535, 65534,
		30000, 50000,
		128, 129, 127
	};

  	// Verify all possible combinations of numbers
	// from the list of signed numbers

	@Test
	public void many_signed_tests() {
		  for(int num_one : signedNums) {
			for(int num_two : signedNums) {
				verify(num_one, num_two, true);
			}
		}
	}

  	// Verify all possible combinations of numbers
  	// from the list of unsigned numbers
  
	@Test
	public void many_unsigned_tests() {
		for(int num_one : unsignedNums) {
			for(int num_two : unsignedNums) {
				verify(num_one, num_two, false);
			}
		}
	}
}
