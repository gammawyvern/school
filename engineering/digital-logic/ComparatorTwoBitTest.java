import org.junit.Assert;
import org.junit.Test;

import static edu.gvsu.dlunit.DLUnit.*;

public class ComparatorTwoBitTest {

	@Test
	public void AllTests() {

		// 0 0
		setPinUnsigned("InputA", 0);
		setPinUnsigned("InputB", 0);
		run();
		Assert.assertEquals(0, readPinUnsigned("Output"));

		// 0 1
		setPinUnsigned("InputA", 0);
		setPinUnsigned("InputB", 1);
		run();
		Assert.assertEquals(1, readPinUnsigned("Output"));
		
		// 0 2
		setPinUnsigned("InputA", 0);
		setPinUnsigned("InputB", 2);
		run();
		Assert.assertEquals(1, readPinUnsigned("Output"));
		
		// 0 3
		setPinUnsigned("InputA", 0);
		setPinUnsigned("InputB", 3);
		run();
		Assert.assertEquals(1, readPinUnsigned("Output"));
		
		// 1 0
		setPinUnsigned("InputA", 1);
		setPinUnsigned("InputB", 0);
		run();
		Assert.assertEquals(0, readPinUnsigned("Output"));
		
		// 1 1
		setPinUnsigned("InputA", 1);
		setPinUnsigned("InputB", 1);
		run();
		Assert.assertEquals(0, readPinUnsigned("Output"));
		
		// 1 2
		setPinUnsigned("InputA", 1);
		setPinUnsigned("InputB", 2);
		run();
		Assert.assertEquals(1, readPinUnsigned("Output"));
		
		// 1 3
		setPinUnsigned("InputA", 1);
		setPinUnsigned("InputB", 3);
		run();
		Assert.assertEquals(1, readPinUnsigned("Output"));
		
		// 2 0
		setPinUnsigned("InputA", 2);
		setPinUnsigned("InputB", 0);
		run();
		Assert.assertEquals(0, readPinUnsigned("Output"));
		
		// 2 1
		setPinUnsigned("InputA", 2);
		setPinUnsigned("InputB", 1);
		run();
		Assert.assertEquals(0, readPinUnsigned("Output"));
		
		// 2 2
		setPinUnsigned("InputA", 2);
		setPinUnsigned("InputB", 2);
		run();
		Assert.assertEquals(0, readPinUnsigned("Output"));
		
		// 2 3
		setPinUnsigned("InputA", 2);
		setPinUnsigned("InputB", 3);
		run();
		Assert.assertEquals(1, readPinUnsigned("Output"));
		
		// 3 0
		setPinUnsigned("InputA", 3);
		setPinUnsigned("InputB", 0);
		run();
		Assert.assertEquals(0, readPinUnsigned("Output"));
		
		// 3 1
		setPinUnsigned("InputA", 3);
		setPinUnsigned("InputB", 1);
		run();
		Assert.assertEquals(0, readPinUnsigned("Output"));
		
		// 3 2
		setPinUnsigned("InputA", 3);
		setPinUnsigned("InputB", 2);
		run();
		Assert.assertEquals(0, readPinUnsigned("Output"));
		
		// 3 3
		setPinUnsigned("InputA", 3);
		setPinUnsigned("InputB", 3);
		run();
		Assert.assertEquals(0, readPinUnsigned("Output"));
	}

}
