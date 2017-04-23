# **T**estable **P**rogramming **L**anguage#

TPL is a static type, compiling, procedural programming language. It's language with new  grammar understandable to turkmen language speakers.

## HOW TO RUN? ##

Program works through command prompt. Call 'tpl.exe' with path to the source codes as arguments. The executable program will be created where 'tpl.exe' was called from. (Name of executable file is first passed argument to the 'tpl.exe'.

# DOCUMENTATION #

### SCOPES ####

This programming language has 4 scope levels: TPL, global, local and function levels.

* TPL level - is the top level. Every function, variable come with tpl.exe, including third party libraries belongs to TPL level. And they are acceptable in other scopes.
* global level - is program level or in other words between source code level. In global level, data defined in A source code, can be used in B source code. User defined functions and user defined types ALWAYS defined in global scope. 
Global variables and arrays always comes with "@" token on front:

```
@san s.     // define s variable in integer type
@s <- 2+2.  // assign 2+2 to global s variable
@d <- ( drob )s + 2_0.

```

      

  The global data (function, variable, user type or array) defined in A source file, to be accessible in B source code file, must be known to TPL already. If program is not already defined. Then they must be declared. For declarations of global data used .bashy files. 
   .BASHY source files must contain only declarations ( see at the bottom of this documentation ). Then must be called only once time, as because TPL will remember them until program will not created from source codes.

      // first source code: a.tepl
      @san s.        // define s variable in integer type
      @drob d.       // define d variable in float type


      // declaration file: a.bashy
      san s.
      drob d.


      // second source code: b.tepl
      #b1
      // load a.bashy declaration file
      #@"a.bashy"
      
      @s <- 1+1.
      @d <- ( drob )s + 2_0.

Global variables defined only outside of any block. Including function block, if block, loop blocks.

* Local level - local data available only inside of one source code. And not available in another source codes and inside of function blocks. Even if function defined in one source code. 

* Function level - function scope variables and arrays are available inside function bodies. They can be defined as parameters or inside function bodies.


### TOKENS, SPECIAL CHARACTERS and COMMANDS ###

In TPL smallest piece of data called token. 
Commands build by tokens and special characters. 
Special characters are small piece of data to, but unlike tokens, has no self meaning. Special characters is one character length and can not be used as part of tokens. 

* DOT (.) - Except some commands all commands ends with dot (special) character:

```

("'Print' function")chap_et.
san s.
s <- 2+2.

```

* OPEN/CLOSE ( ( )/( ) ) -  In TPL, also open/close paranthesis special character.
* COMMA ( , )            - And comma special character to separate items inside parenthesis.


### DATA TYPES ###

In TPL four basic built-in data types:

* san (integers)   - are 4 byte integers.
* drob (floats)    - are 6 byte float numbers. Floating point in TPL replaced with underscore. For example 2_5 is 2 and half. 3_14 is PI number.
* harp (characters) - 1 character with single quoted around

```

'a'
'=s'   // =s is TPL's escape symbol
's'

```

* harpl (strings) - strings are data types to, not an array of characters as you may think. Strings are 1 or more characters double quoted around


```

"a"
"aba"
"Salam=s"

```

Escape characters are equivalents of escape symbols in mainstream languages. Currently TPL has only 3 escape characters:

* == - means =
* =s - means \n
* =t - means \t


### VARIABLES and ARRAYS ####

Any variable or array before used, must be defined or declared in somewhere.
Variable definition:

    *[type] [identifier]*

*[type]* - is basic or user type.
*[identifier]* - is user defined name for variable. Rules for identifiers same as in C or another mainstream languages. 
When variable defined it automatically takes default values for san, drob and harp type variables:


```

san  s.   // contains 0
drob d.   // contains 0_0
harp h.   // contains '\0'

```

But default values to variable can be inserted manually with assign command:

```

san  s <- 1. 
drob pi <- 3_142526.  
harp h <- 's'.   

```

Array definition:

  (*[element number,..]*)*[type][identifier]*

*[element number,..]* - are list of san data separated by comma. Each san data represents array nesting and value of san data represents number of elements. Left side element is top element of array:


```

( 2 )san arr.        // 2 element integer type array
( 2, 10 )harp arr2.  // 2D array in character type.

```

Arrays automatically take initial value. But they can not be added by user with assign command. 
Index of arrays starts at 0 positions. You can access to inner arrays element only. Array element access formula:


```

( 2 )san ss.
ss 0 <- 0.
ss 1 <- 1.
ss 2 <- 2. // WRONG. OUT OF RANGE


( 3, 2 )harpl ds.
ds 0 0 <- "first element of array".
ds 2 1 <- "last element of array".
ds 0 <- "take elements". // WRONG. CAN NOT ACCESS TO ARRAY. ACCESS TO ELEMENT ONLY.

// GLOBAL ARRAY:
@ ( 2 )san glob_arr.
@glob_arr 0<- 1.

```

### IMPORTANCE NOTICE: ###
TPL is strong static, which means it doesn't convert data types automatically and you can't manipulate different type data together:

drob d <- 2 + 0_5. // WRONG
san  s <- 0_0.     // WRONG

Global data can not defined inside any blocks, nor functions, not conditional or loop blocks.


### ARITHMETIC COMMANDS ###
Arithmetic operators are: +,-,*,/. Result is either san or drob:


```

1+2. // returns san
2 + 3 * ( 2 - 2  ). // returns san
5/2 . //returns san 

1_2 + 3_3 // returns drob

```

### COMPARE COMMANDS ###
Compare operators are: <, >, =, <=, =>.
Operands of compare operator must be in san type. Unless arithmetic operators, compare operators don't return anything:

```
san s <- 2_0 < 1_0. // WRONG.
```

*Compare operators used where you must check result*

### LOGIC COMMANDS ###
Logic operators are: &, ? and !.
Operands of logic operators must be compare commands or another logic commands.

& (and), ? (or) operators take two operands. In left side and right side.

! ( not ) operator takes one operand on front and reverse it's logic result:


*Logic operators used where you must check result*

```

san  s <- 1. 
drob pi <- 3_142526.  
harp h <- 's'.   

```

### IF STATEMENT ###
If statement formula is:

eger ( [compare or logic command]  ) bolsa
    // if statement block
ya   ( [compare or logic command] ) bolsa
    // else if statement block
yogsa
    // else block
===.

`eger` is equal to 'if' in english.
`ya`   is equal to 'else if' in english.
`yogsa` is equal to 'else' in english.

IMPORTANT
** *Inside of conditional or loop statement variable or array or user type can not be defined.* **

### LOOP STATEMENT ###
Formula is:
   ta ( [compare or logic command] ) bolyancha
       // loop statement block
   ===.

Loop statement block will be executed until check value is false and will stop when it comes to true:


```

san  s <- 1. 
ta ( s>5 ) bolyancha // executes 4 times.
   s <- s + 1.
===.

```

### BUILT-IN FUNCTIONS ###
TPL comes with only 2 functions. Those functions are available in source code. Functions are chap_et and kabul_et.

    chap_et function prints harpl type data on monitor. It takes only one parameter in HARPL type. There is code:
    ( [harpl type argument] )chap_et.
    chap_et function returns number of printed characters or length of string.

    kabul_et function takes characters from keyboard until user tap ENTER and returns inputted characters as HARPL data.
    kabul_et function has no arguments. There is code:
        ()kabul_et.

### CONVERTING DATA IN BASIC TYPES ###
To translate data from 1 basic type to another used converting type command:
    ( [basic_type] )[data]

[basic_type] - must be basic type keyword. (for example: san, drob).
[data]       - data in basic type. If data type and basic type to convert of data are same, TPL will get error. Some examples:

```

san s <- ( san ) 0_0.      // return  0
harpl hl <- (harpl)s.      // returns "0"
harpl h  <- (harp)"salam". // returns 's'
h     <- (harp)'b'.        // WRONG. CAN'T CONVERT SAME DATA TYPE DATA

```
Table of returned results:
![san_to_other.png](https://bitbucket.org/repo/A84RGB/images/387559274-san_to_other.png)
![drob_to_other.png](https://bitbucket.org/repo/A84RGB/images/3109615254-drob_to_other.png)
![harp_to_other.png](https://bitbucket.org/repo/A84RGB/images/3483793379-harp_to_other.png)
![harpl_to_other.png](https://bitbucket.org/repo/A84RGB/images/3887202243-harpl_to_other.png)

### USER DEFINED FUNCTIONS ###
Function definition code:
   ( [parameter,..] )[identifier] [return type] ->
    // function block
   ===.

*[parameter,..]* - is a list of parameters passed to function. It is either
    * empty
    * One or more variable or array definitions separated by comma
*[identifier]* - is function name
*[return type]* - return data type of function execution. It is either
    - type keyword.
    - hiç_zat keyword, representing "void" keyword in C.

Example of user defined functions:


```

()f hiç_zat ->
   ("Nothing=s")chap_et.
===.

()f.  // call defined function

(san x, san y)gosh san ->
   x + y yza.
===.

```
WARNGING:
*If function return something, `yza` command must be used. 
If function no returns, `yza` command should be avoided.*


### USER DEFINED TYPES ###
User type definition formula code:
  <: [elements,..] :>[identifier] tipi.

*[elements,..]* - list of variable or array definitions separated by comma. They are elements of complex type.
*[identifier]* - name of type.

Example

```

<: harpl name,
   (2)san coord  
 :>dot tipi.

```

USING USER TYPES:
Anywhere where you can use basic types, except where only basic types allowed (is data converting), using user types also allowed.


```

san  s.
harp h.
dot  dt.

//define function
(dot mouseX)func dot ->
   //...
===.

```

ACCESSING ELEMENTS OF USER TYPE:
Variables or arrays in user types can not be accessible to all elements at once. Each element must be accepted individually.

Formula of accessing elements of user type variables:
    [variable | array element | user type variable element] / ["element name"][indexes]

["element name"] - is data in HARPL, represents elements name.
[indexes] - if element is array, then indexes also must be used.


```

dot screenRes.
screenRes / "name" <- "benQ".
screenRes / "coord" 0 <- 23.
screenRes / "coord" 1 <- 32.


```

### DECLARATIONS ###
    Function, user type, global variable and array defined in B source code file, before using in A source, must be known by TPL. Either global data must be defined in current or previous source codes. If not defined yet, they must be declared.
    Declarations of global data writes on .bashy files. Bashy files once loaded are known while parsing all later source codes.
    In bashy files can be only:
* variable declarations
* array declarations
* user function declarations
* user type declarations

All other commands and pragma lines are not allowed.
    

Example of bashy file: 

```
// somewhere in source codes defined @s global variable
san  s.
// somewhere in source codes defined @arr global array
(2)  arr.
// function declaration aka prototype
( san x, san y )gosh san.
// user type
<: harpl name, (2)san coord :>dot tipi.

```

### COMMENTS ###
Single line comments:
 // ... until end of line
Multi-line comments:
 /* ... */

### PRAGMA ###
Pragma represents commands to TPL itself. They start with # letter followed by pragma code and ends with endline.
Between # sign, pragma code and end line can not be spaces.
There is 2 pragma commands:

#b1 - represents main code signal. Every program in TPL must have one #b1 pragma. Those source code where it was written, become start point.
#@"bashy source address" - represents declaration file parsing. TPL opens file inside double quotes and parse declarations.




### OPERATOR ORDER ###

   TOP. MOST PRIOR
   1. parenthesis. ()
   2. User type element separater.   / 
   3. Arithmetic. *, :
   4. Arithmetic. +, -
   5. Compare operators. (<, >, =, <=, >=)
   6. Logic Not operator. !
   7. Logic And, Or operators. &, ?
   8. Assign operator. <-
   9. Yza operator. yza.