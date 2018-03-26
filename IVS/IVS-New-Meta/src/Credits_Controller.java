
import java.io.IOException;
import java.net.URL;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.Hyperlink;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.control.ToggleButton;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;

/**
 * FXML Controller class
 * @file CreditsController.java
 * @author xcrkon00
 */
public class Credits_Controller implements Initializable {

    @FXML
    private Button closeAbout;
    @FXML
    private ToggleButton creditsButton;
    @FXML
    private Pane bg_pane;
    @FXML
    private TextArea bg_pane_text;
    @FXML
    private TextField dsg;
    @FXML
    private TextField crt;
    @FXML
    private TextField dcm;
    @FXML
    private Hyperlink link;

    /**
     * Initializes the controller class.
     */
    @Override
    public void initialize(URL url, ResourceBundle rb) {
    }    
    /**
     * @brief Link pressed.
     * @param event Represents action of pressing a button
     * Click on link opens wanted URL in browser window.
     */
    @FXML
    private void GNU_link(ActionEvent event) throws IOException {
        
        Process start = new ProcessBuilder("x-www-browser", "https://www.gnu.org/licenses/gpl-3.0.html").start();
    }
    
    /**
     * @brief Credits button pressed.
     * @param event Represents action of pressing a button
     * Make textField with credits visible.
     */
    @FXML
    private void creditsAction(ActionEvent event) {
        if(creditsButton.isSelected() == true) {
            bg_pane_text.setVisible(true);
            dsg.setVisible(true);
            crt.setVisible(true);
            dcm.setVisible(true);
        }
        if(creditsButton.isSelected() == false) {
            bg_pane_text.setVisible(false);
            dsg.setVisible(false);
            crt.setVisible(false);
            dcm.setVisible(false);
        }
    }
    /**
     * @brief Close button pressed.
     * @param event Represents action of pressing a button
     * Close Credits window.
     */
    @FXML
    private void closeAction(ActionEvent event) {
        Stage close_stage = (Stage) closeAbout.getScene().getWindow();
        close_stage.close();
    }
    
}
