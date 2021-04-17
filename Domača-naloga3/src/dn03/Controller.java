package dn03;

import javafx.event.ActionEvent;
import javafx.fxml.Initializable;
import javafx.scene.control.*;
import javafx.stage.FileChooser;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.net.URL;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ResourceBundle;

public class Controller implements Initializable {
    public MenuItem aboutACB;
    public TextField znamka;
    public TextField moc;
    public TextField stVrat;
    public ComboBox tipCombo;
    public TextField model;
    public TextField prostornina;
    public TextField stSedezev;
    public TextField masa;
    public ComboBox gorivoCombo;
    public TextField barva;
    public TextField ime;
    public TextField ulica;
    public DatePicker rojstvoDate;
    public TextField postnaSt;
    public TextField priimek;
    public RadioButton mladVoznik;
    public RadioButton izkusenVoznik;
    public TextField kraj;
    public TextField hisnaSt;
    public RadioButton ao;
    public RadioButton aoPlus;
    public RadioButton odbitnaFransiza;
    public RadioButton brezKasko;
    public RadioButton polniKasko;
    public CheckBox parkirisceZav;
    public CheckBox steklaZav;
    public CheckBox gumeZav;
    public CheckBox krajaZav;
    public CheckBox potnikiZav;
    public CheckBox tocaZav;
    public CheckBox tretjaosZav;
    public DatePicker registracijaDatum;
    public Label status;
    public TextField registrska;
    public TextField krajReg;

    public void shraniCB(ActionEvent actionEvent) throws Exception {
        try {
            if (!znamka.getText().isEmpty() && !moc.getText().isEmpty() && !stVrat.getText().isEmpty() && !model.getText().isEmpty() && !prostornina.getText().isEmpty() &&
                !stSedezev.getText().isEmpty() &&!masa.getText().isEmpty() &&!barva.getText().isEmpty() &&!ime.getText().isEmpty() &&!ulica.getText().isEmpty() &&
                !postnaSt.getText().isEmpty() &&!priimek.getText().isEmpty() &&!kraj.getText().isEmpty() &&!hisnaSt.getText().isEmpty() &&!tipCombo.getValue().toString().isEmpty() &&
                !gorivoCombo.getValue().toString().isEmpty() && !rojstvoDate.getValue().toString().isEmpty() && !registracijaDatum.getValue().toString().isEmpty()) {

                FileChooser fc = new FileChooser();
                fc.setTitle("Izberi datoteko za shranjevanje");
                File f = fc.showSaveDialog(null);

                BufferedWriter bw = null;
                String date = DateTimeFormatter.ofPattern("yyyy.MM.dd").format(LocalDateTime.now());

                //File file = new File("D:/documents/Faks/2.LETNIK/2.semester/UV/User-Interfaces/Domača-naloga3/Zavarovalnica/" + ime.getText() + " " + priimek.getText() + " " + date + ".txt");
                if (!f.exists()) {
                    f.createNewFile();
                    status.setText("Podatki so bili shranjeni");
                }
                String tip = tipCombo.getValue().toString();
                String gorivo = gorivoCombo.getValue().toString();
                String izkusnje = "";
                if (mladVoznik.isSelected()) {
                    izkusnje = "Mlad voznik";
                } else {
                    izkusnje = "Izkušen voznik";
                }

                String zavarovanje = "";
                if (ao.isSelected()) {
                    zavarovanje = "AO";
                } else {
                    zavarovanje = "AO+";
                }

                String kasko = "";
                if (brezKasko.isSelected()) {
                    kasko = "Brez";
                } else if (polniKasko.isSelected()) {
                    kasko = "Polni kasko";
                } else {
                    kasko = "Odbitna franšiza";
                }

                String dodatno = "";
                if (parkirisceZav.isSelected()) {
                    dodatno += "Parkirišče ";
                }
                if (steklaZav.isSelected()) {
                    dodatno += "Stekla ";
                }
                if (potnikiZav.isSelected()) {
                    dodatno += "Potniki ";
                }
                if (gumeZav.isSelected()) {
                    dodatno += "Gume ";
                }
                if (tretjaosZav.isSelected()) {
                    dodatno += "Tretje-osebe ";
                }
                if (tocaZav.isSelected()) {
                    dodatno += "Toca ";
                }
                if (krajaZav.isSelected()) {
                    dodatno += "Kraja ";
                }


                FileWriter fw = new FileWriter(f);
                bw = new BufferedWriter(fw);
                bw.write("Podatki o vozilu: \n" + "Znamka: " + znamka.getText() + ", Model: " + model.getText() + ", Moč(kW): " + moc.getText() + ", Prostornina: " + prostornina.getText() + ", Tip: " + tip + ", Masa: " + masa.getText() + ", Gorivo: " + gorivo + ", Barva: " + barva.getText() + ", Št. vrat: " + stVrat.getText() + ", Št. sedežev: " + stSedezev.getText() + "\n\n"
                        + "Podatki o zavarovancu: \n" + "Ime: " + ime.getText() + ", Priimek: " + priimek.getText() + ", Hišna št.: " + hisnaSt.getText() + ", Ulica: " + ulica.getText() + ", Poštna št.: " + postnaSt.getText() + ", Kraj: " + kraj.getText() + ", Datum rojstva: " + rojstvoDate.getValue() + ", Izkušnje: " + izkusnje + "\n\n"
                        + "Podatki o zavarovanju: \n" + "Osnovno zavarovanje: " + zavarovanje + ", Kasko: " + kasko + ", Dodatno zavarovanje: " + dodatno + "\n\n"
                        + "Podatki o registraciji: \n" + "Registrska oznaka: " + registrska.getText() + ", Prva registracija vozila: " + rojstvoDate.getValue() + ", Kraj registracije: " + krajReg.getText());
                bw.close();
            }
            else {
                status.setText("Izpolnite vse potrebne podatke!");
            }
        }
        catch (Exception e) {

        }
    }

    public void exitCB(ActionEvent actionEvent) {
        status.setText("Zapiranje programa!");
        System.exit(0);
    }

    public void aboutCB(ActionEvent actionEvent) {
        status.setText("Avtor: Tim Rus");
    }

    public void ponastaviCB(ActionEvent actionEvent) {
        tipCombo.getSelectionModel().clearSelection();
        gorivoCombo.getSelectionModel().clearSelection();
        znamka.clear();
        moc.clear();
        stSedezev.clear();
        stVrat.clear();
        model.clear();
        prostornina.clear();
        masa.clear();
        barva.clear();
        ime.clear();
        ulica.clear();
        rojstvoDate.getEditor().clear();
        postnaSt.clear();
        priimek.clear();
        mladVoznik.setSelected(false);
        izkusenVoznik.setSelected(false);
        kraj.clear();
        aoPlus.setSelected(false);
        hisnaSt.clear();
        odbitnaFransiza.setSelected(false);
        polniKasko.setSelected(false);
        parkirisceZav.setSelected(false);
        steklaZav.setSelected(false);
        gumeZav.setSelected(false);
        krajaZav.setSelected(false);
        potnikiZav.setSelected(false);
        tocaZav.setSelected(false);
        tretjaosZav.setSelected(false);
        registracijaDatum.getEditor().clear();
        registrska.clear();
        krajReg.clear();
    }


    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        status.setText("Ko izpolnite vse potrebne podatke nadaljujte na naslednji zavihek.");
        tipCombo.getItems().addAll("Limuzina", "Kombilimuzina", "Karavan", "Kabriolet", "Enoprostorec", "Kupe", "SUV");
        gorivoCombo.getItems().addAll("Dizel", "Bencin", "Plin");
    }
}


