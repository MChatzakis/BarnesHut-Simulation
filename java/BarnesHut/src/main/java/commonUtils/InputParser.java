/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package commonUtils;

import barnesHut.Entity;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author manos
 */
public class InputParser {

    public static ArrayList<Entity> parseFile(String path) {
        ArrayList<Entity> entities = new ArrayList<>();

        try {
            File file = new File("Demo.txt");    //creates a new file instance  

            FileReader fr = new FileReader(file);   //reads the file  

            BufferedReader br = new BufferedReader(fr);  //creates a buffering character input stream  

            StringBuffer sb = new StringBuffer();    //constructs a string buffer with no characters  
            String line;
            while ((line = br.readLine()) != null) {
                sb.append(line);      //appends line to string buffer  
                sb.append("\n");     //line feed   
            }
            fr.close();    //closes the stream and release the resources  

        } catch (Exception ex) {
            Logger.getLogger(InputParser.class.getName()).log(Level.SEVERE, null, ex);
        }
        return entities;
    }
}
