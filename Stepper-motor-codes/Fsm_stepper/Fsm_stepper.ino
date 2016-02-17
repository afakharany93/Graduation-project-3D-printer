/*if it works it was done by Ahmed El Fakharany to test the finite state machine concept with stepper motors for the 3d printer graduation project, 
if it doesn't workm I don't know who the hell did it but I'm sure it is not Ahmed El Fakharany :D */

#define DELAY_uS 		800		//amount to delay in us
#define DELAY_mS		1
//mapping the pins with wire colors
#define BLACK 			9
#define BLUE 			11
#define RED 			10
#define GREEN 			8
//mapping wire colors with firing order
#define FIRST 			GREEN
#define SECOND   		RED
#define THIRD   		BLACK 
#define FORTH  			BLUE
//mapping directions motion with state transition direction
#define NEXT 			1
#define PREVIOUS 		0
#define CLOCKWISE 		NEXT
#define ANTICLOCKWISE 	PREVIOUS

/*struct stepper_state_struct is a struct used to hold the info concerning the states, each state resemnles one step,
 it holds the output of the state and a pointer to the next state to use to step forward and
 a pointer to the previous step in case of backwards */

typedef struct stepper_state_struct
{
	unsigned char out;						//used to hold the output values to all pins, this value needs to be inturpeted later
  	struct stepper_state_struct	*nxt;		//used to hold a pointer to the next state which resmbles the next step on the stepper motor, used for forward motion
	struct stepper_state_struct	*prev;		//pointer used to hold the address to the previos state, used for backwards motion
} stepper_state_struct;

typedef struct timer1_value
{
	unsigned int prescale;
	float time_per_tick_us;
	unsigned long int max_period_us;
}timer1_value;

/*Function name : stepper_output
  return : void
  parameters : struct stepper_state_struct *current_state :- pointer to struct, used for call by refrence for the variable containing the information of the current state
  Functionality : to  output the ouy member in the current_state struct to the pins, use after next_step or previos_step functions
 */
void stepper_output (struct stepper_state_struct *current_state);
/*
	Function name : next_step
  	return : void
  	parameters : struct stepper_state_struct *current_state :- pointer to struct, used for call by refrence for the variable containing the information of the current state
  	Functionality : To make resdy the stepper to take the next step in a direction
*/
void next_step (struct stepper_state_struct *current_state);
/*
	Function name : previos_step
  	return : void
  	parameters : struct stepper_state_struct *current_state :- pointer to struct, used for call by refrence for the variable containing the information of the current state
  	Functionality : to ready the stepper to take the previous step in a direction opposite to that of next_step function
*/
void previos_step (struct stepper_state_struct *current_state);

//global variables and arrays

/*stepper_states is an array that holds the constant values of all the states of the stepper motor */
struct stepper_state_struct stepper_states[4] = 
{
  //{out  , next state         , previos state      }
	{0x01 , &stepper_states[1] , &stepper_states[3] } ,
	{0x02 , &stepper_states[2] , &stepper_states[0] } ,
	{0x04 , &stepper_states[3] , &stepper_states[1] } ,
	{0x08 , &stepper_states[0] , &stepper_states[2] }
};

struct timer1_value timer1_value_lookup_table[5] = 
{
	{1 	  , 0.0625  , 4096   },
	{8 	  , 0.5 	, 32767  },
	{64   , 4 	 	, 262140 },
	{256  , 16	 	, 1048560},
	{1024 , 64 		, 4194240}
};

struct stepper_state_struct current_state = stepper_states[0];		//the variable that will hold the current state information, initialized with state zero info
unsigned long int 	stepper_steps;
unsigned char 		direction;


void setup() 
{
	//setting pin modes
	pinMode(FIRST, OUTPUT);
	pinMode(SECOND, OUTPUT);
	pinMode(THIRD, OUTPUT);
	pinMode(FORTH, OUTPUT);
	//setting all pins as low
	digitalWrite(FIRST, LOW);
	digitalWrite(SECOND, LOW);
	digitalWrite(THIRD, LOW);
	digitalWrite(FORTH, LOW);

	interrupts();             // enable all interrupts
	stepper_move (90000, 800 );
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
	if(stepper_steps > 0)
	{
		if (direction == NEXT)
		{			
			stepper_output (&current_state);
			next_step(&current_state);
		}
		else if (direction == PREVIOUS)
		{
			stepper_output (&current_state);
			previos_step(&current_state);
		}
		stepper_steps--;
	}
	else
	{
		stepper_steps = 0;
		TIMSK1 = 0;	//disable timer compare interrupt
	}
}

void loop() 
{
	
	//stepper_move (90000, 800 );
	
	//delay(1000);

}


/************** Functions **************/
//stepper functions :
/*Function name : stepper_output
  return : void
  parameters : struct stepper_state_struct *current_state :- pointer to struct, used for call by refrence for the variable containing the information of the current state
  Method of operation : to interpert and output the out member of the stepper_state_struct variable holding currrent state information
 */
void stepper_output (struct stepper_state_struct *current_state)
{
	digitalWrite(FIRST  , (current_state->out & 0x01));		/*if bit one in the out member of the stepper_state_struct variable holding currrent state information is one
															then the pin mapped to FIRST will be high, if not it will be zero	*/				
	digitalWrite(SECOND , (current_state->out & 0x02));		//same as the line above but with bit 2 and pin mapped to SECOND
	digitalWrite(THIRD  , (current_state->out & 0x04));		//same as the line above but with bit 3 and pin mapped to THIRD
	digitalWrite(FORTH  , (current_state->out & 0x08));		//same as the line above but with bit 4 and pin mapped to FORTH
}

/*
	Function name : next_step
  	return : void
  	parameters : struct stepper_state_struct *current_state :- pointer to struct, used for call by refrence for the variable containing the information of the current state
  	Method of operation : to change the current state variable to the variable pointed at with the pointer member nxt in the stepper_state_struct, i.e. to put the current 
  						  state in next state and take the next step, used for going forwards
*/
void next_step (struct stepper_state_struct *current_state)
{
	*current_state = *current_state->nxt;
}

/*
	Function name : previos_step
  	return : void
  	parameters : struct stepper_state_struct *current_state :- pointer to struct, used for call by refrence for the variable containing the information of the current state
  	Method of operation : to change the current state variable to the variable pointed at with the pointer member prev in the stepper_state_struct, i.e. to put the current 
  						  state in previos state and take the previos step, used for going backwards
*/
void previos_step (struct stepper_state_struct *current_state)
{
	*current_state = *current_state->prev;
}

void stepper_move (long int steps, unsigned long int time_bet_steps_stepper )
{
	if (steps > 0)
	{
		direction = CLOCKWISE;
		stepper_steps = steps;
	}
	else if (steps < 0)
	{
		direction = ANTICLOCKWISE;
		stepper_steps = steps * (-1);
	}
	else if(steps == 0)
	{
		stepper_steps = 0;
	}
	timer1_setup ( timer1_value_lookup_table , time_bet_steps_stepper);
}

//timer functions:

struct timer1_value * prescale_determination (struct timer1_value *timer1_value_lookup_table_ptr_for_prescale , unsigned long int time_bet_steps_for_prescale)
{
	unsigned char counter;
	for (counter = 0; counter < 5 ; counter++)
	{
		if ( (timer1_value_lookup_table_ptr_for_prescale[counter].max_period_us) > time_bet_steps_for_prescale)
		{
			break;
		}
	}
	
	return &timer1_value_lookup_table_ptr_for_prescale[counter] ;
}

unsigned int ctc_value_determination (struct timer1_value *timer1_value_lookup_table_ptr_for_ctc , unsigned long int time_bet_steps_for_ctc)
{
	return (time_bet_steps_for_ctc / (timer1_value_lookup_table_ptr_for_ctc-> time_per_tick_us));
}

void timer1_setup (struct timer1_value *timer1_value_lookup_table_ptr , unsigned long int time_bet_steps)
{
	//temporary varu=iables to hold the values returned from the functions
	struct timer1_value *timer1_value_lookup_table_ptr_temp;
	unsigned int ctc_value;
	timer1_value_lookup_table_ptr_temp = prescale_determination (timer1_value_lookup_table_ptr , time_bet_steps);
	ctc_value = ctc_value_determination(timer1_value_lookup_table_ptr_temp , time_bet_steps);
	//registers initialization
	TCCR1A = 0;
  	TCCR1B = 0;
  	TCNT1  = 0;
  	TIMSK1 = 0;

  	OCR1A =  ctc_value;
  	TCCR1B |= (1 << WGM12);   // CTC mode
  	if ( (timer1_value_lookup_table_ptr_temp->prescale) == 1 )
  	{
  	 TCCR1B |= (1 << CS10);   
	}
	else if ( (timer1_value_lookup_table_ptr_temp->prescale) == 8 )
  	{
  	 TCCR1B |= (1 << CS11);   
	}
	else if ( (timer1_value_lookup_table_ptr_temp->prescale) == 64 )
  	{
  	 TCCR1B |= (1 << CS10);   
  	 TCCR1B |= (1 << CS11);   
	}
	else if ( (timer1_value_lookup_table_ptr_temp->prescale) == 256 )
  	{
  	 TCCR1B |= (1 << CS12);   
	}
	else if ( (timer1_value_lookup_table_ptr_temp->prescale) == 1024 )
  	{
  	 TCCR1B |= (1 << CS10);   
  	 TCCR1B |= (1 << CS12);   
	}
	TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
}