#include "tomo.h"
#include <ics1640FunctionalApi.h>

static struct argp_option options[] = {
  {0,         0,    0,              0,  "Operational modes:", 1},
  {"tomo",    't',  0,              0,  "Tomograph mode 32 channels. (default)"},
  {0,         0,    0,              0,  "Options:", 2},
  {"conf",    300,  "arquivo.cfg",  0,  "Indica qual o arquivo de configuracao. Caso nao seja especificado, utilizara o arquivo conf_default.cfg padrao"},
  {"output", 'f',   "FILE_PREFIX",  0,  "Arquivo com os dados coletados. Caso nao seja especificado, grava a saida em /home/eit/frankieData"},
  {0}
};

static int parse_opt( int key, char *arg, struct argp_state *state ) {
  /* Get the input argument from argp_parse, which we know is a pointer to our arguments structure. */
  struct inputArg *myArgs = state->input;

  switch ( key ) {
    case 300:
      myArgs->conf_file = arg;
      break;
    case 'f':
      myArgs->output_file = arg;
      break;
    case 't':
      myArgs->MODE_TOMO = 1;
      myArgs->MODE_COND = myArgs->MODE_OSC = myArgs->MODE_TOMO_EXP = 0;
      break;
    case 'c':
      myArgs->MODE_COND = 1;
      myArgs->MODE_TOMO = myArgs->MODE_OSC = myArgs->MODE_TOMO_EXP = 0;
      break;
    case 'o':
      myArgs->MODE_OSC = 1;
      myArgs->MODE_TOMO = myArgs->MODE_COND = myArgs->MODE_TOMO_EXP = 0;
      break;
    case 'e':
      myArgs->MODE_TOMO_EXP = 1;
      myArgs->MODE_TOMO = myArgs->MODE_COND = myArgs->MODE_OSC = 0;
      break;
  }
  return 0;
}

ICS1640_HANDLE_T  hDevice1 = ICS1640_INVALID_HANDLE_VALUE;
ICS1640_HANDLE_T  hDevice2 = ICS1640_INVALID_HANDLE_VALUE;
ICS1640_HANDLE_T  hDeviceMaster = ICS1640_INVALID_HANDLE_VALUE;
ICS1640_HANDLE_T  hDeviceSlave = ICS1640_INVALID_HANDLE_VALUE;
ICS1640_INT32_T  *ptrBufferMaster = NULL;
ICS1640_INT32_T  *ptrBufferMasterCurr = NULL;
ICS1640_INT32_T  *ptrBufferSlave = NULL;
ICS1640_INT32_T  *ptrBufferSlaveCurr = NULL;

#define NUMBER_OF_ACTIVE_CHANNELS_PER_BOARD F_ICS1640_BOARD_NUMBER_OF_ACTIVE_CHANNELS_16   /* number of electrodes per board we want to use. Multiple of 4!!!! */

#define temp_time 390  /*this sets the sleep time for the usleep function which requires an input in microseconds*/

void save_data_binary( ICS1640_INT32_T *bufferM,
                       ICS1640_INT32_T *bufferS,
                       int n_ChannelsPerBoard,
                       int n_frames,
                       int n_points_per_channel,
                       int n_current_pattern,
                       char *filename );

void save_data_ascii( ICS1640_INT32_T *bufferM,
                      ICS1640_INT32_T *bufferS,
                      int n_ChannelsPerBoard,
                      int n_frames,
                      int n_points_per_channel,
                      int n_current_pattern,
                      char *filename );

void ErrorExit(int error);
volatile int run;

void *ShowTemperature(void* args) {

  ICS1640_TEMPERATURE     temperature;
	do {
		/* Get the environment Temperature on the first board. */
		if ( ICS1640_OK != ics1640EnvironmentTemperatureGet( hDevice1, &temperature ) ) {
			printf( "Temperature: Board 1 = ---   ");
		}
		else {
			printf( "Temperature: Board 1 = %03d   ", temperature.current );
		}

		/* Get the environment Temperature on the second board. */
		if ( ICS1640_OK != ics1640EnvironmentTemperatureGet( hDevice2, &temperature ) ) {
			printf( "Board 2 = ---.\r" );
		}
		else
			printf( "Board 2 = %03d.\r", temperature.current );
	}	while(run);
	pthread_exit(NULL);
}

pthread_t thTemperature;



int main( int argc, char *argv[] ) {
  int i, n_acquired_frames, signal_freq, oversampling = 1, n_curr_pat, flag_multiplexer, n_position_resistor, count_frame, n_frames_curr;
  int flag_acquire_curr, n_frames, n_frames_per_curr, skip_pattern, flag_save_as_ascii, n_poins_per_channel;
  long int frame, currPat, pos_in_buffer;
  double resistor_in, resistor_out;
  char filename_without_extension[M_TEXT_MAX];
  char filename[M_TEXT_MAX], date[M_TEXT_MAX], command[M_TEXT_MAX];
  char answer;


  pthread_attr_t attr;
  /* Initialize and set thread detached attribute */
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


  
  ICS1640_ULONG_T  u32NumBytesToRead, u32NumBytesRead = 0;
  ICS1640_UINT32_T u32Timeout = 10000;
  ICS1640_UINT32_T temp_max = 0;

  ICS1640_CLOCK           clocking1, clocking2;
  ICS1640_TRIGGER         triggerConfig;
  ICS1640_SYNCHRONIZATION synchronizationConfig1, synchronizationConfig2;
  ICS1640_CORES_REVISIONS coresRevision;
  ICS1640_ADC             adcConfiguration;
  ICS1640_TRANSFER        acq;


  FILE *fp;

  struct inputArg input_arguments;
  struct argp argp = { options, parse_opt, "\n--tomo", "\n\n Roda o programa de aquisicao de dados. \n"};


  /* sets this program to have high priority */
  int which = PRIO_PROCESS;
  id_t pid;
  int priority = -15;
  int ret;

  pid = getpid();
  ret = setpriority( which, pid, priority );

  /*----------------- STUFF FOR ARGP -----------------*/

  /* sets default values*/
  input_arguments.conf_file = "./conf_default.cfg";
  // TODO: Avoid absolute hardcode paths
  input_arguments.output_file = "/home/eit/EIT_ICS1640/data/Frankie2Data";
  input_arguments.MODE_TOMO = 1;
  input_arguments.MODE_COND = input_arguments.MODE_OSC = input_arguments.MODE_TOMO_EXP = 0;

  argp_parse( &argp, argc, argv, 0, 0, &input_arguments );

  /* remove file extension if present */
  sprintf( command, "echo %s | sed 's|.bin$||g'", input_arguments.output_file );
  // TODO: Make a c/c++ function
  roda_script_e_pega_result_string( command, filename_without_extension );
  sprintf( command, "echo %s | sed 's|.txt$||g'", filename_without_extension );
  roda_script_e_pega_result_string( command, filename_without_extension );


  /*-----------------  CONFIGURE FRANKIE -----------------*/
  /* Here we set the number of frames and skip pattern information needed to run the code.*/

  flag_multiplexer = 1;   /*read_parameter_int( "FLAG_USE_MUX", input_arguments.conf_file );*/
  flag_acquire_curr =  1; /*read_parameter_int( "FLAG_ACQUIRE_CURR", input_arguments.conf_file );*/
  signal_freq = read_parameter_int( "SIGNAL_FREQ", input_arguments.conf_file );
  skip_pattern =  read_parameter_int( "SKIP_PATTERN", input_arguments.conf_file );
  n_frames = read_parameter_int( "N_FRAMES", input_arguments.conf_file );


  resistor_in  = read_parameter_double( "RESISTOR_IN", input_arguments.conf_file );  /* resistor connected to I+*/
  resistor_out = read_parameter_double( "RESISTOR_OUT", input_arguments.conf_file );  /* resistor connected to I- or GND*/

  flag_save_as_ascii = 0;   /*1: save as ascii   0: save as binary*/
  n_frames_per_curr = 3;   /* for acquiring data for current determination*/
  n_poins_per_channel = 1024;        /* THIS NUMBER MUST BE A MULTIPLE OF 64!!!!!!!!!!!*/
  n_curr_pat = NUMBER_OF_ACTIVE_CHANNELS_PER_BOARD * 2;

    /* check to see if user is ready to begin */
    while ( answer != 'y' && answer != 'n' ) {
      printf( "\nAre you ready to begin? [y,n]:\n" );
      scanf( "%c", &answer );
      if ( answer != 'y' && answer != 'n' )
        printf( "\nInvalid answer! Try again.\n\n" );
    }

    if ( answer == 'y' )
      n_position_resistor = n_curr_pat;
    else
      n_position_resistor = 1;

    n_frames_curr = n_position_resistor * n_frames_per_curr;
 
  

  /*----------------- INIT PARALLEL PORT  -----------------*/
  // TODO: Initialize USB multiplexer connection


  /*----------------- OPEN HANDLES TO THE BOARDS  -----------------*/
  /* Open a handle to the first board. */
  if ( ICS1640_OK != ics1640CreateFileEx( &hDevice1, 0, ICS1640_CREATE_NORMAL ) ) {
    /* ICS1640_CREATE_NORMAL means normal mode of operation, a.k.a., synchronous mode*/
    printf( "ics1640CreateFileEx on the first board failed!\n" );
    ErrorExit(1);
  }
  /* Open a handle to the second board. */
  if ( ICS1640_OK != ics1640CreateFileEx( &hDevice2, 1, ICS1640_CREATE_NORMAL ) ) {  
    printf( "ics1640CreateFileEx on the second board failed!\n" );
    ErrorExit(1);
  }

  /*----------------- RESET THE BOARDS -----------------*/
  /* Reset the first board. */
  if ( ICS1640_OK != ics1640BoardInitialize( hDevice1 ) ) {
    printf( "ics1640BoardInitialize on the first board failed.\n" );
    ErrorExit(1);
  }
  /* Reset the second board. */
  if ( ICS1640_OK != ics1640BoardInitialize( hDevice2 ) ) {
    printf( "ics1640BoardInitialize on the second board failed.\n" );
    ErrorExit(1);
  }
 

  /*----------------- GET FPGA REVISION NUMBER (not essential for us) -----------------*/
  /* Get first board cores Revision */
  memset( &coresRevision, 0, sizeof( ICS1640_CORES_REVISIONS ) );
  if ( ICS1640_ERROR == ics1640CoresRevisionGet( hDevice1, &coresRevision ) ) {
    printf( "ics1640CoresRevisionGet failed. \n" );
    ErrorExit(1);
  }
  printf( "First board control FPGA revision %s.\n", coresRevision.cptrControlFPGARevision );
  /* Get second board cores Revision */
  memset( &coresRevision, 0, sizeof( ICS1640_CORES_REVISIONS ) );
  if ( ICS1640_ERROR == ics1640CoresRevisionGet( hDevice2, &coresRevision ) ) {
    printf( "ics1640CoresRevisionGet failed. \n" );
    ErrorExit(1);
  }
  printf( "Second board control FPGA revision %s.\n", coresRevision.cptrControlFPGARevision );

  /*----------------- CHECK TEMPERATURE (not essential for us right now, but pretty important in the future )-----------------*/
	pthread_create(&thTemperature, &attr, ShowTemperature, NULL);
	pthread_join(thTemperature, NULL);

  /*----------------- FIND MASTER/SLAVES BOARDS -----------------*/
  /* Set Synchronization control on the first board. */
  memset( &synchronizationConfig1, 0, sizeof( ICS1640_SYNCHRONIZATION ) );
  if ( ICS1640_OK != ics1640SynchronizationSet( hDevice1, &synchronizationConfig1 ) ) { 
    /* This function will set synchronizationConfig struct, which says whether it is the master or slave */
    printf( "ics1640SynchronizationSet on the first board failed.\n" );
    ErrorExit(1);
  }
  memset( &synchronizationConfig2, 0, sizeof( ICS1640_SYNCHRONIZATION ) );
  if ( ICS1640_OK != ics1640SynchronizationSet( hDevice2, &synchronizationConfig2 ) ) { 
    /* This function will set synchronizationConfig struct, which says whether it is the master or slave */
    printf( "ics1640SynchronizationSet on the first board failed.\n" );
    ErrorExit(1);
  }

  /* check if it is the Master board */
  if ( ( synchronizationConfig1.u32BoardType == F_ICS1640_BOARD_TYPE_MASTER ) || ( synchronizationConfig2.u32BoardType == F_ICS1640_BOARD_TYPE_STAND_ALONE ) ) {
    hDeviceMaster = hDevice1;
    hDeviceSlave = hDevice2;
    printf("Master 1 Slave 2\n");
  } else {
     if (( synchronizationConfig2.u32BoardType == F_ICS1640_BOARD_TYPE_MASTER ) || ( synchronizationConfig1.u32BoardType == F_ICS1640_BOARD_TYPE_STAND_ALONE )) {
	    hDeviceMaster = hDevice2;
	    hDeviceSlave = hDevice1;
	    printf("Master 2 Slave 1\n");
	  }
	
else
{
    printf( "ics1640 invalid Synchronization architecture (two masters or two slaves).\n" );
    ErrorExit(1);
}
}


  /*----------------- SET AD CONVERTER CLOCKS -----------------*/
  /* Set ADC Clock on the first board. */
  memset( &clocking1, 0, sizeof( ICS1640_CLOCK ) );
  clocking1.dClockFrequency               = 40; /* clock frequency  >20 MHz and < 640 MHz. The sample frequency of the ADC data: >20 MHz and < 640 MHz.
                                                 The largest frequency that the ADCs can handle is 40 MHz, therefore, with a divider value of 16 the
                                                 highest clock frequency is 640 MHz. For most applications these values will be set between 20 and 40 MHz.  */
  clocking1.dReferenceFrequency           = 10.0; /* The reference frequency of the ADC data: >2 MHz and < dClockFrequency. Thetypical value is 10 MHz.  */
  clocking1.u32ClockSelect                = F_ICS1640_INTERNAL;   /* internal clock*/
  clocking1.u32ReferenceSelect            = F_ICS1640_INTERNAL;  /* internal clock*/
  clocking1.u32ClockDividerADC            = F_ICS1640_CLOCK_DIVIDE_BY_1;
  clocking1.u32ClockDividerInternalOutput = F_ICS1640_CLOCK_DIVIDE_BY_1;
  if ( ICS1640_OK != ics1640ClockSet( hDevice1, &clocking1 ) ) {
    printf( "ics1640ClockSet on the first board failed.\n" );
    ErrorExit(1);
  }
  
  /* Set ADC Clock on the second board. */
  memcpy( &clocking2, &clocking1, sizeof( ICS1640_CLOCK ) );
  if ( ICS1640_OK != ics1640ClockSet( hDevice2, &clocking2 ) ) {
    printf( "ics1640ClockSet on the second board failed.\n" );
    ErrorExit(1);
  }

  /*----------------- AD CONVERTER OVERSAMPLING -----------------*/
  memset( &adcConfiguration, 0, sizeof( ICS1640_ADC ) );
  adcConfiguration.u32NumChannels       = NUMBER_OF_ACTIVE_CHANNELS_PER_BOARD;
  adcConfiguration.u32OverSamplingRatio = F_ICS1640_ADC_OVERSSAMPLING_8X;

  /*Power's on and sets oversampling ratio on the first board. */
  if ( ICS1640_OK != ics1640ADCSet( hDevice1, &adcConfiguration ) ) {
    printf( "ics1640ADCSet on the first board failed.\n" );
    ErrorExit(1);
  }
  /*Power's on and sets oversampling ratio on the second board. */
  if ( ICS1640_OK != ics1640ADCSet( hDevice2, &adcConfiguration ) ) {
    printf( "ics1640ADCSet on the second board failed.\n" );
    ErrorExit(1);
  }

  switch ( adcConfiguration.u32OverSamplingRatio ) { /* stores the oversampling for later use*/
    case F_ICS1640_ADC_OVERSSAMPLING_8X:
      oversampling = 8;
      break;
    case F_ICS1640_ADC_OVERSSAMPLING_16X:
      oversampling = 16;
      break;
    case F_ICS1640_ADC_OVERSSAMPLING_32X:
      oversampling = 32;
      break;
    case F_ICS1640_ADC_OVERSSAMPLING_64X:
      oversampling = 64;
      break;
    case F_ICS1640_ADC_OVERSSAMPLING_128X:
      oversampling = 128;
      break;
    case F_ICS1640_ADC_OVERSSAMPLING_256X:
      oversampling = 256;
      break;
  }

  /*----------------- SET TRANSFER -----------------*/
  memset( &acq, 0, sizeof( ICS1640_TRANSFER ) );
  acq.u32SampleCount                    = n_poins_per_channel;
  acq.u32NumChannels                    = NUMBER_OF_ACTIVE_CHANNELS_PER_BOARD;
  acq.u32SampleType                     = F_ICS1640_DATA_TYPE_24_BIT;         /* selects 24bit measurements*/
  acq.u32BufferLengthInPages            = F_ICS1640_BUFFER_LENGTH_DEFAULT;    /* WE might not need to change it, check! specify the size of **ONE SIDE** of the swing buffer.
                                                                              Valid options of this field  are F_ICS1640_BUFFER_LENGTH_DEFAULT or 2 ‐ 4,096. Must be multiple of 2.
                                                                              Each page is 2,048 bytes. Therefore a value of 8 is equal to 8 * 2048 = 16,384 bytes.
                                                                              4096 is equivalent to 8Mbytes (half of the total buffer size)*/
  /* Set the transfer control on the first board. */
  if ( ICS1640_OK != ics1640TransferSet( hDevice1, &acq ) ) {
    ICS1640_OS_DebugPrint( ( "ics1640TransferSet on the first board failed.\n" ) );
    return ICS1640_ERROR;
  }
  /* Set the transfer control on the second board. */
  if ( ICS1640_OK != ics1640TransferSet( hDevice2, &acq ) ) {
    ICS1640_OS_DebugPrint( ( "ics1640TransferSet on the second board failed.\n" ) );
    return ICS1640_ERROR;
  }

  /*----------------- MASTER'S SYNCHRONIZATION TRIGGER -----------------*/

  /* Set Synchronization Trigger control. */
  if ( ICS1640_OK != ics1640SynchronizationTrigger( hDeviceMaster ) ) {
    printf( "ics1640SynchronizationTrigger failed.\n" );
    ErrorExit(1);
  }

  /* Set Trigger control. */
  memset( &triggerConfig, 0, sizeof( ICS1640_TRIGGER ) );
  triggerConfig.u32Select                 = F_ICS1640_INTERNAL;
  triggerConfig.u32ConnectorMode          = F_ICS1640_INTERNAL_TRIGGER_OUTPUT;
  triggerConfig.u32ExternalMode           = F_ICS1640_TRIGGER_MODE_RISING_EDGE;
  triggerConfig.u32ExternalTermination    = F_ICS1640_EXTERNAL_TRIGGER_TERMINATION_ENABLE;
  if ( ICS1640_OK != ics1640TriggerSet( hDeviceMaster, &triggerConfig ) ) {
    printf( "ics1640TriggerSet failed.\n" );
    ErrorExit(1);
  }

  /*----------------- BUFFER ALLOCATION -----------------*/

/* IMPORTANT COMMENT: note that for the frankie collecting phase data we have 2 x the number of current patterns. This is because there is not a separate current*/
/* bin file and currents are collected after each measurement of voltage on the electrode. */
  /* Allocate memory on the master board. */
  u32NumBytesToRead = acq.u32NumChannels * acq.u32SampleCount * sizeof( ICS1640_INT32_T );  /* JAN 14 NOTE: need to x2?? */

  if ( ( ptrBufferMaster = ( ICS1640_INT32_T * )malloc( u32NumBytesToRead * 2*n_curr_pat * n_frames ) ) == NULL ) { 
  /* JAN 14: this works because in the beginning of the program we set ptrBuffer = NULL*/
  /* b/c this statement is only executed when ptrBufferMaster is not allocated */
    printf( "Could not allocate memory for first board.\n" );
    ErrorExit(1);
  }


  /* Allocate memory on the second board. */ 
  if ( ( ptrBufferSlave = ( ICS1640_INT32_T * )malloc( u32NumBytesToRead * 2*n_curr_pat * n_frames ) ) == NULL ) {
    printf( "Could not allocate memory for second board.\n" );
    ErrorExit(1);
  }

  /*----------------- TRANSFER RESET AND ENABLE -----------------*/
  /* Reset the transfer on the first board. */
  if ( ICS1640_OK != ics1640TransferReset( hDevice1 ) ) {
    printf( "ics1640TransferReset on the first board failed.\n" );
    ErrorExit(1);
  }
  /* Reset the transfer on the second board. */
  if ( ICS1640_OK != ics1640TransferReset( hDevice2 ) ) {
    printf( "ics1640TransferReset on the second board failed.\n" );
    ErrorExit(1);
  }

  /* Enable the transfer on the first board. */
  if ( ICS1640_OK != ics1640TransferEnable( hDevice1 ) ) {
    printf( "ics1640TransferEnable on the first board failed.\n" );
    ErrorExit(1);
  }
  /* Enable the transfer on the second board. */
  if ( ICS1640_OK != ics1640TransferEnable( hDevice2 ) ) {
    printf( "ics1640TransferEnable on the second board failed.\n" );
    ErrorExit(1);
  }

  /* ---------------  DATA ACQUISITION ---------*/
  printf( "Acquiring %d frame(s)...\nHit any key to save the data and exit...\n", n_frames );

  // TODO: Change to functions using ftime.h
  /* ADDED JAN 15 to try to determine time */
  char start_time[M_TEXT_MAX];
  char end_time[M_TEXT_MAX];
  sprintf( command, "date +%%d_%%b_%%Y_%%H:%%M:%%S:%%N" ); /*%%b_%%d_%%Y_%%H:%%M:%%S*/
  roda_script_e_pega_result_string( command, start_time );
  printf( "The start time (Hour:Min:Sec:Nanosec) is %s. \n", start_time );
  long int inc_buffer = ( u32NumBytesToRead) / sizeof( ICS1640_INT32_T ); /* this intializes step size for incrementing the buffer*/
  /* printf( " inc buffer is %ld \n", inc_buffer); */ /* note: this value should be 16384 for 1024 samples for each V_elect and V_curr */
  long int count_buffer = 0;

	run = 1;
	pthread_create(&thTemperature, &attr, ShowTemperature, NULL);

  for ( frame = 1; !kbhit() && frame <= n_frames; frame++ ) {
    /*printf( "frame n. %ld\n", frame );*/

    for ( currPat = 1; currPat <= n_curr_pat; currPat++ ) {
      if ( flag_multiplexer == 0 ) {
        printf( "\nselect the %ldth current pattern and press any key...\n", currPat ); /* wait for the current pattern*/
        waitKeyboard();
      }
      else {
        /* multiplexer mumbo jumbo*/
        // @TODO: MCU USB command
        //SetCurrPatternSkip( skip_pattern, currPat );
        usleep( temp_time ); /* wait a little  */
        /*        printf("pat=%d\n",currPat);*/
        /*waitKeyboard();*/
      }
 
      /*---- Here we will measure the voltage at the electrode. EN_MS = 1. we define ENMS == LP_PIN0 -----*/
      /* set EN_MS to high in the SetCurrPatternSkip function which is found in parallel_port.c*/

      if ( ICS1640_OK != ics1640TriggerTrigger( hDeviceMaster ) ) {  /* TRIGGER! */
        printf( "ics1640TriggerTrigger failed.\n" );
        ErrorExit(1);
      }

			if ( ICS1640_OK != ics1640InterruptWait( hDeviceMaster, ICS1640_SWING_BUFFER_1640_SWAP_EV, &u32Timeout ) ) {
				printf( "Interrupt Wait on the master board failed.\n" );
				ErrorExit(1);
			}
	
     /* read data from the master board first. */
		 pos_in_buffer = inc_buffer * count_buffer;
     /* ( ( frame - 1 ) * u32NumBytesToRead * (n_curr_pat) + (2*( currPat - 1 )) * u32NumBytesToRead ) / sizeof( ICS1640_INT32_T ); *//* select the position in the buffer for the current frame*/
     /* printf( " pos in buffer is %ld \n", pos_in_buffer); */ /* Michelle added trouble shooting print out. uncomment code to see buffer locations */
      if ( ICS1640_OK != ics1640ReadFile( hDeviceMaster, 0, &( ptrBufferMaster[pos_in_buffer] ), u32NumBytesToRead, &u32NumBytesRead, u32Timeout ) ) {
        printf( "DMA on the master board failed (V_elect meas).\n" );
        ErrorExit(1);
      }
      /* read data from the slave board. */
      if ( ICS1640_OK != ics1640ReadFile( hDeviceSlave, 0, &( ptrBufferSlave[pos_in_buffer] ), u32NumBytesToRead, &u32NumBytesRead, u32Timeout ) ) {
        printf( "DMA on the slave board failed (V_elect meas).\n" );
        ErrorExit(1);
      }
      //fflush( stdout );
      
      count_buffer = count_buffer + 1;

      /*----- Here we will measure the voltage related to the current. EN_MS = 0 -----*/
      /* PLEASE NOTE: in parallel_port.c we define ENMS == LP_PIN04 */
          
      if ( ICS1640_OK != ics1640TriggerTrigger( hDeviceMaster ) ) {  /* TRIGGER! */
        printf( "ics1640TriggerTrigger failed.\n" );
        ErrorExit(1);
      }
      
      //clear_pin( LP_PIN04 ); /* set EN_MS to low to activate switch on the active electrodes*/
      /* Michelle comment (jan 24, 2014): note the ADG442 chips are normally open. So, EN_MS must be hight to measure the voltage on the electrodes*/

			if ( ICS1640_OK != ics1640InterruptWait( hDeviceMaster, ICS1640_SWING_BUFFER_1640_SWAP_EV, &u32Timeout ) ) {
				printf( "Interrupt Wait on the master board failed.\n" );
				ErrorExit(1);
			}

      /* read data from the master board first. */
      pos_in_buffer = inc_buffer*count_buffer;
 /*( ( frame - 1 ) * u32NumBytesToRead * (2*n_curr_pat) + (2*( currPat - 1 ) +1) * u32NumBytesToRead ) / sizeof( ICS1640_INT32_T );*/ /* select the position in the buffer for the current frame*/
     /* printf( " pos in buffer is %ld \n", pos_in_buffer);  */ /* Michelle added trouble shooting print out. uncomment code to see buffer locations */     
if ( ICS1640_OK != ics1640ReadFile( hDeviceMaster, 0, &( ptrBufferMaster[pos_in_buffer] ), u32NumBytesToRead, &u32NumBytesRead, u32Timeout ) ) {
        printf( "DMA on the master board failed (V_curr meas).\n" );
        ErrorExit(1);
      }
      /* read data from the slave board. */
      if ( ICS1640_OK != ics1640ReadFile( hDeviceSlave, 0, &( ptrBufferSlave[pos_in_buffer] ), u32NumBytesToRead, &u32NumBytesRead, u32Timeout ) ) {
        printf( "DMA on the slave board failed V_curr meas).\n" );
        ErrorExit(1);
      }
      //fflush( stdout );
     
      count_buffer = count_buffer + 1;

    }
  }
      
  /* ADDED JAN 15 to try to determine time */     
  sprintf( command, "\ndate +%%d_%%b_%%Y_%%H:%%M:%%S:%%N" ); /*%%b_%%d_%%Y_%%H:%%M:%%S*/
  roda_script_e_pega_result_string( command, end_time );
  printf( "\nThe end time (Hour:Min:Sec:Nanosec) is %s. \n", end_time );


  n_acquired_frames = frame - 1;

  /* ---------------  SAVING DATA  ---------*/

  printf( "\nSaving data now. It might take some time...\n" );

  /* Write to file */
  sprintf( command, "date +%%Y_%%m%%b_%%d_%%H:%%M:%%S" );
  roda_script_e_pega_result_string( command, date );


    sprintf( filename, "%s_%s.bin", filename_without_extension, date );
    save_data_binary( ptrBufferMaster,
                      ptrBufferSlave,
                      NUMBER_OF_ACTIVE_CHANNELS_PER_BOARD,
                      n_acquired_frames,
                      n_poins_per_channel,
                      (2*n_curr_pat),
                      filename );
  
  

    /* ---------------  SAVING DATA  ---------*/
  sprintf( filename, "%s_%s.info", filename_without_extension, date );
  fp = fopen( filename, "w" );

  fprintf( fp, "N_FRAMES=%d                 %% number of frames\n", n_acquired_frames );
  fprintf( fp, "N_CHANNELS=%d               %% number of electrodes\n", NUMBER_OF_ACTIVE_CHANNELS_PER_BOARD * 2 );
  fprintf( fp, "N_CURR_PAT=%d               %% number of current patterns per frame\n", n_curr_pat );
  fprintf( fp, "N_POINTS_PER_CHANNEL=%d     %% number of points per channel\n", n_poins_per_channel );
  fprintf( fp, "SKIP_PATTERN=%d                 %% skip pattern\n", skip_pattern );
  fprintf( fp, "SIGNAL_FREQ=%d         %% signal generator frequency. Value in Hz\n", signal_freq );
  fprintf( fp, "SAMPLING_FREQ=%.2lf        %% sampling frequency. Value in Hz\n", ( 1.0e6 * clocking1.dClockFrequency / 2.0 ) / oversampling );
  fprintf( fp, "MEAS_RANGE=%1.3lf          %% measurement range.Value in Volts\n", 20.0 );
  if ( acq.u32SampleType == F_ICS1640_DATA_TYPE_24_BIT )
    fprintf( fp, "AD_RESOLUTION=%d            %% number of bits of precision\n", 24 );
  else
    fprintf( fp, "AD_RESOLUTION=%d            %% number of bits of precision\n", 16 );
  fprintf( fp, "FLAG_ASCII=%d            %% 1: save data as text file 0: save data as binary file\n", flag_save_as_ascii );
  if ( flag_acquire_curr == 1 ){
    
  }

  fclose( fp );
  chmod(filename,S_IRGRP|S_IWGRP|S_IRUSR|S_IWUSR|S_IROTH|S_IWOTH);  /* read and write permition to everyone*/

	ErrorExit(0);
	return 0;
}


void ErrorExit(int error) {

	run = 0;
	pthread_join(thTemperature, NULL);
	printf("\n");

  /* Disable the first board. */
  if ( ICS1640_OK != ics1640TransferDisable( hDevice1 ) ) {
    printf( "ics1640TransferDisable on the first board failed.\n" );
  }
  /* Disable the second board. */
  if ( ICS1640_OK != ics1640TransferDisable( hDevice2 ) ) {
    printf( "ics1640TransferDisable on the second board failed.\n" );
  }

  /* Free memory on the first board. */
  if ( ptrBufferMaster != NULL ) {
    free( ptrBufferMaster );
    ptrBufferMaster = NULL;
  }

  /* Free memory on the second board. */ /*not sure why we don't have it for ptrBuffer2??? perhaps becuase it is the slave board?? added on JAN 14*/
  if ( ptrBufferSlave != NULL ) {
    free( ptrBufferSlave );
    ptrBufferSlave = NULL;
  }
  /* Close board handle on the first board. */
  if ( ICS1640_INVALID_HANDLE_VALUE != hDevice1 ) {
    ics1640CloseFileEx( hDevice1 );
  }
  /* Close board handle on the second board. */
  if ( ICS1640_INVALID_HANDLE_VALUE != hDevice2 ) {
    ics1640CloseFileEx( hDevice2 );
  }
	
	if (error)
	  exit(error);
}

/* Function to write data to file in ascii format************************/
void save_data_ascii( ICS1640_INT32_T *bufferM,
                      ICS1640_INT32_T *bufferS,
                      int n_ChannelsPerBoard,
                      int n_frames,
                      int n_points_per_channel,
                      int n_current_pattern,
                      char *filename ) {
  FILE *fd;
  int frame, currPat, channel;
  long int n_measurements_per_currPat, n_measurements_per_frame, point, pos;

  if ( NULL == ( fd = fopen( filename, "w" ) ) ) {
    printf( "Cannot open file %s.\n", filename );
    return;
  }

  n_measurements_per_currPat = n_points_per_channel * n_ChannelsPerBoard;   /*WARNING: values in samples, not in bytes*/
  n_measurements_per_frame = n_current_pattern * n_measurements_per_currPat;   /*WARNING: values in samples, not in bytes*/

  for ( frame = 1; frame <= n_frames; frame++ ) {
    for ( currPat = 1; currPat <= n_current_pattern; currPat++ ) {
      for ( point = 1; point <= n_points_per_channel; point++ ) {
        for ( channel = 1; channel <= n_ChannelsPerBoard; channel++ ) {
          pos = ( frame - 1 ) * n_measurements_per_frame + ( currPat - 1 ) * n_measurements_per_currPat + ( point - 1 ) * n_ChannelsPerBoard + channel - 1;
          fprintf( fd, "%d ", bufferM[ pos ] );
        }
        for ( channel = 1; channel <= n_ChannelsPerBoard; channel++ ) {
          pos = ( frame - 1 ) * n_measurements_per_frame + ( currPat - 1 ) * n_measurements_per_currPat + ( point - 1 ) * n_ChannelsPerBoard + channel - 1;
          fprintf( fd, "%d ", bufferS[ pos ] );
        }
      }
    }
    fprintf( fd, "\n" ); /*each line contains one complete frame.*/
  }

  fclose( fd );
  chmod(filename,S_IRGRP|S_IWGRP|S_IRUSR|S_IWUSR|S_IROTH|S_IWOTH);  /* read and write permition to everyone*/

  printf( "Wrote %d frames\n", n_frames );
}

/* Function to write data to file in binary format************************/
void save_data_binary( ICS1640_INT32_T *bufferM,
                       ICS1640_INT32_T *bufferS,
                       int n_ChannelsPerBoard,
                       int n_frames,
                       int n_points_per_channel,
                       int n_current_pattern,
                       char *filename ) {
  FILE *fd;
  int frame, currPat;
  long int n_measurements_per_currPat, n_measurements_per_frame, point, pos;

  if ( NULL == ( fd = fopen( filename, "wb" ) ) ) {
    printf( "Cannot open file %s.\n", filename );
    return;
  }

  n_measurements_per_currPat = n_points_per_channel * n_ChannelsPerBoard;
  n_measurements_per_frame = n_current_pattern * n_measurements_per_currPat;

  for ( frame = 1; frame <= n_frames; frame++ ) {
    for ( currPat = 1; currPat <= n_current_pattern; currPat++ ) {
      for ( point = 1; point <= n_points_per_channel; point++ ) {
        pos = ( frame - 1 ) * n_measurements_per_frame + ( currPat - 1 ) * n_measurements_per_currPat + ( point - 1 ) * n_ChannelsPerBoard;
        fwrite( &bufferM[pos], sizeof( ICS1640_INT32_T ), n_ChannelsPerBoard, fd );
        fwrite( &bufferS[pos], sizeof( ICS1640_INT32_T ), n_ChannelsPerBoard, fd );
      }
    }
  }
  fclose( fd );
  chmod(filename,S_IRGRP|S_IWGRP|S_IRUSR|S_IWUSR|S_IROTH|S_IWOTH);  /* read and write permition to everyone*/

  printf( "Wrote %d frames\n", n_frames );
}

