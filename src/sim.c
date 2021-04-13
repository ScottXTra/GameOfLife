/*
 * ============================================================================
 *
 *       Filename:  sim.c
 *
 *    Description:  Main file of the project
 *
 *        Created:  04/12/2021
 *
 *         Author:  Scott
 *
 * ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <pthread.h>
#include <time.h>
#define XY 1024
#define XMAX XY
#define YMAX XY
int itter = 0;
int pixels[XMAX][YMAX] = {0};
int pixelsNextFrame[XMAX][YMAX] ={0};

int iteration = 0;


//STATS
float getDif();
int getAliveCount();
//SIM
void randomizePixels();    
void setNextFrame();
void multiThreadEvolve();
void * getNextEvolve(void * t);
int servives(int x,int y);
//UTIL
int rnd(int min,int max);

//OPENGL

/*
    Function: getDif
    Checks to see what percentage of pixels have changed since the last frame
*/
float getDif(){
    int changes = 0;
    for (int i = 0; i < XMAX; i++){
        for (int j = 0; j < YMAX; j++){
            if(pixelsNextFrame[i][j] != pixels[i][j]){
                changes++;
            }
        }
    }
    //printf("%d\n",changes);
    return (float)changes/((float)XMAX * (float)YMAX);
}
/*
    Function: getAliveCount
    Returns the number of currently alive pixels
*/
int getAliveCount(){
    int numalive =0;
    for (int i = 0; i < XMAX; i++){
        for (int j = 0; j < YMAX; j++){
            if(pixelsNextFrame[i][j] ==1){
                numalive++;
            }
             
            
        }
    }
    return numalive;
}
/*
    Function: randomizePixels
    Sets up the matrixs with some random noise
*/
void randomizePixels(){
    for (int i = 1; i < XMAX-1; i++){
        for (int j = 1; j < YMAX-1; j++){
            if(rnd(0,9) == 1){
                pixels[i][j] = 1;
                pixelsNextFrame[i][j] = 1;
            }
        }
    }
}

/*
    Function: setNextFrame
    Sets up the next frame
*/
void setNextFrame(){
    for (int i = 0; i < XMAX; i++){
        for (int j = 0; j < YMAX; j++){
            pixels[i][j] = pixelsNextFrame[i][j];
            
        }
    }
}
/*
    Function: multiThreadEvolve
    Splits the matrix into 4 and processes them in parallel for large performance boost
*/
void multiThreadEvolve(){
    
    pthread_t tid[4];
    int  start0 = 0;
    int  start1 = 1;
    int  start2 = 2;
    int  start3 = 3;
    pthread_create(&tid[0], NULL, getNextEvolve, &start0);
    pthread_create(&tid[1], NULL, getNextEvolve, &start1);
    pthread_create(&tid[2], NULL, getNextEvolve, &start2);
    pthread_create(&tid[3], NULL, getNextEvolve, &start3);
    
    for (int i = 0; i < 4; i++){
        pthread_join(tid[i], NULL);
    }
       
}
/*
    Function: getNextEvolve
    Evolves the current matrix into the next frame
    NOTES:
        Sloppily hard coded for threading (will fix later)
*/
void * getNextEvolve(void * t){
    int start = *(int *)t;
    if(start == 0){
        for (int i = 1; i <=  XMAX/2; i++){
            for (int j =1; j <=  YMAX/2; j++){
                pixelsNextFrame[i][j] = servives(i,j);
            }
            
        }
    }else if(start == 1){
        for (int i = 1; i <=  XMAX/2; i++){
            for (int j =YMAX/2; j <  YMAX-1; j++){
                pixelsNextFrame[i][j] = servives(i,j);
            }
        }
    }
    else if(start == 2){
        for (int i = XMAX/2; i <  XMAX-1; i++){
            for (int j = 1; j <=  YMAX/2; j++){
                pixelsNextFrame[i][j] = servives(i,j);
            }
        }
    }
    else if(start == 3){
        for (int i = XMAX/2; i <  XMAX-1; i++){
            for (int j = YMAX/2; j <  YMAX-1; j++){
                pixelsNextFrame[i][j] = servives(i,j);
            }
        }
    }
    return NULL;
}
/*
    Function: servives
    Returns the life state of a pixel in the next frame
    NOTES:
        Rules
        - Any live cell with two or three live neighbours survives.
        - Any dead cell with three live neighbours becomes a live cell.
        - All other live cells die in the next generation. Similarly, all other dead cells stay dead.
*/
int servives(int x,int y){
    int liveNeighbours = 0;
    
    if( pixels[x-1][y-1] == 1)
        liveNeighbours++;
    if( pixels[x-1][y] == 1)
        liveNeighbours++;
    if( pixels[x-1][y+1] == 1)
        liveNeighbours++;
    if( pixels[x][y-1] == 1)
        liveNeighbours++;
    if( pixels[x][y+1] == 1)
        liveNeighbours++;
    if( pixels[x+1][y-1] == 1)
        liveNeighbours++;
    if( pixels[x+1][y] == 1)
        liveNeighbours++;
    if( pixels[x+1][y+1] == 1)
        liveNeighbours++;
    
    //Cell is alive
    if(pixels[x][y]==1){
        if(liveNeighbours == 2 || liveNeighbours == 3 ){
            return 1;
        }else{

            return 0;
        }
    //Cell is dead
    }else{
        if(liveNeighbours == 3){
            return 1;
        }else{
            return 0;
        }
    }
    
}
/*
    Function: display
    OPENGL FUNCTION
    NOTES:
        Sloppily (will fix later)
*/
void display()
{
    
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0.0, 500.0, 500.0,0.0 );

    glBegin(GL_POINTS);
    glColor3f(1,1,1);
    
    for (int i = 0; i < XMAX; i++){
        for (int j = 0; j < YMAX; j++){
            if(pixels[i][j] ==1){
                //drawRec(i,j,i+1,j+1);
                glVertex2i(i,j);
            }
            
        }
    }
    
    glEnd();
    glutSwapBuffers();
}
/*
    Function: rnd
    Wrapper for rand()
*/
int rnd(int min,int max){
   return rand()%((max+1)-min) + min;
}
/*
    Function: update
    OPENGL FUNCTION
    NOTES:
        Sloppily (will fix later)
*/
void update(){
    long seed = time(NULL);
    srand(seed);
    randomizePixels();
    while(1){
        multiThreadEvolve();
        printf("ITER (%d)\n",iteration);
        display();
        setNextFrame();
        iteration++;
    }     
}
void init() // Called before main loop to set up the program
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
}
int main(int argc, char **argv)
{
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(XMAX, YMAX);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Sim");
    glutDisplayFunc(display);
    glutIdleFunc(update);
    init();
    glutMainLoop();
    return 0;
}

