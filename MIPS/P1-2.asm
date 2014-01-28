.data
TileGrid:	.alloc 25
Solution:	.alloc 25 
.text
Puzzle: addi $1, $0, TileGrid
        addi $17, $0, 1
        swi   563	     # Create Tile Puzzle
        andi $5, $2, 0xF     #number of col
        addi $6, $5, -1      #last col index
        andi $7, $2, 0xF0
        srl  $7, $7, 4       #number of row
        addi $8, $7, -1      #last row index
        mult $5, $7
        mflo $9              #size of the puzzle
        addi $11, $9, -1     #index of puzzle, row major,starting from the lower right corner 
        sll  $23, $11, 2
        add  $18, $1, $23    #address starting from the last tile
        add  $20, $18, $0
        add  $10, $0, $11    #the lower right corner tile index
        addi $16, $0, 0      #set match indicator to 0, meaning not matching
Init:   sw   $0, Solution($23)
        addi $23, $23, -4
        bne  $23, $0, Init
        sw   $0, Solution($23)
Check:  addi  $17,$0,1
        beq   $11, $10, Match   #if lower right corner tile, always match
        div   $11, $5
        mflo  $12               #current column
        mfhi  $13               #current row
        beq   $12, $8, RowOnly  #if last row tiles, check row only
        beq   $13, $6, CompCol  #if last col tiles, check col only  
        addi  $17, $0, 0        #indicate the tile is not last row/col
        j   CompRow             #if anywhere else, check row first
RowOnly: addi $17,$0,1
         j CompRow
CompRow: lb    $14, 0($18)      #read east color of the tile
         addi  $18, $18,4
         lb    $15, 2($18)      #read west color of the tile right of the current one
         addi  $18, $18,-4
         beq   $14, $15, Match  #if east-west matches, update match
         addi  $16, $0, 0        
         j   NewComb            #otherwise, rotate tiles to generate new combinations
CompCol: lb    $14, 1($18)      #read south color of the tile
         sll   $21, $5,2
         add   $21, $18, $21 
         lb    $15, 3($21)      #read north color of the tile above the current one
         addi  $17, $0, 1
         beq   $14, $15, Match  #if north-south matches, update match
         addi  $16, $0, 0       #set match indicator to 0
         addi  $16, $0, 0
         j   NewComb            #otherwise, rotate tiles to generate new combinations
Match:  addi $16, $0, 1         #set match indicator to 1
        beq  $17, $0, CompCol   #if tile not last col/row, continue to compare col 
        addi $11, $11, -1       #update tile index
        addi $18, $18, -4       #update tile address
        addi $28, $0,  -1
        bne  $11, $28, Check    #keep checking until the tile index is the last of the first row
        j    Exit 
NewComb: sll $23, $11, 2
         lw  $19, Solution($23)        #read rotate times
         slti $3, $19, 3
         bne  $3, $0, Increase
         addi $19,$0,0
         sll  $25, $24, 8
         srl  $26, $24, 24
         or   $24, $25, $26
         sw   $24, 0($18)  
         sw  $19, Solution($23)
         addi $18, $18, 4
         addi $11, $11, 1
         sll  $23, $11, 2
         lw   $24, 0($18)
         bne  $18, $20, NewComb
         lw  $19, Solution($23)
         slti $3, $19, 3
         bne $3, $0, Increase
Increase: addi $19,$19,1
          lw   $24, 0($18)
          sll  $25, $24, 8
          srl  $26, $24, 24
          or   $24, $25, $26
          sw   $24, 0($18)
          sw   $19, Solution($23)
          j Check
Exit:     addi $4, $0, Solution
          swi 569
          jr $31
