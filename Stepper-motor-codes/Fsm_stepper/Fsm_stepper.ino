/*if it works it was done by Ahmed El Fakharany to test the finite state machine concept with stepper motors for the 3d printer graduation project, 
if it doesn't workm I don't know who the hell did it but I'm sure it is not Ahmed El Fakharany :D */

#define DELAY_uS 	800		//amount to delay in us
#define DELAY_mS	1
//mapping the pins with wire colors
#define BLACK 		9
#define BLUE 		11
#define RED 		10
#define GREEN 		8
//mapping wire colors with firing order
#define FIRST 		GREEN
#define SECOND   	RED
#define THIRD   	BLACK 
#define FORTH  		BLUE

/*struct stepper_state_struct is a struct used to hold the info concerning the states, each state resemnles one step,
 it holds the output of the state and a pointer to the next state to use to step forward and
 a pointer to the previous step in case of backwards */

typedef struct stepper_state_struct
{
	unsigned char out;						//used to hold the output values to all pins, this value needs to be inturpeted later
  	struct stepper_state_struct	*nxt;		//used to hold a pointer to the next state which resmbles the next step on the stepper motor, used for forward motion
	struct stepper_state_struct	*prev;		//pointer used to hold the address to the previos state, used for backwards motion
} stepper_state_struct;

/*stepper_states is an array that holds the constant values of all the states of the stepper motor */
struct stepper_state_struct stepper_states[4] = 
{
  //{out  , next state         , previos state      }
	{0x01 , &stepper_states[1] , &stepper_states[3] } ,
	{0x02 , &stepper_states[2] , &stepper_states[0] } ,
	{0x04 , &stepper_states[3] , &stepper_states[1] } ,
	{0x08 , &stepper_states[0] , &stepper_states[2] }
};


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
}

void loop() 
{
	struct stepper_state_struct current_state = stepper_states[0];		//the variable that will hold the current state information, initialized with state zero info
	struct stepper_state_struct *current_state_ptr = &current_state;	//pointer to the variable holding curent state info

	for (int i = 0; i<200 ; i++)
	{
		stepper_output (current_state_ptr);
		next_step (current_state_ptr);
		delay(DELAY_mS);
	}
	delay(200);
	for (int i = 0; i<200 ; i++)
	{
		stepper_output (current_state_ptr);
		previos_step (current_state_ptr);
		delay(DELAY_mS);
	}
	delay(200);

}


/************** Functions **************/
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

