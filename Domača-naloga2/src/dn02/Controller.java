package dn02;

import javafx.event.ActionEvent;
import javafx.fxml.Initializable;
import javafx.scene.control.*;
import javafx.scene.web.HTMLEditor;
import javafx.stage.FileChooser;

import java.io.*;
import java.net.URL;
import java.nio.Buffer;
import java.util.ResourceBundle;

public class Controller implements Initializable {

    public Accordion accordion;
    public TitledPane uredi;
    public HTMLEditor editor;
    public TitledPane besedilo;
    public TextArea besediloTextArea;
    public TextArea dnevnikTextArea;
    public Label status;
    public Label messageBar;
    public TextField nizZamenjava;
    public TextField nizIskanje;

    public void odpriCB(ActionEvent actionEvent) {
        FileChooser fc = new FileChooser();
        fc.setTitle("Izberi HTML datoteko");
        File f = fc.showOpenDialog(null);

        if (f != null) {
            StringBuilder sb = new StringBuilder();
            try (BufferedReader br = new BufferedReader((new FileReader(f)))) {
                String line;
                while ((line = br.readLine()) != null) {
                    sb.append(line).append("\n");
                }
                br.close();
                editor.setHtmlText(sb.toString());

                String text = editor.getHtmlText(); //2. polju v accordian meniju dodelim samo navadno bseedilo iz html fila
                besediloTextArea.setText(text);

                dnevnikTextArea.appendText("Prebrana datoteka: " + f.getAbsolutePath() + "\n"); //dnevniku dodam dejavnost uporabnika
                messageBar.setText("Odpiranje datoteke.");
                status.setText("Izbrana datoteka: " + f.getName() + "     Velikost: " + f.getTotalSpace() + "B"); //izpišem izbrano datoteko

            } catch (Exception e) {
            }
        }
    }

    public void shraniCB(ActionEvent actionEvent) {
        FileChooser fc = new FileChooser();
        fc.setTitle("Izberi datoteko za shranjevanje");
        File f = fc.showSaveDialog(null);

        if (f != null) {
            try (BufferedWriter bw = new BufferedWriter((new FileWriter(f)))) {
                bw.write(editor.getHtmlText());
                bw.close();
            }catch (Exception e) {}
        }

        dnevnikTextArea.appendText("Shranil si datoteko: " + f.getAbsolutePath() + "\n"); //dnevniku dodam dejavnost uporabnika
        messageBar.setText("Shranjevanje.");
        status.setText("Shranjeno v: " + f.getName());
    }

    public void izhodCB(ActionEvent actionEvent) {
        dnevnikTextArea.setText("Zapiranje programa."); //dnevniku dodam dejavnost uporabnika
        messageBar.setText("Zapiranje programa.");
        System.exit(0);
    }

    public void najdiCB(ActionEvent actionEvent) {
        String search = nizIskanje.getText();
        String text = besediloTextArea.getText();

        int index = text.indexOf(search);   //index v html datoteki

        besediloTextArea.requestFocus();
        besediloTextArea.positionCaret(index);



        dnevnikTextArea.appendText("Iskanje besede: " + search); //dnevniku dodam dejavnost uporabnika
        messageBar.setText("Iskanje.");
    }

    public void najdiZamenjajCB(ActionEvent actionEvent) {
        String search = nizIskanje.getText();
        String replacement = nizZamenjava.getText();
        String text = editor.getHtmlText();
        String classicText = besediloTextArea.getText();
        int replacements = 0;


        text = text.replaceAll(search, replacement);
        classicText = classicText.replaceAll(search, replacement);
        replacements++;


        //nastavim za html in besedilno okno
        editor.setHtmlText(text);
        besediloTextArea.setText(classicText);

        dnevnikTextArea.appendText("Vse pojavitve besede: " + search + ", so bile zamenjane z: " + replacement + " najdenih je bilo " + replacements);
        messageBar.setText("Iskanje in zamenjava.");
    }

    public void aboutCB(ActionEvent actionEvent) {
        status.setText("Avtor: Tim Rus");
        dnevnikTextArea.appendText("Uporabnik je iskal podatke o avtorju programa."); //dnevniku dodam dejavnost uporabnika
        messageBar.setText("Uporabnik je iskal podatke o avtorju programa.");

    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        accordion.setExpandedPane(uredi);
        uredi.expandedProperty().addListener((obs, wasExpended, isNowExpended) -> {
            if (isNowExpended) {
                editor.setHtmlText(besediloTextArea.getText());
                messageBar.setText("Vsebina HTML urejevalnika je posodobljena.");
                dnevnikTextArea.appendText("Vsebina HTML urejevalnika je posodobljena. \n");
            }
        });

        besedilo.expandedProperty().addListener((obs, wasExpended, isNowExpended) -> {
            if (isNowExpended) {
                besediloTextArea.setText(editor.getHtmlText());
                messageBar.setText("Vsebina besedilnega urejevalnika je posodobljena.");
                dnevnikTextArea.appendText("Vsebina besedilnega urejevalnika je posodobljena. \n");
            }
        });
    }
}