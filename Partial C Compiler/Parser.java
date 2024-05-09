package finalAssignment;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Stack;
 
/**
LL1
 
E -> E + T | E - T | T 
T -> T * F | T / F | T % F | F 
F -> ( E ) | I 
I -> id | id ++ | id -- | ++ id | -- id


==============================================================================================================================================================
Nonterminal	+				-				*				/				%				(				)			id			++			--	
E			null			null			null			null			null			E -> T E'		null		E -> T E'	E -> T E'	E -> T E'	
E'			E' -> + T E'	E' -> - T E'	null			null			null			null			E' -> ''	null		null		null		
T			null			null			null			null			null			T -> F T'		null		T -> F T'	T -> F T'	T -> F T'			
T'			T' -> ''		T' -> ''		T' -> * F T'	T' -> / F T'	T' -> % F T'	null			T' -> ''	null		null		null
F			null			null			null			null			null			F -> ( E )		null		F -> I 		F -> I		F -> I
I			null			null			null			null			null			null			null		I -> id I	I -> ++id	I -> --id
I'			I' -> ''		I' -> ''		I' -> ''		I' -> ''		I' -> ''		null			I' -> ''	null		I' -> ++	I' -> --

==============================================================================================================================================================
*/


public class Parser
{
	//input
	public String input=""; 
	
	//will go through the input 
    private int indexOfInput=-1;
    
    //Stack
    Stack <String> strack=new Stack<String>();
    
    //Table of rules
    String [][] table=
    {
    	{null,null,null,null,null,"TP",null,"TP","TP","TP",null},
        {"+TP","-TP",null,null,null,null,"",null,null,null,""},
        {null,null,null,null,null,"FB",null,"FB","FB","FB",null}   ,
        {"","","*FB","/FB","%FB",null,"",null,null,null,""},
        {null,null,null,null,null,"(E)",null,"I","I","I",null},
        {null,null,null,null,null,null,null,"iK","vi","wi",null},
        {"","","","","",null,"",null,"v","w",""}
    };
    
    //set of terminals and non terminals to get rule i->id, v->++, w->--
    String [] nonTers={"E","P","T","B","F","I","K"};
    String [] terminals={"+","-","*","/","%","(",")","i","v","w","$"};
 
//====================================================
public Parser(String in)//constructor
{
this.input=in;
}
//====================================================
//Error handling: method to correct if the first character is an operator
private String correctFirstCharacter(String expression) {
 if (expression.length() > 0 && isOperator(expression.charAt(0)) && indexOfInput==-1) {
	 System.out.print("Error occured --> Expression starts with an operator. \nParser recoverd by adding an id at the begginig of the expression: ");
	 return "i" + expression; //recover
 }
 return expression;
}

//check if starts w operator
private boolean isOperator(char c) {
 return c == '+' || c == '-' || c == '*' || c == '/';
}
//====================================================
private  void pushRule(String rule)
{
for(int i=rule.length()-1;i>=0;i--)//push into stack in reverse order
{
  char ch=rule.charAt(i);
  String str=String.valueOf(ch);
 
  push(str);
}
}
//====================================================
public String getRule(String non,String term, int lineNumber, int flag,ArrayList<String> errorsLog)
{
	String rule; 
    int row=getnonTermIndex(non);
    if (row==9000) {//couldn't find non-terminal using getnonTermIndex
    	if (flag==1) { //as compiler
    		errorsLog.add("invalid non-terminal ("+ non+") used in line " + lineNumber + ". ");
        	rule="norule"; //so we dont return null and java stops executing 
        	}
        	else if (flag==0) { //as interpreter
        	error("invalid non-terminal ("+non+") in line "+ lineNumber + ".");;
           }
    	return "norule";
    }
    int column=getTermIndex(term);
    if (column==9000) {//couldn't find terminal using getTermIndex
    	if (flag==1) { //as compiler
    		errorsLog.add("invalid terminal ("+ term+") used in line " + lineNumber + ". ");
        	rule="norule";
        	}
        	else if (flag==0) {//as interpreter
        	error("invalid terminal ("+term+") in line "+ lineNumber + ".");}
    	return "norule";
    }
    //System.out.println(row + " "+column);
    rule=this.table[row][column];
    //System.out.println(rule);
    if(rule==null)
    {
    	if (flag==1) { //as compiler
    	errorsLog.add("There is no Rule by this: Non-Terminal (" +non+ "),Terminal (" +term + ") at line " + lineNumber + ". ");
    	rule="norule"; 
    	}
    	else if (flag==0) { //as interpreter
       error("There is no Rule by this , Non-Terminal("+non+") ,Terminal("+term+") at line "+ lineNumber + ". ");}
    }
    return rule;
}

//====================================================
private boolean exception(String input) {
	//error handling checks if the input starts with + / - / * / / since arithmetic grammar never starts with them unless ++id and --id
    return "--i$".equals(input) || "++i$".equals(input);
}
//====================================================

//algorithm
public void algorithm (int lineNumber, int flag,  ArrayList<String> errorsLog)
{
	int accept=1; //helper flag to allow running as compiler
	
	//check exception before error handling
	if (exception(this.input)) {
	        System.out.println("Input is accepted by LL1\n");
	        return;
	    }
	
	//error handling
	this.input = correctFirstCharacter(this.input);
	
    push(this.input.charAt(0)+"");//starting non-terminal
    push("E");
    //Read one token from input
    String token=read();
    //System.out.println("token="+token);
    String top=null;
    do
    {
     top=this.pop();
     //System.out.println("top="+top);
     //if top is non-terminal
     if(isNonTerminal(top))
     {
       String rule=this.getRule(top, token, lineNumber,flag,errorsLog);
       this.pushRule(rule);
       if(rule=="norule") { //to help with running as compiler 
    	   System.out.println("Input is not Accepted by LL1\n"); 
    	   return; //if we dont it does issue
       }
     }
     else if(isTerminal(top))
     {
       if(!top.equals(token))
       {   
    	   if (flag==1) { //as compiler
    	    	errorsLog.add("Error: Incorrect token ("+ token+") at line " + lineNumber + ". ");
    	    	}
    	    	else if (flag==0) { //as interpreter
    	       error("Error: Incorrect token ("+ token+") at line " + lineNumber + ". ");
    	       }
    	   
       }
       else
       {
         //System.out.println("Matching: Terminal :( "+token+" )");
         token =read();
         //top=pop();
       }
      }
      else
      {
    	  //error("Never Happens , Because top : ( "+top+" )");        
      }
      if(token.equals("$"))
      {
        break;
      }
        //if top is terminal
    }while(true);//out of the loop when $
    
    //accept
    if(token.equals("$")&&accept==1)
    {
     System.out.println("Input is Accepted by LL1\n");   
    }
    else
    {
    	System.out.println("Input is not Accepted by LL1\n");   
    }
}
//====================================================
private boolean isTerminal(String s)
{
  for(int i=0;i<this.terminals.length;i++)
  {
   if(s.equals(this.terminals[i]))
   {return true; }
  }
  return false;
}
//====================================================
private boolean isNonTerminal(String s)
{
  for(int i=0;i<this.nonTers.length;i++)
  {
     if(s.equals(this.nonTers[i]))
     {return true;}
  }
  return false;
}
//====================================================
private String read() {
    indexOfInput++;

    char ch = this.input.charAt(indexOfInput);
    String str = String.valueOf(ch);

    //handling read of ++ and -- 
    // Check for i++
    if (ch == '+' && indexOfInput + 1 < input.length()) {
        if (input.charAt(indexOfInput + 1) == '+') {
            str = "v";//set str to the correct terminal defined
            //skip the next +
            indexOfInput++; 
        }
    }
    // Check for ++i
    else if (ch == '+' && indexOfInput - 1 >= 0) {
        if (input.charAt(indexOfInput - 1) == '+') {
            str = "v"; //set str to the correct terminal defined
            // Do not increment (looking backwards so index correct)
        }
    }
    //check for i--
    else if (ch == '-' && indexOfInput + 1 < input.length()) {
        if (input.charAt(indexOfInput + 1) == '-') {
            str = "w"; //set str to the correct terminal defined
          //skip the next -
            indexOfInput++; 
        }
    //check for i++
    } else if (ch == '-' && indexOfInput - 1 >= 0) {
        if (input.charAt(indexOfInput - 1) == '-') {
            str = "w";//set str to the correct terminal defined
         // Do not increment (looking backwards so index correct)
        }
    }

    return str;
}
//====================================================
private void push(String s)
{
     this.strack.push(s);   
}
//====================================================
private String pop()
{
   return this.strack.pop();   
}
//====================================================
private void error(String message)
{
        System.out.println(message);
        throw new RuntimeException(message);
}
//====================================================

//==================================================================
private int getnonTermIndex(String non)
{		
       for(int i=0;i<this.nonTers.length;i++)
       {
        if(non.equals(this.nonTers[i]))
        {
         return i;
        }
        }
      // error(non +" is not NonTerminal");
       return 9000;//so no index out of bound, less than length
}
//====================================================
private int getTermIndex(String term)
{
  for(int i=0;i<this.terminals.length;i++)
  {
    if(term.equals(this.terminals[i]))
    {return i;}
  }
     // error(term +" is not Terminal");
       return 9000;
}
//====================================================
private static boolean checkSemantics(String semantic, int mode,HashMap<String, String> symbolTable)
{
	
	//locate =
    int equalsIndex = semantic.indexOf('=');
    
    //some syntax check (= not at end and found)
    if (equalsIndex == -1 || equalsIndex == semantic.length() - 1) {
        return false;
    }
    
    //get substring after =
    String numberStr = semantic.substring(equalsIndex + 1).trim();
    
    switch (mode) {//check if the data type from symbol table and first token match
    case 1:
    	 if (symbolTable.containsKey(numberStr) && "Integer".equals(symbolTable.get(numberStr))) {
 	        return true;  
 	    }
    case 2:
    	if (symbolTable.containsKey(numberStr) && "Real".equals(symbolTable.get(numberStr))) {
 	        return true;  
 	    }
    case 3:
    	String string = numberStr.substring(1, numberStr.length() - 1);
    	if (symbolTable.containsKey(string) && "String".equals(symbolTable.get(string))) {
 	        return true;  
 	    }
    default:
        return false; 
    }
}
//main
public static void main(String[] args,int flag)throws IOException {
   // TODO code application logic here
	String inputStream = "input2.c"; 
    String outputStream = "output.c"; 
    String cleanedCode;
    int lineNumber=0; //for error reporting
    ArrayList<String> errorsLog = new ArrayList<>(); //for compiler errors storage
    
    cleanedCode = LexicalAnalyzer.RemoveCommentsAndWhiteSpace(inputStream);
    LexicalAnalyzer.writeToFile(outputStream, cleanedCode);
 
    // StringBuilder to test each statement in input file
    StringBuilder sb = new StringBuilder();
    StringBuilder oldsb = new StringBuilder();
    
    HashMap<String, String> symbolTable=LexicalAnalyzer.lex(outputStream);
    
    //tokenize
    ArrayList<String> tokens = LexicalAnalyzer.tokenizeFile(outputStream);
    
    
    boolean isFirstToken = true;
    int semantics = 0;
    int mode=0;
    // Iterate over the tokens
    for (String token : tokens) {
    	
	    	if (isFirstToken) { //check if i want to do semantics check or syntax check
	    		if (token.equals("int") || token.equals("double")||token.equals("float") || token.equals("String")) {
	    			if (token.equals("int")) {
	    		        mode = 1;
	    		    } else if (token.equals("double")||token.equals("float")) {
	    		        mode = 2;
	    		    } else if (token.equals("String")) {
	    		        mode = 3;
	    		    }
	    			semantics = 1; 
	    		}
	            isFirstToken = false; //set flag to false after processing  first token
	        }
	    	
            if (token.equals(";")) {
            	oldsb.append(": ");
            	isFirstToken = true; //reset it for next statement 
            }
            else{
            	if(semantics==1) {
            		oldsb.append(token);
            		oldsb.append(" "); //more readable 
            	}
            	else {//don't need spaces in syntax checker
            	oldsb.append(token); }
            }
            if (token.equals(";")) {
               System.out.print(oldsb.toString()); //statement print
               oldsb = new StringBuilder(); //save statment
               lineNumber++; // increment line number
            } 
            
            
    	 if (symbolTable.containsKey(token) && "id".equals(symbolTable.get(token))) {
    	        token = "i";  //replace token with i so it works with my grammar
    	    }
    	 
    	// Append the token to StringBuilder
        if (token.equals(";")) {if (semantics==0)sb.append("$"); /*so it works with parser driver*/}
        else{sb.append(token);}

        // Check if the token is a semicolon
        if (token.equals(";")) {
        	if (semantics==1) { //check semantics if starts with int, double, etc.
        		if (checkSemantics(sb.toString(),mode,symbolTable)) {
        			System.out.println("Correct semantic");
        		}
        		else {
        			System.out.println("Incorrect semantic");
        		}
        		semantics=0;//reset flag
        	}
        else if(semantics==0) {
            Parser parser=new Parser(sb.toString()); //pass string to parser driver
        	parser.algorithm(lineNumber,flag,errorsLog);
        	}
            //reset the StringBuilder for the next statement
            sb = new StringBuilder();
        } 
    }
    if (flag==1) {//print error log array in case run as compiler
    	System.out.println("Errors occured: ");
    	for (String str : errorsLog) {
            System.out.println(str);
        }
    }
}

}
