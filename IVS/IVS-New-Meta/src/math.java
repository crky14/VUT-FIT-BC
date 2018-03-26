
import static java.lang.Math.round;
import static java.lang.Math.sqrt;
import java.math.BigDecimal;


/**
 * @file math.java
 * @author xblask04
 * @brief Class with mathematic operation used in calculator.
 */
public class math {
    /**
     * @brief Function will add two double numbers.
     * @param x first number for operation
     * @param y second number for operation
     * @return result of adding two parameters
     */
    public static double add(double x, double y){
        return x+y;
    }
    
    /**
     * @brief Function will add two integer numbers.
     * @param x first number for operation
     * @param y second number for operation
     * @return result of adding two parameters
     */
    public static long add(long x, long y){
        return x+y;
    }
    
    /**
     * @brief Function will add two numbers - one integer and one double.
     * @param x first number for operation
     * @param y second number for operation
     * @return result of adding two parameters
     */
    public static double add(double x, long y){
        return x+y;
    }
    
    /**
     * @brief Function will add two numbers - one integer and one double.
     * @param x first number for operation
     * @param y second number for operation
     * @return result of adding two parameters
     */
    public static double add(long x, double y){
        return x+y;
    }
    
    /**
     * @brief Substraction of two numbers
     * @param x first number for operation
     * @param y second number for operation
     * @return result of substracting two numbers
     */
    public static double sub(double x, double y){
        BigDecimal x2, y2, result ;
        x2 = new BigDecimal(String.valueOf(x));
        y2 = new BigDecimal(String.valueOf(y));
        result = x2.subtract(y2);
        double res = result.doubleValue();
        return res;
    }
    
    /**
     * @brief Function will multiply two double numbers.
     * @param x first number for operation
     * @param y second number for operation
     * @return result of multiplying two numbers
     */
    public static double multiply(double x, double y){
        return x*y;
    }
    /**
     * @brief Function will multiply two integer numbers.
     * @param x first number for operation
     * @param y second number for operation
     * @return result of multiplying two numbers
     */
    public static long multiply(long x, long y){
        return x*y;
    }
    /**
     * @brief Function will multiply two numbers - one double and one integer.
     * @param x first number for operation
     * @param y second number for operation
     * @return result of multiplying two numbers
     */
    public static double multiply(long x, double y){
        return x*y;
    }
    /**
     * @brief Function will multiply two numbers - one double and one integer.
     * @param x first number for operation
     * @param y second number for operation
     * @return result of multiplying two numbers
     */
    public static double multiply(double x, long y){
        return x*y;
    }
    
    /**
     * @brief Division of two numbers
     * @param x divident
     * @param y divisor
     * @return result of dividing two numbers
     */
    public static double divide(double x, double y){
        return x/y;
    }
    
    /**
     * @brief Function will calculate a factorial of a given number.
     * @param num number from which we want factorial to be calculated from
     * @return value of the factorial
     */
    public static long factorial(long num){
        int check = 0;
        if(num < 0){
            check = 1;
            num = -num;
        }
        long sum = 1;
        for (long i = num; i > 0; i--){
            sum = sum*i;
        }
        if(check == 1){
            sum = -sum;
        }
        return sum;
    }
    
    /**
     *
     * @param x floating point base value
     * @param y power value
     * @return result of raising x to the power y
     */
    public static double pow(double x, int y){
        double sum = 1;
        for(int i = 0; i < y; i++){
            sum = sum*x;
        }
        return sum;
    }
    
        /**
     *
     * @param x floating point base value
     * @param y power value
     * @return result of raising x to the power y
     */
    public static int pow(int x, int y){
        int sum;
        sum = 1;
        for(int i = 0; i < y; i++){
            sum = sum*x;
        }
        return sum;
    }
    
    /**
     * @brief Modulo function
     * @param x divident
     * @param y divisor
     * @return the remainder from division
     */
    public static long mod(long x, long y){
        return x%y;
    }
    
    /**
     * @brief Sqrt function.
     * @param x number to be rooted
     * @param root root of the number
     * @return result of operation
     */
    public static double root(double x, double root) {
        double result;
        if( x >= 0 || root % 2 == 0) {
            return Math.pow(x, 1.0 / root);
        } else {
            x = -x;
            result = Math.pow(x, 1.0 / root);
            result = -result;
            return result;
        }
    }
}
