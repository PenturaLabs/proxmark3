OS Support
Confirmed Functional on following OSs:
* Gentoo Linux
* Debian Linux
* Ubuntu Linux
* OSX

Known Problems / Linker Issues
* Windows
* Cygwin
* Android

Operation
===============

hf mfu urdbl   - Read Ultralight Block  
hf mfu urdcard - Read Ultralight Card
hf mfu udump - Dump Ultralight Card
hf mfu uwrbl - Read Ultralight Block

hf mfu ucrdbl - Read Ultralight C Block
hf mfu ucrdcard - Read Ultralight C Card
hf mfu ucdump - Dump Ultralight C Card
hf mfu ucwrbl - Write Ultralight C Block
hf mfu auth - Attempt to Authenticate to Ultralight C Card


Demo...

proxmark3> hf mfu ucrdcard
Attempting to Read Ultralight C...
#db# READ CARD FINISHED
isOk:01
Block 00:04 0e 6b e9
Block 01:ca 0b 28 80
Block 02:69 48 00 00
Block 03:00 00 00 00 [0]
Block 04:02 00 00 10 [0]
Block 05:00 06 01 10 [0]
Block 06:11 ff 00 00 [0]
Block 07:ff ff ff ff [0]
...abreviated...
Block 2b:00 00 00 00 [0]

proxmark3> hf mfu auth
#db# Auth1 Resp: af1eae15f85b05e32d99b5                 
#db# AUTH 1 FINISHED                 
enc(RndB):1e ae 15 f8 5b 05 e3 2d           
     RndB:13 46 86 a9 4b f7 94 cd           
     RndA:9b 75 fe 7f 5b 9e ba 79           
     RA+B:9b 75 fe 7f 5b 9e ba 79 46 86 a9 4b f7 94 cd 13           
enc(RA+B):62 7a b7 02 0c fe c7 8b a2 4e 6b 43 5e 0f a0 b7           
#db# len b                 
#db# Auth2 Resp: 00fcb27f6e3d5db88b8e                 
#db# AUTH 2 FINISHED                 
isOk:88 Resonse:00 00 00 00 00 00 00 00           
proxmark3> hf 14a list
Recorded Activity          
          
Start = Start of Start Bit, End = End of last modulation. Src = Source of Transfer          
All times are in carrier periods (1/13.56Mhz)          
          
     Start |       End | Src | Data          
-----------|-----------|-----|--------          
         0 |       992 | Rdr | 52              
      2404 |      4772 | Tag | 44  00              
      7040 |      9504 | Rdr | 93  20              
     10852 |     16740 | Tag | 88  04  0e  6b  e9              
     18816 |     29280 | Rdr | 93  70  88  04  0e  6b  e9  2c  90              
     30692 |     34212 | Tag | 04  da  17              
     35456 |     37920 | Rdr | 95  20              
     39268 |     45092 | Tag | ca  0b  28  80  69              
     47232 |     57760 | Rdr | 95  70  ca  0b  28  80  69  69  f1              
     59108 |     62692 | Tag | 00  fe  51              
     66176 |     70944 | Rdr | 1a  00  41  76              
     82660 |     95460 | Tag | af  1e  ae  15  f8  5b  05  e3  2d  99  b5              
   1031296 |   1053344 | Rdr | af  62  7a  b7  02  0c  fe  c7  8b  a2  4e  6b  43  5e  0f  a0  b7  96  df              
   1065060 |   1077796 | Tag | 00  fc  b2  7f  6e  3d  5d  b8  8b  8e  cc 
