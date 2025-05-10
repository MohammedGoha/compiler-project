# IDENTIFIER
    I = I . L | I . D | L

# NUMBER
    N = D . N | D

**Letter**
L = A-Z | a-z
**Digit**
D = 0-9

# PROGRAM
    P = S . ; . P | S . ;

**Statement**
S = F | R | W | RE | CO

# IF STATEMENT
    F = (if . C . then . P) . (end | else . P . end)

**Condition**
C = E . > . E | E . = . E
**Expression**
E = ((I | N) . O . (I | N)) | (I | N)
**Operator**
O = * | / | + | -

# ASSIGMENT STATEMENT
    A = I . := . E

# REPEAT/UNTIL STATEMENT
    R = repeat . P . until . C

# READ STATEMENT
    RE = read . (T)

**Identifiers seperated by commas**
T = I | I , T 

# WRITE STATEMENT
    W = write . U

**String literal**
SL = "SE"

**Identifiers and/or strings seperated by commas**
U = I | SL | I , U | SL , U 

# COMMENT

    CO = {SE}

**Sentence**
SE = I . SE | N . SE | I | N
