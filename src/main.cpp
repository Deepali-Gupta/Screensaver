//LIBRARIES INCLUDED
#include <math.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <GL/glut.h>
#include <unistd.h>

//HEADER FILES INCLUDED
#include "main.h"
#include "physics.h"
#include "balls.h"
#include "display.h"
#include "buttons.h"

//CONSTANTS
#define WIDTH 1000
#define HEIGHT 1000
#define LENGTH 1000
#define SPEED 0.2
#define RADIUS 60


#ifdef RANDOM
	int rndm=1;
#else
	int rndm=0;
#endif

int width=WIDTH;
int height=HEIGHT;
int length=LENGTH;
float speed=SPEED;
float Radius=RADIUS;

//Objects of class Ball and struct Mouse created
Ball b[COUNT];
Mouse TheMouse = {0,0,0,0,0};

//gravity flag
float gravity=0;

//pause/play flag
bool play=true;

//speed increase/decrease flags
int acc[COUNT]={};
int dcc[COUNT]={};

//temporary variables
int colour[COUNT];
int red[COUNT],blue[COUNT],green[COUNT];
int var_radius[COUNT];
int ctr[COUNT]={};

sem_t mutex_rdcnt, mutex_wrcnt, mutex_3, w, r;

/*
 * \brief	This is the Glut Idle Function 
 */
 
void idle()
{
   glutPostRedisplay();
}

/*
 * \brief	The thread call function, controls the movement of the balls
 * \param	j	-	thread id, used as ball id
 */

void *myThreadFun(void *j)
{
	int i= (long int) j;
	
	//static 	Array of ball data shared among the threads
	static float px[COUNT];   
	static float py[COUNT];  
	static float pz[COUNT];
	static float vx[COUNT];  
	static float vy[COUNT];  
	static float vz[COUNT];
	
	px[i]=b[i].get_px(); 
	py[i]=b[i].get_py(); 
	pz[i]=b[i].get_pz();
	vx[i]=b[i].get_vx(); 
	vy[i]=b[i].get_vy();
	vz[i]=b[i].get_vz();

	static int readcount=0;
	static int writecount=0;

	while (true)
	{ 
		#ifdef DEBUG
			std::ofstream file;
			file.open("log.txt",std::ios::app);
			file<<"Px"<<i<<" "<<px[i]<<" Py"<<i<<" "<<py[i]<<" Pz"<<i<<" "<<pz[i]
			    <<" Vx"<<i<<" "<<vx[i]<<" Vy"<<i<<" "<<vy[i]<<" Vz"<<i<<" "<<vz[i]<<"\n";
		#endif
		ctr[i]++;
		if(!play)
		{ 
			//Pause the threads
			continue;
			#ifdef DEBUG
				printf("\nScreen Paused\n");
			#endif
		}
		else 
		{
			float sp=sqrt(vx[i]*vx[i]+vy[i]*vy[i]+vz[i]*vz[i]);
			
			if(dcc[i]&&sp>=0.05) //minimum speed assumed
			{	
				float delta=0.8;
				dcc[i]=0;
				vx[i]*=delta;
				vy[i]*=delta;
				vz[i]*=delta;
				#ifdef DEBUG
					printf("\nSpeed of Ball %d decreased\n",i);
				#endif
			}
			
			if(acc[i]&&sp<=4) //maximum speed assumed
			{
				float delta = 1.25;
				acc[i]=0;
				vx[i]*=delta;
				vy[i]*=delta;
				vz[i]*=delta;
				#ifdef DEBUG
					printf("\nSpeed of Ball %d increased\n",i);
				#endif
			}
		
			if(gravity&&py[i]>0.5)
			{
				float delta=0.001;
				vy[i]-=gravity*delta;
			}
			if(rndm&&(ctr[i]%1000==0)) //randomly change the speed of the balls at 1000th iteration in random version
			{
				vx[i]=rand() * ((float)2*speed) / RAND_MAX + ((float)(-1)*speed);
				vy[i]=rand() * ((float)2*speed) / RAND_MAX + ((float)(-1)*speed);
				vz[i]=rand() * ((float)2*speed) / RAND_MAX + ((float)(-1)*speed);
			}
		
			//Update postions
			px[i] += vx[i];
			py[i] += vy[i];
			pz[i] += vz[i];
			b[i].set_py(py[i]); 
			b[i].set_px(px[i]);
			b[i].set_pz(pz[i]);
		
			//Change the radius for 3D effect
			var_radius[i] = Radius/2 + (2*pz[i]+length)*Radius/(4*length);
			b[i].set_rad(var_radius[i]);
		
			usleep(2000);
		
			red[i]=b[i].get_r2();
			blue[i]=b[i].get_b2();
			green[i]=b[i].get_g2();
		
			//if current color not equal to original color
			if (red[i] != b[i].get_r() or blue[i] != b[i].get_b() or green[i] != b[i].get_g())
			{
				if (red[i] != b[i].get_r())
				{
					red[i] -= 1;
					b[i].set_r2(red[i]);
				}
				if (blue[i] != b[i].get_b())
				{
					blue[i] -= 1;
					b[i].set_b2(blue[i]);
				}
				if (green[i] != b[i].get_g())
				{
					green[i] -= 1;
					b[i].set_g2(green[i]);
				}
			}

			else 
				colour[i] = 0;
    
			float tmp[7];
			
			//check for collision with walls
			float *arr= wall(px[i],py[i],pz[i],vx[i],vy[i],vz[i]);
                tmp[0]=arr[0];
                tmp[1]=arr[1];
                tmp[2]=arr[2];
                tmp[3]=arr[3];
                tmp[4]=arr[4];
				tmp[5]=arr[5];
                tmp[6]=arr[6];

                px[i]=tmp[0];
                py[i]=tmp[1];
				pz[i]=tmp[2];
                vx[i]=tmp[3];
                vy[i]=tmp[4];
				vz[i]=tmp[5]; 
                                	 
                int flag=tmp[6];
				
			if(flag)//change color on collision
			{	
				#ifdef DEBUG
					printf("\nCollision of ball %d with wall\n",i);
				#endif	   
				red[i] = 255;
				b[i].set_r2(red[i]);
				blue[i] = 255;
				b[i].set_b2(blue[i]);
				green[i] = 255;
				b[i].set_g2(green[i]);
				colour[i] = 1;
			}

			if(i<COUNT-1)
			{               
				for(int k=i+1;k<COUNT;k++)
				{       
					sem_wait(&mutex_3);
					sem_wait(&r);
					sem_wait(&mutex_rdcnt);
					readcount++;
					if (readcount == 1)
						sem_wait(&w);
    			
					sem_post(&mutex_rdcnt);
					sem_post(&r);
					sem_post(&mutex_3);
				 
					// Reading started
					float pxk=px[k]; float pyk=py[k];float pzk=pz[k]; float vxk=vx[k]; float vyk=vy[k]; float vzk=vz[k];
					float pxi=px[i]; float pyi=py[i]; float pzi=pz[i]; float vxi=vx[i]; float vyi=vy[i]; float vzi=vz[i];
					// Reading completed
 				 
					sem_wait(&mutex_rdcnt);
					readcount--;
					if (readcount == 0) 
						sem_post(&w);
					sem_post(&mutex_rdcnt);

					//detect overlapping of two balls i adn k
					if (overlap(pxi,pyi,pzi,pxk,pyk,pzk))
					{
						#ifdef DEBUG
							printf("\nOverlap between balls %d and %d\n",i,k);
						#endif
						red[i] = 255;
						b[i].set_r2(red[i]);
						red[k] = 255;
						b[k].set_r2(red[k]);
						blue[i] = 255;
						b[i].set_b2(blue[i]);
						blue[k] = 255;
						b[k].set_b2(blue[k]);
						green[i] = 255;
						b[i].set_g2(green[i]);
						green[k] = 255;
						b[k].set_g2(green[k]);
						colour[i] = 1;
						colour[k] = 1;

						float step = 0.1;
					
						while (overlap(pxi,pyi,pzi,pxk,pyk,pzk))
						{
							pxi-=step*vxi;
							pyi-=step*vyi;
							pzi-=step*vzi; 
							pxk-=step*vxk;
							pyk-=step*vyk;
							pzk-=step*vzk; 
						}
						float vel[6];
						float *temp= velocities(pxi,pyi,pzi,vxi,vyi,vzi,pxk,pyk,pzk,vxk,vyk,vzk);
							vel[0]=*temp++;
							vel[1]=*temp++;
							vel[2]=*temp++;
							vel[3]=*temp++;
							vel[4]=*temp++;
							vel[5]=*temp; 
						
						sem_wait(&mutex_wrcnt);
						writecount++;
						if (writecount == 1) 
							sem_wait(&r);
						sem_post(&mutex_wrcnt); 
						sem_wait(&w);
 					 
						px[i]=pxi; 
						py[i]=pyi; 
						pz[i]=pzi; 
						px[k]=pxk; 
						py[k]=pyk;  
						pz[k]=pzk;
						vx[i]=vel[0]; 
						vy[i]=vel[1];
						vz[i]=vel[2];
						vx[k]=vel[3]; 
						vy[k]=vel[4];
						vz[k]=vel[5]; 
   					 
						sem_post(&w); 
						sem_wait(&mutex_wrcnt);
						writecount--;
						if (writecount == 0) 
							sem_post(&r);
						sem_post(&mutex_wrcnt);
 					 
						b[i].set_vx(vx[i]);
						b[i].set_vy(vy[i]);
						b[i].set_vz(vz[i]); 					 
					}
				}
			}   
		}
	}
}

/*
 * \brief	The Main Function
 */
 
int main(int argc, char *argv[])
{
	#ifdef DEBUG
		printf("\nHi main\n");
	#endif
	sem_init(&mutex_rdcnt, 0, 1);
	sem_init(&mutex_wrcnt, 0, 1);
	sem_init(&mutex_3, 0, 1);
	sem_init(&w, 0, 1);
	sem_init(&r, 0, 1);
	pthread_t tid;
  
    glutInit (&argc, argv);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("ScreenSaver");
   
    init();
	 
	for(long int j=0;j<COUNT;j++) //Create threads
	{
		b[j].set_id(j);
		pthread_create(&tid, NULL, myThreadFun, (void *)j);
		#ifdef DEBUG
			printf("\nThread#%d created",(int)j);
		#endif
	}
	
    glutDisplayFunc(display);
	glutMouseFunc(MouseButton);	
	glutPassiveMotionFunc(MousePassiveMotion);
    glutIdleFunc(idle);  
    glutMainLoop();
    
    return 0;
}
