/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.net.URL;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.stage.Stage;
import javafx.stage.StageStyle;


public class Help_controller implements Initializable {

    @FXML
    private Button closeWindow;
 
    @Override
    public void initialize(URL url, ResourceBundle rb) {
    }    
    /**
     * @brief Button 'About' pressed.
     * @param event Represents action of pressing a button
     * Function close Help window.
     */
    @FXML
    private void closeAction(ActionEvent event) {
        Stage close_stage = (Stage) closeWindow.getScene().getWindow();
        close_stage.close();
    }
    /**
     * @brief Button 'About' pressed.
     * @param event Represents action of pressing a button
     * Function opens Credits window.
     */
    @FXML
    private void creditsAction(ActionEvent event) {
        try {
            FXMLLoader CreditsLoader = new FXMLLoader(getClass().getResource("Credits.fxml"));
            Parent root2 = (Parent) CreditsLoader.load();
            Stage help;
            help = new Stage();
            help.initStyle(StageStyle.UNDECORATED);
            help.setScene(new Scene(root2));  
            help.show();
        } catch(Exception e) {
            e.printStackTrace();
          }
    }
    
}
