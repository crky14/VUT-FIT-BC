
import java.net.URL;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.TextField;
import javafx.stage.Stage;
import javafx.stage.StageStyle;


/**
 * FXML Controller class
 */

/**
 * @file GUI_Controller.java
 * @brief Contains button actions.
 * @author xcrkon00
 */
public class GUI_Controller implements Initializable {

    @FXML
    private TextField display;
    @FXML
    private TextField OP_display;
    private double operand_one, operand_two;
    private boolean dot_flag, reset_D;
    private int operation;
    
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        this.operand_one = 0;
        this.operand_two = 0;
        this.dot_flag = false;
        this.reset_D = false;
        this.operation = 0;
    }
    
    @FXML
    /**
     * @brief Zero button pressed.
     * @param event Represents action of pressing a button
     * Print zero on display. If reset_D is true also reset display.
     */
    private void zeroAction(ActionEvent event) {
        if(reset_D == true) {
           display.setText("");
           reset_D = false;
            
        }
        display.setText(display.getText() + "0");
    }
    
    @FXML
    /**
     * @brief Button '1' pressed.
     * @param event Represents action of pressing a button
     * Print one on display. If reset_D is true also reset display.
     */
    private void oneAction(ActionEvent event) {
        if(reset_D == true) {
           display.setText("");
           reset_D = false;
            
        }
        display.setText(display.getText() + "1");
    }
    
    @FXML
    /**
     * @brief Button '2' pressed.
     * @param event Represents action of pressing a button
     * Print two on display. If reset_D is true also reset display.
     */
    private void twoAction(ActionEvent event) {
        if(reset_D == true) {
           display.setText("");
           reset_D = false;
            
        }       
        display.setText(display.getText() + "2");
    }
    
    @FXML
    /**
     * @brief Button '3' pressed.
     * @param event Represents action of pressing a button
     * Print three on display. If reset_D is true also reset display.
     */
    private void threeAction(ActionEvent event) {
        if(reset_D == true) {
           display.setText("");
           reset_D = false;
            
        }        
        display.setText(display.getText() + "3");
    }
    
    @FXML
    /**
     * @brief Button '4' pressed.
     * @param event Represents action of pressing a button
     * Print four on display. If reset_D is true also reset display.
     */
    private void fourAction(ActionEvent event) {
        if(reset_D == true) {
           display.setText("");
           reset_D = false;
            
        }        
        display.setText(display.getText() + "4");
    }
    
    @FXML
    /**
     * @brief Button '5' pressed.
     * @param event Represents action of pressing a button
     * Print five on display. If reset_D is true also reset display.
     */
    private void fiveAction(ActionEvent event) {
        if(reset_D == true) {
           display.setText("");
           reset_D = false;
            
        }        
        display.setText(display.getText() + "5");
    }
    
    @FXML
    /**
     * @brief Button '6' pressed.
     * @param event Represents action of pressing a button
     * Print six on display. If reset_D is true also reset display.
     */
    private void sixAction(ActionEvent event) {
        if(reset_D == true) {
           display.setText("");
           reset_D = false;
            
        }        
        display.setText(display.getText() + "6");
    }
    
    @FXML
    /**
     * @brief Button '7' pressed.
     * @param event Represents action of pressing a button
     * Print seven on display. If reset_D is true also reset display.
     */
    private void sevenAction(ActionEvent event) {
        if(reset_D == true) {
           display.setText("");
           reset_D = false;
            
        }        
        display.setText(display.getText() + "7");
    }
    
    @FXML
    /**
     * @brief Button '8' pressed.
     * @param event Represents action of pressing a button
     * Print eight on display. If reset_D is true also reset display.
     */
    private void eigthAction(ActionEvent event) {
        if(reset_D == true) {
           display.setText("");
           reset_D = false;
            
        }        
        display.setText(display.getText() + "8");
    }
    
    @FXML
    /**
     * @brief Button '9' pressed.
     * @param event Represents action of pressing a button
     * Print nine on display. If reset_D is true also reset display.
     */
    private void nineAction(ActionEvent event) {
        if(reset_D == true) {
           display.setText("");
           reset_D = false;
            
        }        
        display.setText(display.getText() + "9");
    }
    
    @FXML
    /**
     * @brief Button 'DEL' pressed.
     * @param event Represents action of pressing a button
     * Delete last inserted value on display. If after DEL display would be empty also resets all calculator variables.
     */
    private void DELAction(ActionEvent event) {
        String str = "";
        if(display.getText().length() == 1 && operation != 0) {
            reset();
        }
        if (display.getText() != null && display.getText().length() > 0) {
            str = display.getText().substring(0,display.getText().length()-1);
            if (str.contains(".") == false) {
                dot_flag = false;
            }
        }
        display.setText(str);
    }
    
    @FXML
    /**
     * @brief Button 'C' pressed.
     * @param event Represents action of pressing a button
     * When button 'C' is pressed calls function reset.
     */
    private void CAction(ActionEvent event) {
        reset();
    }
    
    @FXML
    /**
     * @brief Button '=' pressed.
     * @param event Represents action of pressing a button
     * When button '=' is pressed calls function mid_result().
     */
    private void equalAction(ActionEvent event) {
        mid_result();
    }
    
    @FXML
    /**
     * @brief Button '.' pressed.
     * @param event Represents action of pressing a button
     * Prints dot on display. Only one dot can be displayed.
     */
    private void dotAction(ActionEvent event) {
        if(display.getText().length() > 0 && dot_flag == false && (! display.getText().equals("-"))) {
                display.setText(display.getText() + "."); 
                dot_flag = true;
            }
    }
    
    @FXML
    /**
     * @brief Button '+' pressed.
     * @param event Represents action of pressing a button
     * Function load value on display into operand_one.
     * Set operation value on 1.
     * Set reset_D on true.
     * Set dot_flag on false;
     */
    private void plusAction(ActionEvent event) {
        if(display.getText().length() > 0 && (! display.getText().equals("-"))) {
            if(operation != 0) {
                mid_result();
            }
            dot_flag = false;
            reset_D = true;
            operand_one = Double.parseDouble(display.getText()); 
            operation = 1; // ADD 
            OP_display.setText("+");
        }
    }
    
    @FXML
    /**
     * @brief Button '-' pressed.
     * @param event Represents action of pressing a button
     * If display length is zero function prints '-' on display if display is empty.
     * If display length is not zero function load value on display into operand_one.
     * Set operation value on 3.
     * Set reset_D on true.
     * Set dot_flag on false;
     */
    private void minusAction(ActionEvent event) {

        if(display.getText().equals("-")) {
            display.setText("");
        }
        else if(display.getText().length() == 0 || operation != 0) {
            display.setText("-");
            reset_D = false;
        }
        else if((display.getText().length() > 1 && display.getText().contains("-")) || (display.getText().length() > 0 && !(display.getText().contains("-")))) {           
            dot_flag = false;
            reset_D = true;
            operand_one = Double.parseDouble(display.getText()); 
            operation = 3; // SUBSTRACT 
            OP_display.setText("-");
         }
    }
    
    @FXML
    /**
     * @brief Button '%' pressed.
     * @param event Represents action of pressing a button
     * Function load value on display into operand_one.
     * Set operation value on 6.
     * Set reset_D on true.
     * Set dot_flag on false;
     */
    private void moduloAction(ActionEvent event) {
        if(display.getText().length() > 0 && (! display.getText().equals("-"))) {
            if(operation != 0) {
                mid_result();
            }            
            dot_flag = false;
            reset_D = true;
            operand_one = Double.parseDouble(display.getText()); 
            operation = 6; // MODULO
            OP_display.setText("%");
        }
    }
    
    @FXML
    /**
     * @brief Button 'sqrt' pressed.
     * @param event Represents action of pressing a button
     * Function load value on display into operand_one.
     * Set operation value on 7.
     * Set reset_D on true.
     * Set dot_flag on false;
     */
    private void sqrtAction(ActionEvent event) {
        if(display.getText().length() > 0 && (! display.getText().equals("-"))) {
            if(operation != 0) {
                mid_result();
            }            
            dot_flag = false;
            reset_D = true;
            operand_one = Double.parseDouble(display.getText()); 
            operation = 7; // sqrt
            OP_display.setText("√");
        }
    }
    
    @FXML
    /**
     * @brief Button '^' pressed.
     * @param event Represents action of pressing a button
     * Function load value on display into operand_one.
     * Set operation value on 5.
     * Set reset_D on true.
     * Set dot_flag on false;
     */
    private void powAction(ActionEvent event) {
        if(display.getText().length() > 0 && (! display.getText().equals("-"))) {
            if(operation != 0) {
                mid_result();
            }            
            dot_flag = false;
            reset_D = true;
            operand_one = Double.parseDouble(display.getText()); 
            operation = 5; // POW
            OP_display.setText("^");
        }        
    }
    
    @FXML
    /**
     * @brief Button '*' pressed.
     * @param event Represents action of pressing a button
     * Function load value on display into operand_one.
     * Set operation value on 2.
     * Set reset_D on true.
     * Set dot_flag on false;
     */
    private void multiAction(ActionEvent event) {
        if(display.getText().length() > 0 && (! display.getText().equals("-"))) {
            if(operation != 0) {
                mid_result();
            }           
            dot_flag = false;
            reset_D = true;
            operand_one = Double.parseDouble(display.getText()); 
            operation = 2; // MULTIPLY
            OP_display.setText("*");
        }          
    }
    
    @FXML
    /**
     * @brief Button '/' pressed.
     * @param event Represents action of pressing a button
     * Function load value on display into operand_one.
     * Set operation value on 4.
     * Set reset_D on true.
     * Set dot_flag on false;
     */
    private void divAction(ActionEvent event) {
        if(display.getText().length() > 0 && (! display.getText().equals("-"))) {
            if(operation != 0) {
                mid_result();
            }           
            dot_flag = false;
            reset_D = true;
            operand_one = Double.parseDouble(display.getText()); 
            operation = 4; // DIVIDE
            OP_display.setText("/");
        } 
    }
    
    @FXML
    /**
     * @brief Button '!' pressed.
     * @param event Represents action of pressing a button
     * Set operation value on 8.
     * Function load value on display into operand_one and call mid_result().
     * Set reset_D on true.
     * Set dot_flag on false; 
     */
    private void factAction(ActionEvent event) {
        if(display.getText().length() > 0 && (! display.getText().equals("-"))) {        
            dot_flag = false;
            reset_D = true;
            operand_one = Double.parseDouble(display.getText()); 
            operation = 8;
            mid_result();
        } 
    }
    
    @FXML
    /**
     * @brief Button '?' pressed.
     * @param event Represents action of pressing a button
     * Function opens HELP window.
     */
    private void helpAction(ActionEvent event) {
        try {
            FXMLLoader helpLoader = new FXMLLoader(getClass().getResource("Help.fxml"));
            Parent root1 = (Parent) helpLoader.load();
            Stage help;
            help = new Stage();
            help.initStyle(StageStyle.UNDECORATED);
            help.setScene(new Scene(root1));  
            help.show();
        } catch(Exception e) {
           e.printStackTrace();
          }
    }
    /**
     * @brief Function print result of selected operation on display.
     * Depending on value in operation, function calls wanted method from math library.
     * Function use value of operand_one as first operand , load number on display into 
     * operand_two and use it as second operand for selected operation.
     * Check if number is integer or floating point number and print value on display.
     */
    private void mid_result() {
        if((operation <= 7) && (operation > 0)) {
            operand_two = Double.parseDouble(display.getText());
            double result = 0;
            switch(operation) {
                case 1: result = math.add(operand_one, operand_two);
                        break;
                case 2: result = math.multiply(operand_one, operand_two);
                        break;
                case 3: result = math.sub(operand_one, operand_two);
                        break;
                case 4: if(operand_two != 0) {
                            result = math.divide(operand_one, operand_two);
                        } else{
                            display.setText("Err");
                        }
                        break;
                case 5: if(is_int(operand_two) >= 0){
                            result = math.pow(operand_one, (int)operand_two);
                        }else {
                            display.setText("Err");
                        }
                        break;
                case 6: if(is_int(operand_one) >= 0 && is_int(operand_two) >= 0){
                            result = math.mod((int)operand_one, (int)operand_two); 
                        }else{
                            display.setText("Err");
                        }
                        break;
                case 7: result = math.root(operand_one, operand_two);
                        if(result == -1) {
                            display.setText("Err");
                        }
                        break;
            }
            if(!"Err".equals(display.getText())) {
                if(is_int(result) >= 0) {
                    display.setText(String.valueOf((int)result));
                }else {
                    display.setText(String.valueOf(result));
                }
            }
        }
        if(operation == 8) {
            if(is_int(operand_one) >= 0){
                display.setText(String.valueOf(math.factorial((long)operand_one)));
            }else{
                display.setText("Err");
            }
        }
        dot_flag = false;
        if(display.getText().contains(".") == true) {
            dot_flag = true;
        }
        reset_D = false;
        OP_display.setText("=");
        operation = 0;
    }
    /**
     * @brief Function will chceck whether the given number is integer or not.
     * @param x Number to be checked. 
     * @return Integer representation of number if number was x.0, or -1 if not.
     */
    private long is_int(double x) {
        if(x % 1 == 0){
            return (long)x;
        }else {
            return -1;
        }
    }
    /**
     * @brief Fucntion resets calculator display and all needed calculator variables
     */
    private void reset() {
        display.setText("");
        OP_display.setText("");
        operand_one = 0;
        operand_two = 0;
        operation = 0;
        dot_flag = false;
        reset_D = false;
    }
}
