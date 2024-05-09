package finalAssignment;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class LexicalAnalyzer {
	private static final String TOKEN_PATTERN = "\\b[_a-zA-Z][_a-zA-Z0-9]*\\b|\\d+\\.?\\d*|\"[^\"]*\"|\\S";
	 
    public static void main(String[] args) throws IOException {
        // names of input and output files  
    	String inputStream = "input.c"; 
        String outputStream = "output.c"; 

        //1) remove white space, comments
        String cleanedCode = RemoveCommentsAndWhiteSpace(inputStream);
        writeToFile(outputStream, cleanedCode); //function to write sting onto output file
        System.out.println("Lexical analyzer cleaning done.");
   
        
        //2) calling function to print the symbol table at run time. 
        HashMap<String, String> symbolTable=lex(outputStream);
        printSymbolTable(symbolTable);

       //3) calling function to tokenize file for parser. 
        ArrayList<String> tokens = tokenizeFile(outputStream);
       /* for (String token : tokens) {
            System.out.println(token);
        }*/
     }
    
    //symbol table 
	public static HashMap<String, String> lex(String outputStream) throws IOException {
        char ch;
        int lineNumber = 1;
        StringBuilder id = new StringBuilder("");
              
        HashMap<String, String> reservedTable = new HashMap<String, String>();
        fillReservedTable(reservedTable);

        HashMap<String, String> symbolTable = new HashMap<String, String>();

        try (BufferedReader reader = new BufferedReader(new FileReader(outputStream))) {
            int readChar;
            while ((readChar = reader.read()) != -1) {
                ch = (char) readChar;
                if (ch == '\n') {
                    lineNumber++; // increment line number
                 }
                
              
               if (Character.isDigit(ch)) { //check for numbers ( should start with number)
            	   boolean doubleNumber = false; //flag for double or int number type
                    id.append(ch);
                    ch = (char) reader.read();
                    
                    while (Character.isDigit(ch)) {
                        id.append(ch);
                        ch = (char) reader.read();
                    }

                    if (ch == '.') {//it is a double
                    	doubleNumber=true; //set flag to true
                        id.append(ch);
                        ch = (char) reader.read();

                        while (Character.isDigit(ch)) {
                            id.append(ch);
                            ch = (char) reader.read();
                        }
                    }

                    if (ch == 'E' || ch == 'e') {
                        id.append(ch);
                        ch = (char) reader.read();

                        if (ch == '+' || ch == '-') {
                            id.append(ch);
                            ch = (char) reader.read();
                        }

                        while (Character.isDigit(ch)) {
                            id.append(ch);
                            ch = (char) reader.read();
                        }
                    } 
                    
                    else if (Character.isLetter(ch)) {// in c grammar it is not allowed to have number then letter
                        System.out.print("Error: Not a valid identifier at line " + lineNumber + ": "); //print error +number
                        printLine(lineNumber); //call function to print line
                        id.delete(0, id.length());
                        break; //compiler should stop analyzing so break
                    }
                    
                    String value; //data type of number 
                    if (doubleNumber==true) { //if flag is true then number is double
                    	value = "Real";
                    	//maybe float/double
                    }
                    else {
                    	value = "Integer";
                    }
                    symbolTable.put(id.toString(), value); //add the number and its type to the symbol hash table
                    
                    id.delete(0, id.length()); //clear id StringBuilder
                } 
                
               else if (Character.isLetter(ch)||ch=='_') { //check for identifiers (starts with letter)
            	    id.append(ch);
            	    ch = (char) reader.read();

            	    while (Character.isDigit(ch) || Character.isLetter(ch)||ch=='_') {//can be followed by letter or letter 
            	        id.append(ch);
            	        ch = (char) reader.read();
            	    }

            	    // identifier max 32 characters 
            	    if (id.length() > 32) {
            	        System.out.println("Identifier length exceeds 32 characters at line " + lineNumber);
            	        id = new StringBuilder(id.substring(0, 32));
            	    }

            	    if (!reservedTable.containsKey(id.toString())) { //check if this id is a reserved word (compare id to keys of ReservedWord hash table)
            	        String value = "id";
            	        symbolTable.put(id.toString(), value);
            	    }
            	    
            	    id.delete(0, id.length());
            	}
               
                else if (ch == '\"') { //check for strings starting with double quote
                    ch = (char) reader.read(); //read next character after the opening quote
                    
                    while (ch != '\"' && ch != (char)-1) { //read until another double quote or end of file
                        if (ch == '\\') { //if it's a backslash, check next character for escape 
                            ch = (char) reader.read();
                            if (ch == 'n' || ch == 't' || ch == 'a') {
                            	//When followed by an n or a t, the backslash denotes a new line (CR) or tab
                                //skip adding to the string
                            } 
                            else {
                                //if not n,t,a its invalid so print error, dont save to symbol table and exit
                                System.out.println("Error: Invalid escape character used at line " + lineNumber);
                                id.delete(0, id.length());
                                break;
                            }
                        } 
                        else {id.append(ch);}
                        ch = (char) reader.read();
                    }

                    if (ch == '\"' && id.length() > 0) { //if reached end of sting (") and id is not empty (because of invalid escape), add to symbol table
                    	String string=id.toString().replaceAll("\\s+", " ");
                    	symbolTable.put(string, "String");
                        id.delete(0, id.length()); //clear the StringBuilder
                    } 
                    else if (id.length() == 0) {
                        //if invalid escape character which is already handled don't do anything
                    } 
                    else {
                        //handle error for unclosed string literal
                        System.out.println("Error: Unclosed string literal at line " + lineNumber);
                    }
                }
            }
        }

        // Save keys and values in a text file (SymbolTable)
        try (BufferedWriter writer = new BufferedWriter(new FileWriter("SymbolTable.txt"))) {
            // Write the symbol table to the file
            for (String j : symbolTable.keySet()) {
                writer.write(j + " " + symbolTable.get(j) + "\n");
            }
        }

        return symbolTable;
    }

	//tokens sent to parser 
	public static ArrayList<String> tokenizeFile(String filename) throws IOException {
	    ArrayList<String> tokens = new ArrayList<>();
	    BufferedReader reader = new BufferedReader(new FileReader(filename));

	    String line;
	    while ((line = reader.readLine()) != null) {
	        Matcher matcher = Pattern.compile(TOKEN_PATTERN).matcher(line);
	        while (matcher.find()) {
	            tokens.add(matcher.group());
	        }
	    }

	    reader.close();
	    return tokens;
	}
    
    // function to print the symbol table at run time.
    public static void printSymbolTable(HashMap<String, String> symbolTable) {
        System.out.println("Symbol Table Contents:");
        for (String key : symbolTable.keySet()) {
            System.out.println("Symbol: " + key + ", Type: " + symbolTable.get(key));
        }
    }
   
    //function to remove comments and white space
    public static String RemoveCommentsAndWhiteSpace(String inputStream) throws IOException {
        Path path = Paths.get(inputStream);
        String content = Files.readString(path);

        // remove multi-line comments
        content = removeMultilineComments(content);
        //call function that removes multi line comments using regex

        // Remove single line comments
        content = content.replaceAll("//.*", ""); 
        //Regular expression of (//) followed by any sequence of characters (.*) other than new line replaced with nth so removing. 
        

        // Remove extra white spaces
        content = content.replaceAll("\\s+", " ");
        //Regular expression for one or more whitespace characters replaced with one space so removing only the more.

        return content;
    }

    public static String removeMultilineComments(String content) {
    	//pattern starts with (/*) then followed by an optional sequence of characters (.*?) then ends with (*/).
        Pattern pattern = Pattern.compile("/\\*.*?\\*/", Pattern.DOTALL); 
        //Pattern.compile will compile the regular expression pattern into a Pattern object, the DOTALL is a flag used to let the dot in the regular expression to match newline characters as well. 
        Matcher matcher = pattern.matcher(content); //Matcher will find 
        return matcher.replaceAll(""); //will remove the multiline comments.
    }

    public static void writeToFile(String outputStream, String content) throws IOException {
        try {
            //add new line after each semi-colon to preserve structure
            content = content.replaceAll(";", ";\n");

            //write array of Bytes (content.getBytes() changes string to Bytes array) onto output file path
           Files.write(Paths.get(outputStream), content.getBytes());
        } catch (IOException e) {
            throw new IOException("Error writing to file: " + e.getMessage());
        }
    }

    public static void printLine(int lineNumber) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader("output.c"))) {
            String line;
            int currentLineNumber = 1;

            while ((line = reader.readLine()) != null) {
                if (currentLineNumber == lineNumber) {
                    System.out.println(line);
                    return;
                }

                currentLineNumber++;
            }
        }
    }
    
    //function to fill reserved table
    
    public static void fillReservedTable(HashMap<String, String> reservedTable) {
        // Keywords
    	reservedTable.put("printf", "Keyword");
        reservedTable.put("abstract", "Keyword");
        reservedTable.put("assert", "Keyword");
        reservedTable.put("boolean", "Keyword");
        reservedTable.put("break", "Keyword");
        reservedTable.put("byte", "Keyword");
        reservedTable.put("case", "Keyword");
        reservedTable.put("catch", "Keyword");
        reservedTable.put("char", "Keyword");
        reservedTable.put("class", "Keyword");
        reservedTable.put("const", "Keyword");
        reservedTable.put("continue", "Keyword");
        reservedTable.put("default", "Keyword");
        reservedTable.put("do", "Keyword");
        reservedTable.put("double", "Keyword");
        reservedTable.put("else", "Keyword");
        reservedTable.put("enum", "Keyword");
        reservedTable.put("extends", "Keyword");
        reservedTable.put("final", "Keyword");
        reservedTable.put("finally", "Keyword");
        reservedTable.put("float", "Keyword");
        reservedTable.put("for", "Keyword");
        reservedTable.put("if", "Keyword");
        reservedTable.put("implements", "Keyword");
        reservedTable.put("import", "Keyword");
        reservedTable.put("instanceof", "Keyword");
        reservedTable.put("int", "Keyword");
        reservedTable.put("interface", "Keyword");
        reservedTable.put("long", "Keyword");
        reservedTable.put("native", "Keyword");
        reservedTable.put("new", "Keyword");
        reservedTable.put("package", "Keyword");
        reservedTable.put("private", "Keyword");
        reservedTable.put("protected", "Keyword");
        reservedTable.put("public", "Keyword");
        reservedTable.put("return", "Keyword");
        reservedTable.put("short", "Keyword");
        reservedTable.put("static", "Keyword");
        reservedTable.put("strictfp", "Keyword");
        reservedTable.put("super", "Keyword");
        reservedTable.put("switch", "Keyword");
        reservedTable.put("synchronized", "Keyword");
        reservedTable.put("this", "Keyword");
        reservedTable.put("throw", "Keyword");
        reservedTable.put("throws", "Keyword");
        reservedTable.put("transient", "Keyword");
        reservedTable.put("try", "Keyword");
        reservedTable.put("void", "Keyword");
        reservedTable.put("volatile", "Keyword");
        reservedTable.put("while", "Keyword");

        // Literals
        reservedTable.put("true", "Literal");
        reservedTable.put("false", "Literal");
        reservedTable.put("null", "Literal");

        // Operators
        reservedTable.put("+", "Operator");
        reservedTable.put("-", "Operator");
        reservedTable.put("*", "Operator");
        reservedTable.put("/", "Operator");
        reservedTable.put("%", "Operator");
        reservedTable.put("++", "Operator");
        reservedTable.put("--", "Operator");
        reservedTable.put("==", "Operator");
        reservedTable.put("!=", "Operator");
        reservedTable.put(">", "Operator");
        reservedTable.put("<", "Operator");
        reservedTable.put(">=", "Operator");
        reservedTable.put("<=", "Operator");
        reservedTable.put("&&", "Operator");
        reservedTable.put("||", "Operator");
        reservedTable.put("!", "Operator");
        reservedTable.put("&", "Operator");
        reservedTable.put("|", "Operator");
        reservedTable.put("^", "Operator");
        reservedTable.put("<<", "Operator");
        reservedTable.put(">>", "Operator");
        reservedTable.put(">>>", "Operator");
        reservedTable.put("~", "Operator");

        // Data Types
        reservedTable.put("byte", "Data Type");
        reservedTable.put("short", "Data Type");
        reservedTable.put("int", "Data Type");
        reservedTable.put("long", "Data Type");
        reservedTable.put("float", "Data Type");
        reservedTable.put("double", "Data Type");
        reservedTable.put("char", "Data Type");
        reservedTable.put("boolean", "Data Type");
    }
}