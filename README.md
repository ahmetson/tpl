# TPL - Testable pogramming language #

TPL - is a string static type, compiling, procedural programming language with turkmen language syntax. It's not a just replaced words of Mainstream language with non-english words, but language with new, great syntax understandable to turkmen language speakers.

TPL means 'testable programming language'.

## HOW TO RUN TPL PROGRAMS? ##

Program runs via command line. Just call 'tpl.exe' with adresses of source code as arguments. final program will be created where tpl.exe was called.

Program name that will be created by TPL made by first passed source code name.

# DOCUMENTATION #

### SCOPES ####

This programming language has 4 scope levels: TPL, global, local and function levels.

* TPL level - is the top level. Every function, variable come with tpl.exe, including third party libraries belongs to TPL level. And they are acceptable in other scopes.
* global level - is program level or in other words between source code level. In global level, data defined in A source code, can be used in B source code. User defined functions and user defined types ALWAYS defined in global scope. 
  Global variables and arrays always comes with "@" token on front:

      // define global variable
      @san s.     // define s variable in integer type
      @s <- 2+2.  // assign 2+2 to global s variable

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


### Contribution guidelines ###

* Writing tests
* Code review
* Other guidelines

### Who do I talk to? ###

* Repo owner or admin
* Other community or team contact