package dn04;

import javafx.event.ActionEvent;
import javafx.fxml.Initializable;
import javafx.scene.control.*;
import javafx.stage.FileChooser;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.net.URL;
import java.util.ResourceBundle;

public class Controller implements Initializable {
    public DatePicker zacetekIzposoje;
    public DatePicker konecIzposoje;
    public ComboBox krajPrevzema;
    public ComboBox krajVrnitve;
    public ComboBox tipVozila;
    public ComboBox tipMenjalnika;
    public ComboBox gorivo;
    public TextField uraRez;
    public TextField uraVrnit;
    public TextField ime;
    public TextField naslovStranke;
    public TextField emailStranke;
    public TextField telStevilka;
    public TextField starostStranke;
    public TextField priimek;
    public TextField postnaStevilka;
    public TextField izkusnje;
    public TextField zaPlacilo;
    public TextField stKartice;
    public TextField ccvKoda;
    public RadioButton kartica;
    public RadioButton gotovina;
    public Label status;
    public CheckBox zavarovanje;
    public TextField model;

    public void ponastaviCB(ActionEvent actionEvent) {
        zacetekIzposoje.getEditor().clear();
        konecIzposoje.getEditor().clear();
        krajVrnitve.getSelectionModel().clearSelection();
        krajPrevzema.getSelectionModel().clearSelection();
        tipMenjalnika.getSelectionModel().clearSelection();
        tipVozila.getSelectionModel().clearSelection();
        gorivo.getSelectionModel().clearSelection();
        uraRez.clear();
        uraVrnit.clear();
        ime.clear();
        naslovStranke.clear();
        emailStranke.clear();
        telStevilka.clear();
        starostStranke.clear();
        priimek.clear();
        postnaStevilka.clear();
        izkusnje.clear();
        zaPlacilo.clear();
        stKartice.clear();
        ccvKoda.clear();
        kartica.setSelected(false);
        gotovina.setSelected(false);
        zavarovanje.setSelected(false);
        status.setText("Ko izpolnite vse potrebne podatke nadaljujte na naslednji zavihek.");
    }

    public void shraniCB(ActionEvent actionEvent) {
        try {
            if (!uraVrnit.getText().isEmpty() && !uraRez.getText().isEmpty() && !ime.getText().isEmpty() && !priimek.getText().isEmpty() && !naslovStranke.getText().isEmpty() &&
                !emailStranke.getText().isEmpty() &&!telStevilka.getText().isEmpty() &&!starostStranke.getText().isEmpty() &&!priimek.getText().isEmpty() &&!postnaStevilka.getText().isEmpty() &&
                !izkusnje.getText().isEmpty() &&!priimek.getText().isEmpty() &&!zaPlacilo.getText().isEmpty() &&!stKartice.getText().isEmpty() &&!ccvKoda.getText().isEmpty() &&
                 !zacetekIzposoje.getValue().toString().isEmpty() && !konecIzposoje.getValue().toString().isEmpty() && !krajVrnitve.getValue().toString().isEmpty() &&
                !krajPrevzema.getValue().toString().isEmpty() && !tipMenjalnika.getValue().toString().isEmpty() && !tipVozila.getValue().toString().isEmpty() && !gorivo.getValue().toString().isEmpty()) {

                FileChooser fc = new FileChooser();
                fc.setTitle("Izberi datoteko za shranjevanje");
                File f = fc.showSaveDialog(null);


                if (!f.exists()) {
                    f.createNewFile();
                    status.setText("Podatki so bili shranjeni v računalnik.");
                }

                String placilo = "";
                if (kartica.isSelected()) {
                    placilo = "Kreditna kartica";
                } else {
                    placilo = "Gotovina ob prevzemu";
                }

                String prevzem = krajPrevzema.getValue().toString();
                String vrnitev = krajVrnitve.getValue().toString();
                String gor = gorivo.getValue().toString();
                String tip = tipVozila.getValue().toString();
                String menjalnik = tipMenjalnika.getValue().toString();

                //zakritje številk kreditne kartice, razen zadnjih 4-ih

                FileWriter fw = new FileWriter(f);
                BufferedWriter bw = new BufferedWriter(fw);
                bw.write("Podatki o izposoji avtomobila: \n" + "Datum izposoje: " + zacetekIzposoje.getValue() + ", Datum vrnitve: " + konecIzposoje.getValue() + ", Kraj prevzema: " + prevzem + ", Kraj vrnitve: " + vrnitev + ", Ura rezervacije: " + uraRez.getText() + ", Ura vrnitve: " + uraVrnit.getText() + ", Tip avtomobila: " + tip + ", Tip menjalnika: " + menjalnik + ", Gorivo: " + gor + ", Model avtomobila: " + model.getText() + ", Dodatno zavarovanje: " + zavarovanje + "\n\n"
                        + "Podatki o stranki: \n" + "Ime: " + ime.getText() + ", Priimek: " + priimek.getText() + ", Naslov prebivališča: " + naslovStranke.getText() + ", Poštna št. in kraj: " + postnaStevilka.getText() + ", Email: " + emailStranke.getText() + ", Telefonska št.: " + telStevilka.getText() + ", Starost: " + starostStranke.getText() + ", Leta izkušenj v vožnji: " + izkusnje + "\n\n"
                        + "Podatki o plačilu: \n" + "Za plačilo: " + zaPlacilo.getText() + ", Način plačila: " + placilo + ", Številka kreditne kartice: " + "XXXX-XXXX-XXXX-XXXX" + ", CCV koda: " + "XXX");
                bw.close();

            }
            else  {
            status.setText("Izpolni vse podatke!!");
            }
        } catch (Exception e){
            status.setText("Prišlo je do napake v programu.");
        }
    }

    public void zapriCB(ActionEvent actionEvent) {
        status.setText("Zapiranje programa");
        System.exit(0);
    }

    public void aboutCB(ActionEvent actionEvent) {
        status.setText("Avtor: Timotej Tim Rus");
    }

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        zaPlacilo.setText("1100€");
        status.setText("Ko izpolnite vse potrebne podatke nadaljujte na naslednji zavihek.");
        tipVozila.getItems().addAll("Limuzina", "Kombilimuzina", "Karavan", "Kabriolet", "Enoprostorec", "Kupe", "SUV");
        gorivo.getItems().addAll("Dizel", "Bencin", "Plin");
        krajPrevzema.getItems().addAll("Ljubljana", "Koper", "Maribor", "Brežice", "Novo mesto", "Nova Gorica");
        krajVrnitve.getItems().addAll("Ljubljana", "Koper", "Maribor", "Brežice", "Novo mesto", "Nova Gorica");
        tipMenjalnika.getItems().addAll("Avtomatski", "Ročni");
    }
}
