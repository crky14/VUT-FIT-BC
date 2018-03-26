
import static java.lang.Double.NaN;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.Test;
import static org.junit.Assert.*;
import org.junit.Before;
import org.junit.BeforeClass;

/**
 *
 * @author xkosti05
 * @file mathTest.java
 * @brief Tests for math.java library
 */
public class mathTest {
    
    public mathTest() {
    }

    @BeforeClass
    public static void setUpClass() throws Exception {
    }

    @AfterClass
    public static void tearDownClass() throws Exception {
    }

    @Before
    public void setUp() throws Exception {
    }

    @After
    public void tearDown() throws Exception {
    }

   /**
     * Test of add method, of class math. 
     */
    @Test
    public void testAdd() {
        System.out.println("add");
        assertEquals(2.0, math.add(1.0, 1.0), 0.00);       
        assertEquals(0, math.add(0, 0), 0.00);
        assertEquals(-5, math.add(-5, 0), 0.00);
        assertEquals(275, math.add(-5, 280), 0.00);
        assertEquals(-17, math.add(-9, -8), 0.00);
        assertEquals(9.6745, math.add(5.3895, 4.285), 0.01);
        assertEquals(0.0, math.add(0.0, 0.0), 0.00);
        assertEquals(-10130, math.add(-57987, 47857), 0.00);          
    }

    /**
     * Test of sub method, of class math.
     */
    @Test
    public void testSub() {
        System.out.println("sub");
        assertEquals(0.0, math.sub(1.0, 1.0), 0.00);       
        assertEquals(0.0, math.sub(0, 0), 0.00);
        assertEquals(-5.0, math.sub(-5, 0), 0.00);
        assertEquals(-285.0, math.sub(-5, 280), 0.00);
        assertEquals(-1.0, math.sub(-9, -8), 0.00);
        assertEquals(1.1045, math.sub(5.3895, 4.285), 0.00);
        assertEquals(3812130.0, math.sub(8557987, 4745857), 0.00);
    }

    /**
     * Test of multiply method, of class math.
     */
    @Test
    public void testMultiply() {
        System.out.println("multiply");
        assertEquals(1.0, math.multiply(1.0, 1.0), 0.00);       
        assertEquals(0.0, math.multiply(0, 0), 0.00);
        assertEquals(0.0, math.multiply(-5, 0), 0.00);
        assertEquals(-1400.0, math.multiply(-5, 280), 0.00);
        assertEquals(72.0, math.multiply(-9, -8), 0.00);
        assertEquals(23.0940075, math.multiply(5.3895, 4.285), 0.00);
        assertEquals(46779859.0, math.multiply(7987, 5857), 0.00);
    }

    /**
     * Test of divide method, of class math.
     */
    @Test
    public void testDivide() {
        System.out.println("divide");
        assertEquals(1.0, math.divide(1.0, 1.0), 0.00);       
        assertEquals(-0.017857142866, math.divide(-5, 280), 0.0000001);
        assertEquals(3.0, math.divide(-9, -3), 0.00);
        assertEquals(1.257759627, math.divide(5.3895, 4.285), 0.000001);
        assertEquals(13770.68966, math.divide(7987, 0.58), 0.001);
    }

    /**
     * Test of factorial method, of class math.
     */
    @Test
    public void testFactorial() {
        System.out.println("factorial");
        assertEquals(1, math.factorial(1));       
        assertEquals(1, math.factorial(0));
        assertEquals(2, math.factorial(2));
        assertEquals(40320, math.factorial(8));
        assertEquals(-40320, math.factorial(-8));
        assertEquals(-1, math.factorial(-1));
    }

    /**
     * Test of pow method, of class math.
     */
    @Test
    public void testPow() {
        System.out.println("pow");
        assertEquals(1.0, math.pow(1.0, 1), 0.00);       
        assertEquals(1, math.pow(-5, 0), 0.00);
        assertEquals(9765625, math.pow(-5, 10), 0.00);
        assertEquals(-1953125, math.pow(-5, 9), 0.00);
        assertEquals(843.7113763, math.pow(5.3895, 4), 0.000001); 
    }
  

    /**
     * Test of mod method, of class math.
     */
    @Test
    public void testMod() {
        System.out.println("mod");
        assertEquals(0, math.mod(1, 1), 0.00);       
        assertEquals(2, math.mod(37, 5), 0.00);
        assertEquals(2, math.mod(2, 5), 0.00);
    }

    /**
     * Test of add method, of class math. Double, Double 
     */
    @Test
    public void testAdd_double_double() {
        System.out.println("add");
         assertEquals(2.0, math.add(1.0, 1.0), 0.00);       
        assertEquals(0, math.add(0, 0), 0.00);
        assertEquals(-5, math.add(-5, 0), 0.00);
        assertEquals(275, math.add(-5, 280), 0.00);
        assertEquals(-17, math.add(-9, -8), 0.00);
        assertEquals(9.6745, math.add(5.3895, 4.285), 0.00);
        assertEquals(0.0, math.add(0.0, 0.0), 0.00);
        assertEquals(-10130, math.add(-57987, 47857), 0.00); 
    }

    /**
     * Test of add method, of class math. Long, Long
     */
    @Test
    public void testAdd_long_long() {
        System.out.println("add");
        assertEquals(2, math.add(1, 1), 0.00);       
        assertEquals(0, math.add(0, 0), 0.00);
        assertEquals(-5, math.add(-5, 0), 0.00);
        assertEquals(275, math.add(-5, 280), 0.00);
        assertEquals(-17, math.add(-9, -8), 0.00);
        assertEquals(0, math.add(0, 0), 0.00);
        assertEquals(-10130, math.add(-57987, 47857), 0.00); 
    }

    /**
     * Test of add method, of class math. Double, Long
     */
    @Test
    public void testAdd_double_long() {
        System.out.println("add");
         assertEquals(2.0, math.add(1.0, 1), 0.00);       
        assertEquals(0.0, math.add(0.0, 0), 0.00);
        assertEquals(-5.0, math.add(-5.0, 0), 0.00);
        assertEquals(275.0, math.add(-5.0, 280), 0.00);
        assertEquals(-17.0, math.add(-9.0, -8), 0.00);
        assertEquals(0.0, math.add(0.0, 0), 0.00);
        assertEquals(-10130.0, math.add(-57987.0, 47857), 0.00); 
    }

    /**
     * Test of add method, of class math. Long, Double
     */
    @Test
    public void testAdd_long_double() {
        System.out.println("add");
         assertEquals(2.0, math.add(1, 1.0), 0.00);       
        assertEquals(0.0, math.add(0, 0.0), 0.00);
        assertEquals(-5.0, math.add(-5, 0.0), 0.00);
        assertEquals(275.0, math.add(-5, 280.0), 0.00);
        assertEquals(-17.0, math.add(-9, -8.0), 0.00);
        assertEquals(0.0, math.add(0, 0.0), 0.00);
        assertEquals(-10130.0, math.add(-57987, 47857.0), 0.00); 
    }

    /**
     * Test of multiply method, of class math. Double, Double
     */
    @Test
    public void testMultiply_double_double() {
        System.out.println("multiply");
        assertEquals(1.0, math.multiply(1.0, 1.0), 0.00);       
        assertEquals(0.0, math.multiply(0.0, 0.0), 0.00);
        assertEquals(0.0, math.multiply(-5.0, 0.0), 0.00);
        assertEquals(-1400.0, math.multiply(-5.0, 280.0), 0.00);
        assertEquals(72.0, math.multiply(-9.0, -8.0), 0.00);
        assertEquals(23.0940075, math.multiply(5.3895, 4.285), 0.00);
        assertEquals(46779859.0, math.multiply(7987.0, 5857.0), 0.00);
    }

    /**
     * Test of multiply method, of class math. Long, Long
     */
    @Test
    public void testMultiply_long_long() {
        System.out.println("multiply");
        assertEquals(1, math.multiply(1, 1), 0.00);       
        assertEquals(0, math.multiply(0, 0), 0.00);
        assertEquals(0, math.multiply(-5, 0), 0.00);
        assertEquals(-1400, math.multiply(-5, 280), 0.00);
        assertEquals(72, math.multiply(-9, -8), 0.00);
        assertEquals(46779859, math.multiply(7987, 5857), 0.00);
    }

    /**
     * Test of multiply method, of class math. Long, Double
     */
    @Test
    public void testMultiply_long_double() {
        assertEquals(1.0, math.multiply(1, 1.0), 0.00);       
        assertEquals(0.0, math.multiply(0, 0.0), 0.00);
        assertEquals(0.0, math.multiply(-5, 0.0), 0.00);
        assertEquals(-1400.0, math.multiply(-5, 280.0), 0.00);
        assertEquals(72.0, math.multiply(-9, -8.0), 0.00);
        assertEquals(46779859.0, math.multiply(7987, 5857.0), 0.00);
    }

    /**
     * Test of multiply method, of class math. Double, Long
     */
    @Test
    public void testMultiply_double_long() {
        assertEquals(1.0, math.multiply(1.0, 1), 0.00);       
        assertEquals(0.0, math.multiply(0.0, 0), 0.00);
        assertEquals(0.0, math.multiply(-5.0, 0), 0.00);
        assertEquals(-1400.0, math.multiply(-5.0, 280), 0.00);
        assertEquals(72.0, math.multiply(-9.0, -8), 0.00);
        assertEquals(46779859.0, math.multiply(7987.0, 5857), 0.00);
    }

    /**
     * Test of pow method, of class math. Double, Int
     */
    @Test
    public void testPow_double_int() {
        System.out.println("pow");
        assertEquals(1, math.pow(1.0, 1), 0.00);       
        assertEquals(1, math.pow(-5.0, 0), 0.00);
        assertEquals(9765625, math.pow(-5.0, 10), 0.00);
        assertEquals(-1953125, math.pow(-5.0, 9), 0.00);
    }

    /**
     * Test of pow method, of class math. Int, Int
     */
    @Test
    public void testPow_int_int() {
        assertEquals(1, math.pow(1, 1), 0.00);       
        assertEquals(1, math.pow(-5, 0), 0.00);
        assertEquals(9765625, math.pow(-5, 10), 0.00);
        assertEquals(-1953125, math.pow(-5, 9), 0.00);
    }

    /**
     * Test of root method, of class math.
     */
    @Test
    public void testRoot() {
        assertEquals(4, math.root(16, 2), 0.0); 
        assertEquals(0, math.root(0, 2), 0.0); 
        assertEquals(0, math.root(0, 3), 0.0); 
        assertEquals(0, math.root(0, 8), 0.0); 
        assertEquals(1, math.root(1, 2), 0.0); 
        assertEquals(1, math.root(1, 3), 0.0); 
        assertEquals(1, math.root(1, 8), 0.0);
        assertEquals(2, math.root(256, 8), 0.0);

        assertEquals(-1, math.root(-1, 3), 0.0);
        assertEquals(-2, math.root(-8, 3), 0.0);
        
        assertEquals(NaN, math.root(-5, 8), 0.0);
        assertEquals(NaN, math.root(-5, 2), 0.0);

    }
    
}

/*** End of file ***/
