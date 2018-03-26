/**
 * @file Calculator.java
 * @brief Main class - Calculator
 * @author xcrkon00
 */
import javafx.application.Application;
import static javafx.application.Application.launch;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;


public class Calculator extends Application {
    
    @Override
    public void start(Stage stage) throws Exception {
        long result = math.factorial(10);
        double result1 = math.root(512,33);
        Parent root = FXMLLoader.load(getClass().getResource("GUI.fxml"));
        Scene scene = new Scene(root);
        stage.setResizable(false);
        stage.setScene(scene);
        stage.show();
    }
    /**
     * @brief Main function of the calculator.
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        launch(args);
    }
    
}
