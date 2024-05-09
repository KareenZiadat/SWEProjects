package finalAssignment;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) throws IOException {
        Scanner scanner = new Scanner(System.in);
        System.out.println("Compiler Demo");
        System.out.println("1. Run lexical analyzer alone");
        System.out.println("2. Run parser in interpreter mode");
        System.out.println("3. Run parser in compiler mode");
        System.out.println("4. Print the symbol table"); 
        System.out.println("Enter your choice:");

        int choice = scanner.nextInt();
        String inputStream = "input.c"; 
        String outputStream = "output.c"; 
        String cleanedCode;
        switch (choice) {
            case 1:
                LexicalAnalyzer.lex(outputStream);         
                break;
                
            case 2:
            	//as interpreter: stop as soon as i find error
            	Parser.main(args,0);
                break;
                
            case 3:
            	//as compiler: log error
            	Parser.main(args,1);
              break;
              
            case 4:
            	//clean code
            	cleanedCode = LexicalAnalyzer.RemoveCommentsAndWhiteSpace(inputStream);
                LexicalAnalyzer.writeToFile(outputStream, cleanedCode); //function to write sting onto output file
               
                //generate symbol table
                HashMap<String, String> symbolTable=LexicalAnalyzer.lex(outputStream);
                
                //print symbol table
                LexicalAnalyzer.printSymbolTable(symbolTable);
                break;
                
            default:
                System.out.println("Invalid choice.");
                break;
        }

        scanner.close();
    }
}
