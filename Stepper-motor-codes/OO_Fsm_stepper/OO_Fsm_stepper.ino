/*if it works it was done by Ahmed El Fakharany to test the finite state machine concept with stepper motors for the 3d printer graduation project, 
if it doesn't workm I don't know who the hell did it but I'm sure it is not Ahmed El Fakharany :D */

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
#define CLOCKWISE 		PREVIOUS
#define ANTICLOCKWISE 	NEXT
#define FORWARD 		CLOCKWISE
#define BACKWARD 		ANTICLOCKWISE


/*struct stepper_state_struct is a struct used to hold the info concerning the states, each state resemnles one step,
it holds the output of the state and a pointer to the next state to use to step forward and
a pointer to the previous step in case of backwards */

typedef struct stepper_state_struct
{
	unsigned char out;						//used to hold the output values to all pins, this value needs to be inturpeted later
 	struct stepper_state_struct	*nxt;		//used to hold a pointer to the next state which resmbles the next step on the stepper motor, used for forward motion
	struct stepper_state_struct	*prev;		//pointer used to hold the address to the previos state, used for backwards motion
} stepper_state_struct;
/* struct timer1_value is used in the lookup table used to determine the value of the prescale and the determination of the value of OCR1A register
 to be able to operate with the timer1 asagile as possible */
typedef struct timer1_value
{
	unsigned int prescale;				//to hold the value of the prescale
	float time_per_tick_us;				//to hold the value of time needed to make one tick in the timer in micro seconds
	unsigned long int max_period_us;	//to haold the max value the timer can hold in micro seconds
}timer1_value;

/*********** global variables and arrays ***********/
struct stepper_state_struct current_state;		//the variable that will hold the current state information, initialized with state zero info
unsigned long int 	stepper_steps;			//this variable holds the number of steps remained to be moved, needed by the isr
unsigned char 		direction;				//this variable holds the direction of movement, needed by the isr
unsigned char		stepper_permission = 1;		//used to prevent stepper_move function fromoverwriting itself, to execute stepper_move set it to 1, to stop the overwrting set it to 0

class stepper_3d
{
	public:
		stepper_3d ();
		/*
			Function name : stepper_move
		  	return : void
		  	parameters : long int steps :- variable to hold the value of the number of steps needed to be move, if +ve the motion is in a direction, if -ve the motion is in reverse
		  				 unsigned long int time_bet_steps_stepper :- time between each step, this is the value that determines the speed of motion
		  	Method of operation : its a non blocking function, used to move the stepper motor when it has permission i.e.(stepper_permission == 1), 
		  						  it sets the values of the global variables direction and stepper_steps, and sends the parameters needed to the timer setup function
		*/
		void stepper_move (long int steps, unsigned long int time_bet_steps_stepper );

		/*
			Function name : stepper_stop
		  	return : void
		  	parameters : none
		  	Method of operation : it stops - or pauses- the motion of the stepper motor, it actually stops the timer interrupt since the timer ISR is the function that does the motion 
		*/
		void stepper_stop ();

		/*
			Function name : stepper_resume
		  	return : void
		  	parameters : none
		  	Method of operation : it is used to resume the motion after stopping
		*/
		void stepper_resume ();

		/*
			Function name : stepper_flow
		  	return : void
		  	parameters : unsigned char direction_flow :- to determine the direction of motion
		  	Method of operation : it is used to move the stepper motor in a very big nummber of steps that it seems it moves for infinityyyyy, but it doesn't, 
		  						  best used for moving the stepper motor till a physical event happens to stop it
		*/
		void stepper_flow (unsigned char direction_flow);
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

	private:
		/*stepper_states is an array that holds the constant values of all the states of the stepper motor */
		struct stepper_state_struct stepper_states[4] = 
		{
		  //{out  , next state         , previos state      }
			{0x01 , &stepper_states[1] , &stepper_states[3] } ,
			{0x02 , &stepper_states[2] , &stepper_states[0] } ,
			{0x04 , &stepper_states[3] , &stepper_states[1] } ,
			{0x08 , &stepper_states[0] , &stepper_states[2] }
		};

		/* timer1_value_lookup_table is an array that holds the values of the prescales and the time per tick and the max time value for each prescale */
		struct timer1_value timer1_value_lookup_table[5] = 
		{
		 //{prescale , time_per_tick_us , max_period_us}
			{1 	  	, 0.0625  			, 4096   	   },
			{8 	  	, 0.5 				, 32767  	   },
			{64   	, 4 	 			, 262140 	   },
			{256  	, 16	 			, 1048560	   },
			{1024 	, 64 				, 4194240	   }
		};

		/*
			Function name : prescale_determination
		  	return : struct timer1_value * :- pointer to the element of the timer1_value_lookup_table with the right prescale
		  	parameters : struct timer1_value *timer1_value_lookup_table_ptr_for_prescale :- pointer to the timer1_value_lookup_table array
		  				 unsigned long int time_bet_steps_for_prescale :- used to hold the time between each step in microseconds
		  	Method of operation : it searches the timer1_value_lookup_table array for the suitable prescale and returns a pointer to the member with the suitable prescale
		*/
		struct timer1_value * prescale_determination (struct timer1_value *timer1_value_lookup_table_ptr_for_prescale , unsigned long int time_bet_steps_for_prescale);

		/*
			Function name : ctc_value_determination
		  	return : unsigned int:- used to return the value needed for the OCR1A register in ctc mode
		  	parameters : struct timer1_value *timer1_value_lookup_table_ptr_for_prescale :- pointer to the suitable element in the timer1_value_lookup_table array
		  				 unsigned long int time_bet_steps_for_ctc :- used to hold the time between each step in microseconds
		  	Method of operation : it calculates the value neede to be in the OCR1A register for the isr to work in the right perioo of time
		*/
		unsigned int ctc_value_determination (struct timer1_value *timer1_value_lookup_table_ptr_for_ctc , unsigned long int time_bet_steps_for_ctc);

		/*
			Function name : timer1_setup
		  	return : void
		  	parameters : struct timer1_value *timer1_value_lookup_table_ptr_for_prescale :- pointer to the suitable element in the timer1_value_lookup_table array
		  				 unsigned long int time_bet_steps :- used to hold the time between each step in microseconds
		  	Method of operation : it sets the registers for timer 1 to the right prescale value and ctc value and enables the timer one ctc interrupt
		*/
		void timer1_setup (struct timer1_value *timer1_value_lookup_table_ptr , unsigned long int time_bet_steps);
};


stepper_3d::stepper_3d()
{
	current_state = stepper_states[0];
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
}

//stepper functions :
/*Function name : stepper_output
  return : void
  parameters : struct stepper_state_struct *current_state :- pointer to struct, used for call by refrence for the variable containing the information of the current state
  Method of operation : to interpert and output the out member of the stepper_state_struct variable holding currrent state information
 */
void stepper_3d::stepper_output (struct stepper_state_struct *current_state)
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
void stepper_3d::next_step (struct stepper_state_struct *current_state)
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
void stepper_3d::previos_step (struct stepper_state_struct *current_state)
{
	*current_state = *current_state->prev;
}

/*
	Function name : stepper_move
  	return : void
  	parameters : long int steps :- variable to hold the value of the number of steps needed to be move, if +ve the motion is in a direction, if -ve the motion is in reverse
  				 unsigned long int time_bet_steps_stepper :- time between each step, this is the value that determines the speed of motion
  	Method of operation : its a non blocking function, used to move the stepper motor when it has permission i.e.(stepper_permission == 1), 
  						  it sets the values of the global variables direction and stepper_steps, and sends the parameters needed to the timer setup function
*/
void stepper_3d::stepper_move (long int steps, unsigned long int time_bet_steps_stepper )
{
	if (stepper_permission == 1)	//if you have permission, execute
	{
		stepper_permission = 0;		//permission is used for one time only, this line expires it, to execute again a new permission must be granted
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
			stepper_steps = 0;	//for safety and anti user stupidity
		}
		timer1_setup ( timer1_value_lookup_table , time_bet_steps_stepper);		//make the timer do its magic
	}
}

/*
	Function name : stepper_stop
  	return : void
  	parameters : none
  	Method of operation : it stops - or pauses- the motion of the stepper motor, it actually stops the timer interrupt since the timer ISR is the function that does the motion 
*/
void stepper_3d::stepper_stop ()
{
	TCCR1B &= (~(1 << WGM12));   // disable timer CTC mode
	TIMSK1 = 0 ;  // disable timer compare interrupt
}

/*
	Function name : stepper_resume
  	return : void
  	parameters : none
  	Method of operation : it is used to resume the motion after stopping
*/
void stepper_3d::stepper_resume ()
{
	TCCR1B |= (1 << WGM12);   // CTC mode
	TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
}

/*
	Function name : stepper_flow
  	return : void
  	parameters : unsigned char direction_flow :- to determine the direction of motion
  	Method of operation : it is used to move the stepper motor in a very big nummber of steps that it seems it moves for infinityyyyy, but it doesn't, 
  						  best used for moving the stepper motor till a physical event happens to stop it
*/
void stepper_3d::stepper_flow (unsigned char direction_flow)
{
	if (direction_flow == CLOCKWISE )
	{
		stepper_move (2147483647, 1000 );	//move max number of steps in a direction
	}
	else if (direction_flow == ANTICLOCKWISE )
	{
		stepper_move (-2147483647, 1000 );	//move max number of steps oin the other direction
	}
}



//timer functions:

/*
	Function name : prescale_determination
  	return : struct timer1_value * :- pointer to the element of the timer1_value_lookup_table with the right prescale
  	parameters : struct timer1_value *timer1_value_lookup_table_ptr_for_prescale :- pointer to the timer1_value_lookup_table array
  				 unsigned long int time_bet_steps_for_prescale :- used to hold the time between each step in microseconds
  	Method of operation : it searches the timer1_value_lookup_table array for the suitable prescale and returns a pointer to the member with the suitable prescale
*/
struct timer1_value * stepper_3d::prescale_determination (struct timer1_value *timer1_value_lookup_table_ptr_for_prescale , unsigned long int time_bet_steps_for_prescale)
{
	unsigned char counter;		//just a counter for the loop
	for (counter = 0; counter < 5 ; counter++)	//a loop to search the timer1_value_lookup_table array
	{
		if ( (timer1_value_lookup_table_ptr_for_prescale[counter].max_period_us) > time_bet_steps_for_prescale)	//if a prescale is found 
		{
			break;
		}
	}
	
	return &timer1_value_lookup_table_ptr_for_prescale[counter] ;	//return the address of the suitable array member
}

/*
	Function name : ctc_value_determination
  	return : unsigned int:- used to return the value needed for the OCR1A register in ctc mode
  	parameters : struct timer1_value *timer1_value_lookup_table_ptr_for_prescale :- pointer to the suitable element in the timer1_value_lookup_table array
  				 unsigned long int time_bet_steps_for_ctc :- used to hold the time between each step in microseconds
  	Method of operation : it calculates the value neede to be in the OCR1A register for the isr to work in the right perioo of time
*/
unsigned int stepper_3d::ctc_value_determination (struct timer1_value *timer1_value_lookup_table_ptr_for_ctc , unsigned long int time_bet_steps_for_ctc)
{
	return (time_bet_steps_for_ctc / (timer1_value_lookup_table_ptr_for_ctc-> time_per_tick_us));
}

/*
	Function name : timer1_setup
  	return : void
  	parameters : struct timer1_value *timer1_value_lookup_table_ptr_for_prescale :- pointer to the suitable element in the timer1_value_lookup_table array
  				 unsigned long int time_bet_steps :- used to hold the time between each step in microseconds
  	Method of operation : it sets the registers for timer 1 to the right prescale value and ctc value and enables the timer one ctc interrupt
*/
void stepper_3d::timer1_setup (struct timer1_value *timer1_value_lookup_table_ptr , unsigned long int time_bet_steps)
{
	//temporary varu=iables to hold the values returned from the functions
	struct timer1_value *timer1_value_lookup_table_ptr_temp;
	unsigned int ctc_value;

	timer1_value_lookup_table_ptr_temp = prescale_determination (timer1_value_lookup_table_ptr , time_bet_steps);	//determine the prescale
	ctc_value = ctc_value_determination(timer1_value_lookup_table_ptr_temp , time_bet_steps);	//determine the ctc value
	//registers initialization
	TCCR1A = 0;
  	TCCR1B = 0;
  	TCNT1  = 0;
  	TIMSK1 = 0;

  	OCR1A =  ctc_value;		//set the ctct value in the OCR1A register
  	TCCR1B |= (1 << WGM12);   // CTC mode
  	//prescale setting
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


stepper_3d extruder;
/*********** main and isr functions ***********/

void setup() 
{
	
}

/* the ISR function is the one that does the moving of the stepper motor, it outputs the step at the time required */
ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
	if(stepper_steps > 0)	//if there are any remaining number of steps
	{
		if (direction == NEXT)
		{			
			extruder.stepper_output (&current_state);	//ouput the current state
			extruder.next_step(&current_state);			//point to the next state so that it can be outputed the next call of the isr
		}
		else if (direction == PREVIOUS)
		{
			extruder.stepper_output (&current_state);	//ouput the current state
			extruder.previos_step(&current_state);		//point to the previos state so that it can be outputed the next call of the isr
		}
		stepper_steps--;	//decrease the number of steps reaimed by one as it was just taken
	}
	else
	{
		stepper_steps = 0;	//just for safety
		TIMSK1 = 0;	//disable timer compare interrupt
		TCCR1B &= (~(1 << WGM12));   // disable timer CTC mode
	}
}

void loop() 
{
	extruder.stepper_flow (FORWARD);
	if (digitalRead(7) == HIGH)
	{
		extruder.stepper_stop();
	}
	if (digitalRead(6) == HIGH)
	{
		extruder.stepper_resume ();
	}
	
}



/************** Functions **************/

