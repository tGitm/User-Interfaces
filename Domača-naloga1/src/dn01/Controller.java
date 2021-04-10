package dn01;

import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.Initializable;
import javafx.scene.control.*;
import javafx.stage.FileChooser;

import java.io.File;
import java.net.URL;
import java.util.ResourceBundle;

public class Controller implements Initializable {
    public ComboBox combo;
    public Spinner spinner;
    public TextField textField;
    public Button button;
    public Label status;
    public ToggleGroup element;
    public RadioButton dodaj;
    public RadioButton odstIzbranega;
    public RadioButton odstPrvega;
    public Label message;
    public CheckBox doubles;

    public void odpriCB(ActionEvent actionEvent) {  //dialog za izbiro datoteke
        FileChooser fc = new FileChooser();
        File f = fc.showOpenDialog(null);

        if (f != null) {
            status.setText("Izbrana datoteka: " + f.getAbsolutePath()); //izpišem izbrano datoteko
        }
    }

    public void pobrisiCB(ActionEvent actionEvent) {
        message.setText("Sporočilo:");
        status.setText("Status:");
    }

    public void izhodCB(ActionEvent actionEvent) {
        System.exit(0);
    }

    public void avtorCB(ActionEvent actionEvent) {
        status.setText("Avtor: Tim Rus");
    }

    public void izvediCB(ActionEvent actionEvent) {
        String vsebina = textField.getText();
        //vse kar se zgodi ob kliku na gumb izvedi akcijo
        RadioButton selectedRadioButton = (RadioButton) element.getSelectedToggle();
        ObservableList<String> items = combo.getItems();
        //pogledam za duplikate v izvlečnem meniju
        for (String item : items) {
            if (dodaj.isSelected()  && !doubles.isSelected() && textField.getText().length() != 0) {
                combo.getItems().add(vsebina);
                message.setText("Dodajam");
            }
            else if (textField.getText().equals(item)) {
                message.setText("Najden je duplikat!");
            }
            if (textField.getText().length() == 0) {
                status.setText("Polje za vnos je prazno!");
            }
        }

        if (odstIzbranega.isSelected()) {
            String selected = combo.getValue().toString();
            combo.getItems().remove(selected);
            message.setText("Odstranjujem izbranega");
        }
        else if (odstPrvega.isSelected()) {
            combo.getItems().remove(combo.getItems().get(0));
            //combo.removeItemAt(0);
            message.setText("Odstranjujem prvega");
        }

    }

    public void iz1TCB(ActionEvent actionEvent) {
        status.setText("T");
    }

    public void iz1ICB(ActionEvent actionEvent) {
        String let = status.getText();
        status.setText(let + "I");
    }

    public void iz1MCB(ActionEvent actionEvent) {
        String let = status.getText();
        status.setText(let + "M");
    }

    public void iz2RCB(ActionEvent actionEvent) {
        String let = status.getText();
        status.setText(let + " R");
    }

    public void iz2UCB(ActionEvent actionEvent) {
        String let = status.getText();
        status.setText(let + "U");
    }

    public void iz2SCB(ActionEvent actionEvent) {
        String let = status.getText();
        status.setText(let + "S");
    }

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        combo.getItems().addAll("Globoko"); //dodajanje elementov v comboBox

        spinner.setValueFactory(new SpinnerValueFactory.IntegerSpinnerValueFactory(0, 20, 0));  //dodajanje števil v spinner polje
        spinner.valueProperty().addListener((obser, oldV, newV) -> {
            int noItems = combo.getItems().size();

            if (noItems < (int)newV) {
                message.setText("Ni elementa!");
            }
            else {
                String elementOnIndex = (String)combo.getItems().get((int)newV);
                message.setText(elementOnIndex);
            }
        });
    }

    public void tCB(ActionEvent actionEvent) {
        status.setText("T");
    }

    public void iCB(ActionEvent actionEvent) {
        String let = status.getText();
        status.setText(let + "I");
    }

    public void mCB(ActionEvent actionEvent) {
        String let = status.getText();
        status.setText(let + "M");
    }

    public void rCB(ActionEvent actionEvent) {
        String let = status.getText();
        status.setText(let + " R");
    }

    public void uCB(ActionEvent actionEvent) {
        String let = status.getText();
        status.setText(let + "U");
    }

    public void sCB(ActionEvent actionEvent) {
        String let = status.getText();
        status.setText(let + "S");
    }
}