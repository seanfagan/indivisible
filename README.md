# Indivisible
A simple game about the power of distribution.

## Concept Art
```
>>new

   +-----------------+
   | ...   ===   ... |
3  |  1     3     1  |
   | ...   ===   ... |
   |                 |
   | ...   ...   === |
2  |  2     1     1  |
   | ...   ...   === |
   |                 |
   | ...   ===   ... |
1  |  1     1     2  |
   | ...   ===   ... |
   +-----------------+
      A     B     C

* This region is expected to vote in favor of Terminal Party, 8 - 5.
* Please group them in favor of the Equal Party.
```
```
>>a3, b3, c3

   @@@@@@@@@@@@@@@@@@@
   @ ...   ===   ... @
3  @  1     3     1  @
   @ ...   ===   ... @
   @@@@@@@@@@@@@@@@@@@
     ...   ...   ===  
2     2     1     1   
     ...   ...   ===  
                      
     ...   ===   ...  
1     1     1     2   
     ...   ===   ...  

      A     B     C

+ Cracked! Equal Party will win 3-2 in A3/B3/C3.
* Equal Party: 1
* Terminal Party: 0
```
```
>>a1, a2, b2

   +-----------------+
   | ...   ===   ... |
3  |  1     3     1  |
   | ...   ===   ... |
   @@@@@@@@@@@@@-----+
   @ ...   ... @ ===  
2  @  2     1  @  1   
   @ ...   ... @ ===  
   @     @@@@@@@      
   @ ... @ ===   ...  
1  @  1  @  1     2   
   @ ... @ ===   ...  
   @@@@@@@
      A     B     C

+ Packed! Terminal Party will win 4-0 in A1/A2/B2.
* Equal Party: 1
* Terminal Party: 1
```
```
>>b1, c1, c2

   +-----------------+
   | ...   ===   ... |
3  |  1     3     1  |
   | ...   ===   ... |
   +-----------@@@@@@@
   | ...   ... @ === @
2  |  2     1  @  1  @
   | ...   ... @ === @
   |     @@@@@@@     @
   | ... @ ===   ... @
1  |  1  @  1     2  @
   | ... @ ===   ... @
   +-----@@@@@@@@@@@@@
      A     B     C

+ Tie. Uncertain who will win B1/C1/C2.
* Equal Party: 1
* Terminal Party: 1
* Tied: 1
```
```
>>vote

   +-----------------+
   | ===   ===   === |
3  | ===   ===   === |
   | ===   ===   === |
   +-----------+-----+
   | ...   ... | === |
2  | ...   ... | === |
   | ...   ... | === |
   |     +-----+     |
   | ... | ===   === |
1  | ... | ===   === |
   | ... | ===   === |
   +-----+-----------+
      A     B     C

* The people have voted!
* Swing county B1/C1/C2 voted in favor of Terminal Party!
*** Terminal Party wins the region 2-1, with 40% of the popular vote!
```
