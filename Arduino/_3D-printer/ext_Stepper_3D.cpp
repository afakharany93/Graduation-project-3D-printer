#include "ext_Stepper_3D.h"
#include "Arduino.h"


ext_stepper_3d::ext_stepper_3d()
{
	current_state = stepper_states[0];
	//setting pin modes
	pinMode(first, OUTPUT);
	pinMode(second, OUTPUT);
	pinMode(third, OUTPUT);
	pinMode(forth, OUTPUT);
	//setting all pins as low
	digitalWrite(first, LOW);
	digitalWrite(second, LOW);
	digitalWrite(third, LOW);
	digitalWrite(forth, LOW);

	
	interrupts();             // enable all interrupts
}

//stepper functions :
/*Function name : stepper_output
  return : void
  parameters : struct ext_stepper_state_struct *current_state :- pointer to struct, used for call by refrence for the variable containing the information of the current state
  Method of operation : to interpert and output the out member of the ext_stepper_state_struct variable holding currrent state information
 */
void ext_stepper_3d::stepper_output (struct ext_stepper_state_struct *current_state)
{
	digitalWrite(first  , ((current_state->out) & 0x01));		/*if bit one in the out member of the ext_stepper_state_struct variable holding currrent state information is one
															then the pin mapped to first will be high, if not it will be zero	*/				
	digitalWrite(second , ((current_state->out) & 0x02));		//same as the line above but with bit 2 and pin mapped to second
	digitalWrite(third  , ((current_state->out) & 0x04));		//same as the line above but with bit 3 and pin mapped to third
	digitalWrite(forth  , ((current_state->out) & 0x08));		//same as the line above but with bit 4 and pin mapped to forth
}

/*
	Function name : next_step
  	return : void
  	parameters : struct ext_stepper_state_struct *current_state :- pointer to struct, used for call by refrence for the variable containing the information of the current state
  	Method of operation : to change the current state variable to the variable pointed at with the pointer member nxt in the ext_stepper_state_struct, i.e. to put the current 
  						  state in next state and take the next step, used for going forwards
*/
void ext_stepper_3d::next_step (struct ext_stepper_state_struct *current_state)
{
	*current_state = *current_state->nxt;
}

/*
	Function name : previos_step
  	return : void
  	parameters : struct ext_stepper_state_struct *current_state :- pointer to struct, used for call by refrence for the variable containing the information of the current state
  	Method of operation : to change the current state variable to the variable pointed at with the pointer member prev in the ext_stepper_state_struct, i.e. to put the current 
  						  state in previos state and take the previos step, used for going backwards
*/
void ext_stepper_3d::previos_step (struct ext_stepper_state_struct *current_state)
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
void ext_stepper_3d::stepper_move (long int steps, unsigned long int time_bet_steps_stepper )
{
	if (permission == 1)	//if you have permission, execute
	{
		permission = 0;		//permission is used for one time only, this line expires it, to execute again a new permission must be granted
		status_var = MOVE;	//setting status to currently moving
		if (steps > 0)
		{
			direction = clockwise;
			stepper_steps = steps;
		}
		else if (steps < 0)
		{
			direction = anticlockwise;
			stepper_steps = steps * (-1);
		}
		else if(steps == 0)
		{
			stepper_steps = 0;	//for safety and anti user stupidity
		}
		timer5_setup ( timer5_value_lookup_table , time_bet_steps_stepper);		//make the timer do its magic
	}
}

/*
	Function name : stepper_stop
  	return : void
  	parameters : none
  	Method of operation : it stops - or pauses- the motion of the stepper motor, it actually stops the timer interrupt since the timer ISR is the function that does the motion 
*/
void ext_stepper_3d::stepper_stop ()
{
	TCCR5B &= (~(1 << WGM52));   // disable timer CTC mode
	TIMSK5 = 0 ;  // disable timer compare interrupt
	status_var = SW_FORCE_STOP;	//setting status to indicate the stop due to software command
}

/*
	Function name : stepper_resume
  	return : void
  	parameters : none
  	Method of operation : it is used to resume the motion after stopping
*/
void ext_stepper_3d::stepper_resume ()
{
	TCCR5B |= (1 << WGM52);   // CTC mode
	TIMSK5 |= (1 << OCIE5A);  // enable timer compare interrupt
	status_var = RESUME_AF_STOP; //set status to indicate that the steper is resuming 
}

/*
	Function name : stepper_flow
  	return : void
  	parameters : unsigned char direction_flow :- to determine the direction of motion
  	Method of operation : it is used to move the stepper motor in a very big nummber of steps that it seems it moves for infinityyyyy, but it doesn't, 
  						  best used for moving the stepper motor till a physical event happens to stop it
*/
void ext_stepper_3d::stepper_flow (unsigned char direction_flow)
{
	if (direction_flow == clockwise )
	{
		stepper_move (2147483647, 164933 );	//move max number of steps in a direction
	}
	else if (direction_flow == anticlockwise )
	{
		stepper_move (-2147483647, 164933 );	//move max number of steps oin the other direction
	}
	status_var = FLOW;

}



//timer functions:

/*
	Function name : prescale_determination
  	return : struct timer5_value * :- pointer to the element of the timer5_value_lookup_table with the right prescale
  	parameters : struct timer5_value *timer5_value_lookup_table_ptr_for_prescale :- pointer to the timer5_value_lookup_table array
  				 unsigned long int time_bet_steps_for_prescale :- used to hold the time between each step in microseconds
  	Method of operation : it searches the timer5_value_lookup_table array for the suitable prescale and returns a pointer to the member with the suitable prescale
*/
struct timer5_value * ext_stepper_3d::prescale_determination (struct timer5_value *timer5_value_lookup_table_ptr_for_prescale , unsigned long int time_bet_steps_for_prescale)
{
	unsigned char counter;		//just a counter for the loop
	for (counter = 0; counter < 5 ; counter++)	//a loop to search the timer5_value_lookup_table array
	{
		if ( (timer5_value_lookup_table_ptr_for_prescale[counter].max_period_us) > time_bet_steps_for_prescale)	//if a prescale is found 
		{
			break;
		}
	}
	
	return &timer5_value_lookup_table_ptr_for_prescale[counter] ;	//return the address of the suitable array member
}

/*
	Function name : ctc_value_determination
  	return : unsigned int:- used to return the value needed for the OCR5A register in ctc mode
  	parameters : struct timer5_value *timer5_value_lookup_table_ptr_for_prescale :- pointer to the suitable element in the timer5_value_lookup_table array
  				 unsigned long int time_bet_steps_for_ctc :- used to hold the time between each step in microseconds
  	Method of operation : it calculates the value neede to be in the OCR5A register for the isr to work in the right perioo of time
*/
unsigned int ext_stepper_3d::ctc_value_determination (struct timer5_value *timer5_value_lookup_table_ptr_for_ctc , unsigned long int time_bet_steps_for_ctc)
{
	return (time_bet_steps_for_ctc / (timer5_value_lookup_table_ptr_for_ctc-> time_per_tick_us));
}

/*
	Function name : timer5_setup
  	return : void
  	parameters : struct timer5_value *timer5_value_lookup_table_ptr_for_prescale :- pointer to the suitable element in the timer5_value_lookup_table array
  				 unsigned long int time_bet_steps :- used to hold the time between each step in microseconds
  	Method of operation : it sets the registers for timer 1 to the right prescale value and ctc value and enables the timer one ctc interrupt
*/
void ext_stepper_3d::timer5_setup (struct timer5_value *timer5_value_lookup_table_ptr , unsigned long int time_bet_steps)
{
	//temporary varu=iables to hold the values returned from the functions
	struct timer5_value *timer5_value_lookup_table_ptr_temp;
	unsigned int ctc_value;

	timer5_value_lookup_table_ptr_temp = prescale_determination (timer5_value_lookup_table_ptr , time_bet_steps);	//determine the prescale
	ctc_value = ctc_value_determination(timer5_value_lookup_table_ptr_temp , time_bet_steps);	//determine the ctc value
	//registers initialization
	TCCR5A = 0;
  	TCCR5B = 0;
  	TCNT5  = 0;
  	TIMSK5 = 0;

  	OCR5A =  ctc_value;		//set the ctct value in the OCR5A register
  	TCCR5B |= (1 << WGM52);   // CTC mode
  	//prescale setting
  	if ( (timer5_value_lookup_table_ptr_temp->prescale) == 1 )
  	{
  	 TCCR5B |= (1 << CS50);   
	}
	else if ( (timer5_value_lookup_table_ptr_temp->prescale) == 8 )
  	{
  	 TCCR5B |= (1 << CS51);   
	}
	else if ( (timer5_value_lookup_table_ptr_temp->prescale) == 64 )
  	{
  	 TCCR5B |= (1 << CS50);   
  	 TCCR5B |= (1 << CS51);   
	}
	else if ( (timer5_value_lookup_table_ptr_temp->prescale) == 256 )
  	{
  	 TCCR5B |= (1 << CS52);   
	}
	else if ( (timer5_value_lookup_table_ptr_temp->prescale) == 1024 )
  	{
  	 TCCR5B |= (1 << CS50);   
  	 TCCR5B |= (1 << CS52);   
	}

	TIMSK5 |= (1 << OCIE5A);  // enable timer compare interrupt
}

/*
	Function name : inside_ISR
  	return : void
  	parameters :void
  	Functionality : this function is to be called inside the timer ISR function, it the function resposible for doing the motion everytime the ISR runs
*/
void ext_stepper_3d::inside_ISR () 
{
	if(stepper_steps > 0)	//if there are any remaining number of steps
	{
		if (direction == NEXT)
		{			
			stepper_output (&current_state);	//ouput the current state
			next_step(&current_state);			//point to the next state so that it can be outputed the next call of the isr
		}
		else if (direction == PREVIOUS)
		{
			stepper_output (&current_state);	//ouput the current state
			previos_step(&current_state);		//point to the previos state so that it can be outputed the next call of the isr
		}
		stepper_steps--;	//decrease the number of steps reaimed by one as it was just taken
	}
	else
	{
		stepper_steps = 0;	//just for safety
		TIMSK5 = 0;	//disable timer compare interrupt
		TCCR5B &= (~(1 << WGM52));   // disable timer CTC mode
		status_var = END_MOVE;	//setting status to indicate that the motion ended
	}
}

/*
	Function name : change_rotation_direction
	return : void
	parameters :void
	Functionality : if the motor rotates in the other direction than the one specified - given to a function - in all times and all calls, just use 
	this function to correct the rotation direction
*/
void ext_stepper_3d::change_rotation_direction_mapping()
{
	clockwise     ^= 0x01;
	anticlockwise ^= 0x01;
}

/*
	Function name : change_rotation_direction
	return : void
	parameters :void
	Functionality : if the motor when connected to a linear actuator moves in the other direction than the one specified - given to a function - in all times and all calls, 
	just use this function to correct the motion direction
*/

void ext_stepper_3d::change_linear_direction_mapping()
{
	forward  ^= 0x01;
	backward ^= 0x01;
}

char * ext_stepper_3d::stepper_status()
{
	int x = 0;	//to hold the return value of sprintf
	long int steps;
	if (direction == clockwise)
		{
			steps = stepper_steps;
		}
	else if (direction == anticlockwise)
		{
			steps =  stepper_steps* (-1);
		}
	char buff[120];
	x = sprintf(buff, "Status %d, t_bet_steps %lu, remain_steps %ld",status_var , time_bet_steps_us, steps);
	return (char *) buff;
}

