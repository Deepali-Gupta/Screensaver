//LIBRARIES INCLUDED
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <GL/glut.h>
#include <unistd.h>

//HEADER FILES INCLUDED
#include "buttons.h"
#include "balls.h"
#include "main.h"
#include "display.h"

extern Button PauseButton;
extern Button IncButton;
extern Button DecButton;
extern Button PlusButton;
extern Button MinusButton;
extern Button GravityButton;
extern Mouse TheMouse;
extern Ball b[COUNT];

void ThePauseCallback()
{
	if(play)
	{
		play=false;
		#ifdef DEBUG
			printf("\nPause Called\n");
		#endif
	}
	else
	{
		play=true;
		#ifdef DEBUG
			printf("\nPlay Called\n");
		#endif
	}
}

void TheIncCallback()
{
	#ifdef DEBUG
		printf("\nIncrease Called\n");
	#endif
	acc[ballno]=1;
}

void TheDecCallback()
{
	#ifdef DEBUG
		printf("\nDecrease Called\n");
	#endif
	dcc[ballno]=1;
}

void ThePlusCallback()
{
	#ifdef DEBUG
		printf("\nSize Increased\n");
	#endif
	if(Radius<=95)
		Radius+=5;
}

void TheMinusCallback()
{
	#ifdef DEBUG
		printf("\nSize Decreased\n");
	#endif
	if(Radius>=10)
		Radius-=5;
}

void TheGravCallback()
{
	
	if(!gravity)
	{
		#ifdef DEBUG
			printf("\nGravity Acitvated\n");
		#endif
		gravity=9.8;
	}
	else
	{
		gravity=0;
		#ifdef DEBUG
			printf("\nGravity Deacitvated\n");
		#endif
	}
}

/*
 * \brief	Detects if Mouse is over the button b
 * \param	b	-	The Button	whose click is to be detected
 * \param	x	-	The Mouse x Coordinate
 * \parma	x	-	The Mouse y Coordinate
 */
int ButtonClickTest(Button* b,int x,int y) 
{
	if( b) 
	{
		/*
		 *	If clicked within button area, then return true
		 */
		 x=x-width/2;	//synchronize the mouse and opengl coordinate system
		 y=(-1)*y+height/2;
	    if( x > b->x && x < b->x+b->w && y > b->y  && y < b->y+b->h ) 
		{
			return 1;
		}
	}
	
	return 0;
}

/*
 * \brief	Detects if button is releases and restores the state of the button
 * \param	b	-	The Button	whose release is to be detected	
 * \param	x	-	The Mouse x Coordinate
 * \parma	x	-	The Mouse y Coordinate
 */

void ButtonRelease(Button *b,int x,int y)
{
	if(b) 
	{
		/*
		 *	If the mouse button was pressed within the button area
		 *	as well as being released on the button.....
		 */
		if( ButtonClickTest(b,TheMouse.xpress,TheMouse.ypress) && ButtonClickTest(b,x,y) )
		{
			/*
			 *	Then if a callback function has been set, call it.
			 */
			if (b->callbackFunction) 
			{
				b->callbackFunction();
			}
		}

		/*
		 *	Set state back to zero.
		 */
		b->state = 0;
	}
}

/*
 * \brief	Detects if button is pressed and changes the state of the button
 * \param	b	-	The Button	whose click is to be detected	
 * \param	x	-	The Mouse x Coordinate
 * \parma	x	-	The Mouse y Coordinate
 */

void ButtonPress(Button *b,int x,int y)
{
	if(b)
	{
		/*
		 *	if the mouse click was within the buttons client area, 
		 *	set the state to true.
		 */
		if( ButtonClickTest(b,x,y) )
		{
			b->state = 1;
		}
	}
}

/*
 * brief	Detects if mouse is over button and highlights it accordingly
 * \param	b	-	The button to be highlighted
 * \param	x	-	x coordinate of the mouse
 * \parma	y	-	y coordinate of the mouse
 */
void ButtonPassive(Button *b,int x,int y)
{
	if(b)
	{
		/*
		 *	if the mouse moved over the control
		 */
		if( ButtonClickTest(b,x,y) )
		{
			/*
			 *	If the cursor has just arrived over the control, set the highlighted flag
			 *	and force a redraw. The screen will not be redrawn again until the mouse
			 *	is no longer over this control
			 */
			if( b->highlighted == 0 ) {
				b->highlighted = 1;
				glutPostRedisplay();
			}
		}
		else

		/*
		 *	If the cursor is no longer over the control, then if the control
		 *	is highlighted (ie, the mouse has JUST moved off the control) then
		 *	we set the highlighting back to false, and force a redraw. 
		 */
		if( b->highlighted == 1 )
		{
			b->highlighted = 0;
			glutPostRedisplay();
		}
	}
}

/*
 * \brief	Defines the font of the button text
 * \param	font	-	The font of the text to be printed
 * \param	text	-	The text to be printed
 * \param	x		-	x coordinate of the text
 * \param	y		-	y cooridnate of the text
 */
void Font(void *font,const char *text,int x,int y)
{
	glRasterPos2i(x, y);
	while( *text != '\0' )
	{
		glutBitmapCharacter( font, *text );
		++text;
	}
}

void ButtonDraw(Button *b)
{
	int fontx;
	int fonty;
	if(b)
	{
		fontx = b->x + (b->w - glutBitmapLength(GLUT_BITMAP_HELVETICA_10,(const unsigned char*)b->label))/2 ;
		fonty = b->y + (b->h)/2;
			
		/*
		 *	if the button is pressed, make it look as though the string has been pushed
		 *	down. It's just a visual thing to help with the overall look....
		 */
		if (b->state) 
		{
			fontx+=2;
			fonty+=2;
		}

		/*
		 *	If the cursor is currently over the button we offset the text string and draw a shadow
		 */
		if(b->highlighted)
		{
			glColor3f(0,0,0);
			Font(GLUT_BITMAP_HELVETICA_10,b->label,fontx,fonty);
			fontx--;
			fonty--;
		}

		glColor3f(1,1,1);
		Font(GLUT_BITMAP_HELVETICA_10,b->label,fontx,fonty);
		/*
		 *	We will indicate that the mouse cursor is over the button by changing its
		 *	colour.
		 */
		if (b->highlighted) 
			glColor3f(0.7f,0.7f,0.8f);
		else 
			glColor3f(0.6f,0.6f,0.6f);

		/*
		 *	draw background for the button.
		 */
		glBegin(GL_QUADS);
		glVertex2i( b->x     , b->y      );
		glVertex2i( b->x     , b->y+b->h );
		glVertex2i( b->x+b->w, b->y+b->h );
		glVertex2i( b->x+b->w, b->y      );
		glEnd();

		/*
		 *	Draw an outline around the button with width 3
		 */
		glLineWidth(3);

		/*
		 *	The colours for the outline are reversed when the button is clicked
		 */
		if (b->state) 
			glColor3f(0.4f,0.4f,0.4f);
		else 
			glColor3f(0.8f,0.8f,0.8f);

		glBegin(GL_LINE_STRIP);
			glVertex2i( b->x+b->w, b->y      );
			glVertex2i( b->x     , b->y      );
			glVertex2i( b->x     , b->y+b->h );
		glEnd();

		if (b->state) 
			glColor3f(0.8f,0.8f,0.8f);
		else 
			glColor3f(0.4f,0.4f,0.4f);

		glBegin(GL_LINE_STRIP);
			glVertex2i( b->x     , b->y+b->h );
			glVertex2i( b->x+b->w, b->y+b->h );
			glVertex2i( b->x+b->w, b->y      );
		glEnd();

		glLineWidth(1);		
	}
}

/*
 * \brief	Operates on click of mouse button
 * \param 	button	-	which button of mous eis pressed
 * \param	state	-	mouse is pressed or released
 * \param	x		-	mouse x coordinate
 * \param	y		-	mouse y coordinate
 */
 
void MouseButton(int button,int state,int x, int y)
{
	/*
	 *	update the mouse position
	 */
	TheMouse.x = x;
	TheMouse.y = y;

	/*
	 *	has the button been pressed or released?
	 */
	if (state == GLUT_DOWN) 
	{
		#ifdef DEBUG
			printf("\nMouse clicked\n");
		#endif
		/*
		 *	This holds the location of the first mouse click
		 */
		if ( !(TheMouse.lmb) ) 
		{
			TheMouse.xpress = x;
			TheMouse.ypress = y;			
		}

		/*
		 *	Which button was pressed?
		 */
		 if(button==GLUT_LEFT_BUTTON)
		 {
			 TheMouse.lmb = 1;
			ButtonPress(&PauseButton,x,y);
			ButtonPress(&IncButton,x,y);
			ButtonPress(&DecButton,x,y);
			ButtonPress(&PlusButton,x,y);
			ButtonPress(&MinusButton,x,y);
			ButtonPress(&GravityButton,x,y);
			for(int i=0;i<COUNT;i++)
			{
				b[i].BallPress(x,y);
			}
				
		}
	}
	else 
	{
		/*
		 *	Which button was released?
		 */
		if(button==GLUT_LEFT_BUTTON)
		{
			TheMouse.lmb = 0;
			ButtonRelease(&PauseButton,x,y);
			ButtonRelease(&IncButton,x,y);
			ButtonRelease(&DecButton,x,y);
			ButtonRelease(&PlusButton,x,y);
			ButtonRelease(&MinusButton,x,y);
			ButtonRelease(&GravityButton,x,y);
			for(int i=0;i<COUNT;i++)
			{
				b[i].BallRelease(x,y);
			}
		}
	}

	/*
	 *	Force a redraw of the screen. If we later want interactions with the mouse
	 *	and the 3D scene, we will need to redraw the changes.
	 */
	glutPostRedisplay();
}

/*
 * \brief	Operates on hovering of mouse
 * \param	x		-	mouse x coordinate
 * \param	y		-	mouse y coordinate
 */

void MousePassiveMotion(int x, int y)
{
	/*
	 *	Calculate how much the mouse actually moved
	 */
	int dx = x - TheMouse.x;
	int dy = y - TheMouse.y;

	/*
	 *	update the mouse position
	 */
	TheMouse.x = x;
	TheMouse.y = y;

	ButtonPassive(&PauseButton,x,y);
	ButtonPassive(&IncButton,x,y);
	ButtonPassive(&DecButton,x,y);
	ButtonPassive(&PlusButton,x,y);
	ButtonPassive(&MinusButton,x,y);
	ButtonPassive(&GravityButton,x,y);
}



