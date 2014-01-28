// puzzle solver C code for ECE 2035 Project1
// @author Yingyan Samantha Wang
// @version 1.0 -- Feb 26th 2013

#include <stdio.h>
#include <stdlib.h>

void solver(int row_size, int column_size, int num_color, int *packed_puzzle, int *solution) {
    void NewCombination(); // a function to generate a new combination of tiles.
    void Rotate(); // a function to rotate each tile;
    int CheckSingle(); // a function to check if each tile matches or not;
    int Find(); 
    // a function to find the index of the last tile of a consecutive 
    // series of tiles that has rotated 3 times.
    int CheckRowMatch(); // a function to check if each row meets the rule;
    int CheckColMatch(); // a function to check if each column meets the rule;
    int i, j;
    int *unpacked_puzzle;
    int *rotate_times;
    // In the following example, the unpacked puzzle is a row*(column*4) array, where the puzzle  is laid out
    // in the following order: N, E, S, W of (1,1), N, E, S, W of (1, 2), ... , the next row, ...
    // It's perfectly fine if you choose a different data layout.
    unpacked_puzzle = (int *)malloc( sizeof(int) * row_size * column_size * 4);
    rotate_times = (int *)malloc( sizeof(int) * row_size * column_size);
    for(i=0;i<row_size;i++) {
	for(j=0;j<column_size;j++) {
	    unpacked_puzzle[(i*column_size+j)*4+1]=(packed_puzzle[i*column_size+j] &  0xff) ;
            unpacked_puzzle[(i*column_size+j)*4+2]=(packed_puzzle[i*column_size+j] & 0xff00) >> 8;
	    unpacked_puzzle[(i*column_size+j)*4+3]=(packed_puzzle[i*column_size+j] & 0xff0000) >> 16;
	    unpacked_puzzle[(i*column_size+j)*4]=(packed_puzzle[i*column_size+j] & 0xff000000) >> 24;
	}        
    }
    for (i=0;i<row_size-1;i++){
        for(j=0;j<column_size-1;j++){
	    rotate_times[i*column_size+j]=0;
        }
    }
    //initializing the array that keeps track of how many degrees each tile has rotated.
    int match;
    int match_row;
    int match_col;
    int match_start=row_size*column_size-1;
    while (Find(0,row_size*column_size,rotate_times)<row_size*column_size){
        match_row=CheckRowMatch(0,row_size,column_size,unpacked_puzzle);
        match_col=CheckColMatch(0,row_size,column_size,unpacked_puzzle);
        match=match_row+match_col;
        if ((match)==(column_size*(row_size-1)+row_size*(column_size-1))){    
              for(i=0;i<row_size*column_size;i++){                  
                    solution[i]=rotate_times[i];
              }
                    return;
        } 
        //store the solution to puzzle when all the adjacent edges match and exit.           
        else{       
            int check=CheckSingle(match_start,row_size,column_size,unpacked_puzzle);
            if (check==1){
                match_start=match_start-1;  
            }
            else{
                  int n=Find(match_start,row_size*column_size,rotate_times);
                  NewCombination(match_start,row_size*column_size,rotate_times,unpacked_puzzle);
                  match_start=n+1;
                  
                  
            }
        }
        //check each tile if it matches the one below it and to the right of it.
        //if matches, move on to the next tile to the left of it; if not, generate
        //a new combination and check the entire puzzle again.
        
    }        
	return;
}

int CheckRowMatch(int start,int r,int c,int Array3[]){
       int i; 
       int matchr=0;
         for(i=start;i<r*c;i++){
             if((i%c)!=c-1){
                if(Array3[i*4+1]==Array3[i*4+7]){
                    matchr++;
                }
                else{
                    break;      
                }          
            }
        }
        return matchr;
}

int CheckColMatch(int start,int r,int c,int Array3[]){
       int j;
       int matchc=0;
       for (j=start;j<r*c;j++){
            int current_row=(j-(j%c))/c;
            if(current_row!=r-1){
               if(Array3[j*4+2]==Array3[c*4+j*4]){
                    matchc++;
               }
               else{
                   break;                
               }
           }
      }
        return matchc;
}
              
void NewCombination(int start,int end,int Array[],int Array2[]){
    void Rotate();
    int Find();
    int m;
    int k=Find(start,end,Array); 
    if (k>start-1){            
        for (m=start;m<k+1;m++){
            Rotate(m,Array2);                 
            Array[m]=0;
        } 
        Rotate(k+1,Array2);
        Array[k+1]++;       
    }
    else{
        Rotate(start,Array2);
        Array[start]++;
    }  
}
//the NewCombination function performs an addition on base four;

int Find(int start,int end,int Array[]){
    int k=start-1;
    int i;
    for(i=start;i<end;i++){
        if (Array[i]==3){
            k=i;
        }
        else if (Array[i]!=3){
            break;
        }        
    }
return k;
}      

void Rotate(int x, int Array[],int times[]){
    int rotate_holder[4]={0}; // a temporary space holder;
    rotate_holder[1]=Array[x*4];
    rotate_holder[2]=Array[x*4+1];
    rotate_holder[3]=Array[x*4+2];
    rotate_holder[0]=Array[x*4+3];
    Array[x*4]=rotate_holder[0];
    Array[x*4+1]=rotate_holder[1];
    Array[x*4+2]=rotate_holder[2];
    Array[x*4+3]=rotate_holder[3];   
}

int CheckSingle(int tile_ndx,int r,int c, int unpacked_puzzle[]){
    int match=0;
    int current_col=(tile_ndx%c);
    int current_row=(tile_ndx-(tile_ndx%c))/c;    
    if (tile_ndx==r*c-1){
        match=1;
    }
    else if (current_col==c-1){
        if (unpacked_puzzle[tile_ndx*4+2]==unpacked_puzzle[tile_ndx*4+c*4]){
            match=1;
        }
    }
    else if(current_row==r-1){
        if (unpacked_puzzle[tile_ndx*4+1]==unpacked_puzzle[tile_ndx*4+7]){
            match=1;
        }
    }
    else{
        if ((unpacked_puzzle[tile_ndx*4+1]==unpacked_puzzle[tile_ndx*4+7])
            &&(unpacked_puzzle[tile_ndx*4+2]==unpacked_puzzle[tile_ndx*4+c*4])){
            match=1;
        }
    }
    return match;
}




