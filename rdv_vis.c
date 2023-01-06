#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <ncurses.h>
#include <unistd.h>

struct Point{
	int x;
  int y; 	
}; 

int N = 4, nproc= 0, currLine = 15; 


pthread_t th[3];

sem_t prive, bin, bin2;

void *Personne(void *args); 
void rdv(int i);
  

struct Point pts[5]; 




int main(){
			
	initscr();
	start_color(); 
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK); 
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_WHITE);
	int i, j = 0; 
	for(i = 0; i < 4; i++ ){
		j	= j + 8; 
		pts[i].y = 0;
		pts[i].x = j;  
	}
	mvprintw(6, 0, "rdv");
	move(6, 3); 
	for(i = 0; i < 50; i++){
		printw("="); 
	} 
	refresh(); 	



	sem_init(&prive, 0, 0);  
	sem_init(&bin, 0, 1);  	
	sem_init(&bin2, 0, 1);  	
	for(i = 0; i < 4 ; i++){
		pthread_create(&th[i], NULL, Personne, (void*) i); 
	}
	for(i = 0; i < 4 ; i++){
		pthread_join(th[i], NULL); 
	}
	currLine++;
	attron(A_BOLD); 
	attron(COLOR_PAIR(1)); 
	mvprintw(currLine, 0,"Algorithm finished.");
	attroff(COLOR_PAIR(1));
	attroff(A_BOLD);
	getch(); 
  endwin(); 

	sem_destroy(&bin); 	
	sem_destroy(&bin2); 	
	sem_destroy(&prive); 	


	 	
	return 0;
}
void *Personne(void *args){
	int i = (int) args;  
	for (int j = 0; j < 5; j++){
		sem_wait(&bin2);
		mvprintw(pts[i].y, pts[i].x, "   "); 
		pts[i].y++;
	  mvprintw(pts[i].y, pts[i].x, "(%d)", i);  	
		refresh();
		sleep(1);
		sem_post(&bin2); 
	} 
	
	rdv(i);
	
	
	for (int j = 0; j < 3; j++){
		sem_wait(&bin2);
		mvprintw(pts[i].y, pts[i].x, "     "); 
		pts[i].y++;
	  mvprintw(pts[i].y, pts[i].x, "(%d)", i);  	
		refresh();
		sleep(1);
		sem_post(&bin2);
	} 
	sem_wait(&bin2);
		mvprintw(pts[i].y, pts[i].x, "    "); 
		pts[i].y++;
	  mvprintw(pts[i].y, pts[i].x, "(%d)F", i);  
	  attron(COLOR_PAIR(3)); 
	  attron(A_BOLD);	
		mvprintw(currLine, 0, "thread %d finished", i); 
		attroff(COLOR_PAIR(3)); 
		attroff(A_BOLD);	
		currLine++;
		refresh();
		sleep(1);
		sem_post(&bin2);


	pthread_exit(NULL);  
} 

void rdv(int i){

	sem_wait(&bin); 
	nproc++;

	if(nproc == N){
		attron(COLOR_PAIR(4));
		currLine++; 
		mvprintw(currLine , 0,"thread %d fait le reveille .", i);
		attroff(COLOR_PAIR(4));;	
		currLine = currLine + 2;
		mvprintw(pts[i].y, pts[i].x, "     "); 
		pts[i].y++;
	  mvprintw(pts[i].y, pts[i].x, "(%dR)", i);  	
		refresh();
		sleep(1);

		while( N != 1){

			N--;
			sem_post(&prive);  
		}
		sem_post(&bin); 
	} 
	else{
		
		sem_post(&bin); 
		sem_wait(&bin2);
		mvprintw(pts[i].y, pts[i].x, "     "); 
		pts[i].y++;
	  mvprintw(pts[i].y, pts[i].x, "(%dB)", i);  
	  attron(COLOR_PAIR(2)); 
	  mvprintw(currLine, 0, "thread %d blocked", i);
	  attroff(COLOR_PAIR(2));  	
	  currLine++;
		refresh();
		sleep(1);
		sem_post(&bin2);
		sem_wait(&prive); 
	}
}

