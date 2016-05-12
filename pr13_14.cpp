#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>
#define MAX_FORTIO 900
#define MAX_ROBOTS 20
#define PROB_NEW_ROBOT 15
#define PROB_NEW_ANAL 10
#define PROB_NEW_EXP 5
#define SPACE_X 30
#define SPACE_Y 30
#define ROBOT_OK  true
#define ROBOT_NOK false
#define MAX_Ir 1000
#define MAX_Pt 1000
#define MAX_Pd 1000
#define TIME_DELAY 1000


using namespace std;

/*bohthtikes synarthseis*/
//tyxaioi arithmoi
int get_rand(int min, int max) {
	return (rand() % (max - min)) + min;
}

//synarthsh xronokathisterishs pou metra se millisecond ton xrono
void sleep_for(int msec) {
	Sleep(msec);
}
// Sinartisi pou dexetai enan xarakthra apo ton xrhsth kai ton epistrefei wste na diaxeiristei apo to parapanw block if-else
int get_keyboard_character() {
    int t;
    if (kbhit()) {
       t = getch();
       while(kbhit()) {
          getch();
       }
       return t;
    }
    else
        return 0;
}



/*--------------------------- class robot -----------------------*/
class robot
{
	
	public:
		void get_thesi(int*,int*);		//mas dinei thn trexousa thesi tou robot
		char get_type();				//epistrefei ton typo tou robot
	//	bool in_base();
		bool running_status(){return status;}; //epistrefei thn katastasi
		void repair(){status = true;};	//isws na mhn xreiastei
		void corruption(){status = false;};
		virtual void move()=0;					//kinei tyxaia ta robot
		void move(int, int);			//kinei ta robot stoxeumena kapou
		virtual void action()=0;			//h virual funvtion leitourgias
		virtual void print_robot()=0;
		void robot_play();				//bohthitikh synarthsh gia thn planet::play
		
		robot();						//o constructor
		
		
	protected:
		bool status;					//katastasi oximatos
		int velocity;					//taxythta kinhshs
		char type;						//typos robot
		int base_x, base_y;				//thesi bashs
		int time_down;					//metrhths pou metra poso ena robot einai se blabh	
		int thesi_x;					//Syntetagmenes thesis robot
		int thesi_y;
		int moves;						//kiniseis
		int blabes;						//blabes							
};

robot::robot(){
	status = ROBOT_OK;					//arxikopoioumai thn katastasi ws energh(true)
	thesi_x = get_rand(0,SPACE_X);	//tyxaia arxikopoihsh thesis
	thesi_y = get_rand(0,SPACE_Y);	//tyxaia arxikopoihsh thesis
	type = '?';						//type etoimo na anagnwristei apo tous allous constructors
	moves=0;
	blabes=0;
	velocity=5;
}
//apothikeysh ths thesis tou robot
void robot::get_thesi(int* x, int*y){
	*x=thesi_x;
	*y=thesi_y;
}
//epistrefei to eidos tou robot
char robot::get_type(){
	return type;
}





/*-------------------- class Planet ----------------*/

class planet{

	protected:
		struct edafos{
			int amount_of_Pd,amount_of_Ir,amount_of_Pt;//poso yliko se kathe keli
			int total_amount;//sunoliko poso ylikou
			bool robot_here;//yparksi robot
			bool danger_flag;//shmaia kindynou
			float access_dangers;//pososto epikyndhnothtas
		};
		edafos kosmos[SPACE_X][SPACE_Y];//Pinakas apo structs opou struct to kathe pedio eksoriksis
		robot *robots[MAX_ROBOTS]; //ta kelia periexoun anafores sta robot
		int base_x, base_y;			//syntetagmenes bashs
		int base_Ir,base_Pt,base_Pd; //trexousa posotita apothematwn
	public:
		void set_base(int, int);		//orizei thn bash gia kathe eksomoivsh
		void has_robot(int,int);	//yparxei robot sto shmeio
		void add_robot(void);	//dhmioyrgia robot apo ton xrhsth
		void get_robot_and_change_status(void);	//o xrhsths mporei na tropopoihsei thn status tou robot
		void change_a_point(void);//o xrshts mporei na tropopoihsei ena shmeio
		void point_info(void);//o xrhsths lambanei plhrofories gia to shmeio
		void new_rescuing_robot(void);//dhmiourgia robot apo to systhma
		void print_planet(void);//ektypwsh kosmou
		void play(void);
		bool base_full(void);//einai gemath h bash
		bool has_flag(int,int);
		planet();
		~planet();
};
	
	
planet::planet(){
	int x,y,bx,by;
	for (x=0; x<SPACE_X; x++){
    	for (y=0; y<SPACE_Y; y++){
	 		kosmos[x][y].amount_of_Pd=get_rand(0,300);
	 		kosmos[x][y].amount_of_Ir=get_rand(0,300);
	 		kosmos[x][y].amount_of_Pt=get_rand(0,300);
	 		kosmos[x][y].total_amount+=(kosmos[x][y].amount_of_Pd + kosmos[x][y].amount_of_Ir + kosmos[x][y].amount_of_Pt);
	 		kosmos[x][y].danger_flag=false;
	 		kosmos[x][y].access_dangers=get_rand(1,9);
	 		//cout<<kosmos[x][y].access_dangers;
    	}	
   }
   bx = get_rand(0, SPACE_X);
   by = get_rand(0, SPACE_Y);
   set_base(bx,by);
   for(int i = 0; i < MAX_ROBOTS; i++){
		robots[i] = 0;
   }
    cout << "Space inited!\n";    
}

planet::~planet(){
   	for(int i=0;i <MAX_ROBOTS; i++){
   		if (robots[i]){
   			delete robots[i];
   			robots[i]=0;
   		}
   	}
}




/*---------------------- class Exploring ---------------------*/

class exploring: public robot, public planet {
	public:
		exploring();
		void action();
		void print_robot();
		void move();
	protected:
		int num_of_flags;
		float access_cap;				//ikanothta prosbashs		
};

exploring::exploring(){
	type = 'E';
	//set_base(base_x, base_y);
	num_of_flags=0;
	access_cap=get_rand(7,10);				//ikanothta prosbashs
}

//ylopoihsh ths virtual synarthshs print gia ta exploring robots
void exploring::print_robot(){
cout<<"\n";	
cout<< type<<" "<<status<<"X|Y:"<<thesi_x<<"|"<<thesi_y <<"\tVel"<< velocity;
cout<<"\tO Arithmos twn flags pou exoun topothetithei einai:"<<num_of_flags;
}


//ylopoihsh ths virtual synarthshs move gia ta exploring robots
void exploring::move(){
	int new_x, new_y;

	new_x = get_rand(thesi_x - velocity, thesi_x + velocity);//tyxaia kinisi tou oximatos ston aksona x
	new_y = get_rand(thesi_y - velocity, thesi_y + velocity);//tyxaia kinisi tou oximatos ston aksona y
//elegxos oriwn gia thn kinisi
	if (new_x < 0)
		new_x = 0;
	if (new_y < 0)
		new_y = 0;
	if (new_x >= SPACE_X)
		new_x = SPACE_X - 1;
	if (new_y >= SPACE_Y)
		new_y = SPACE_Y - 1;
		
	thesi_x = new_x;
	thesi_y = new_y;	
}


void exploring::action(){
	move();
	if (kosmos[thesi_x][thesi_y].access_dangers>6){
		kosmos[thesi_x][thesi_y].danger_flag=true;
		num_of_flags++;
	}
	move();
}
/*--------------------- class Analyzing  --------------------*/

class analyzing: public robot,public planet{
	public:
		analyzing();
		void action();
		void print_robot();
		void move();
	protected:
		int max_fortio;			//sunoliko max fortio
		int max_Pd;				//max fortio paladiou
		int max_Ir;				//max fortio iridiou
		int max_Pt;				//max fortio leukoxrysou
		int fortio_Pd;			//fortio paladiou
		int fortio_Ir;			//fortio iridiou
		int fortio_Pt;			//fortio leukoxrysou
		int fortio;				//sunoliko fortio
		float access_cap;				//ikanothta prosbashs
			
};

analyzing::analyzing(){
	type = 'A';
	//set_base(base_x, base_y);
	max_fortio=MAX_FORTIO;
	int max_Pd = max_fortio/3;
	int max_Ir = max_fortio/3;
	int max_Pt = max_fortio/3;
	int fortio_Pd = 0;
	int fortio_Ir = 0;
	int fortio_Pt = 0;
	int fortio = 0;
	float access_cap=get_rand(1,10);				//ikanothta prosbashs
}

//ylopoihsh ths virtual synarthshs print gia ta analyzing robots
void analyzing::print_robot(){
cout<<"\n";	
cout<< type<<" "<<status<<"X|Y:"<<thesi_x<<"|"<<thesi_y <<"\tVel"<< velocity;
cout<<"M_F"<<max_fortio<<"\tF_Pd"<<fortio_Pd<<"\tF_I"<<fortio_Ir<<"\tF_Pt"<<fortio_Pt<<"\tF"<<fortio;	
}

//ylopoihsh ths virtual synarthshs move gia ta analyzing robots
void analyzing::move(){
	int new_x,new_y;
	float corr_prop;
	new_x = get_rand(thesi_x - velocity, thesi_x + velocity);//tyxaia kinisi tou oximatos ston aksona x
	new_y = get_rand(thesi_y - velocity, thesi_y + velocity);//tyxaia kinisi tou oximatos ston aksona y
//elegxos oriwn gia thn kinisi
	if (new_x < 0)
		new_x = 0;
	if (new_y < 0)
		new_y = 0;
	if (new_x >= SPACE_X)
		new_x = SPACE_X - 1;
	if (new_y >= SPACE_Y)
		new_y = SPACE_Y - 1;
	if(!kosmos[new_x][new_y].danger_flag){
		thesi_x = new_x;
		thesi_y = new_y;	
	}
	//yparxei pithanotita ena oxima na mpei se blabi kata thn kinhsh tou	
	corr_prop=kosmos[new_x][new_y].access_dangers *(1-access_cap);
	if(corr_prop>8){
		corruption();
	}
		
}


void analyzing::action(){
	move();
	if(kosmos[thesi_x][thesi_y].total_amount>450){
		fortio_Pd=+kosmos[thesi_x][thesi_y].amount_of_Pd;
		fortio_Ir=+kosmos[thesi_x][thesi_y].amount_of_Ir;
		fortio_Pt=+kosmos[thesi_x][thesi_y].amount_of_Pt;
	}
	if(fortio==MAX_FORTIO){
		base_Pd+=fortio_Pd;
		fortio_Pd=0;
		base_Ir+=fortio_Ir;
		fortio_Ir=0;
		base_Pt+=fortio_Pt;
		fortio_Pt=0;
	}
}



/*---------------- class Rescuing ---------------------*/

class rescuing: public robot,public planet{
	public:
		rescuing();
		void action();
		void print_robot();
		void move();
	protected:
		int rescues;
		float access_cap;	
};

rescuing::rescuing(){
	type = 'R';
	//set_base(base_x, base_y);
	rescues=0;
	float access_cap=get_rand(7,10);
}


//ylopoihsh ths virtual synarthshs move gia ta rescuing robots
void rescuing::move(){
	int new_x,new_y;
	float corr_prop;

	new_x = get_rand(thesi_x - velocity, thesi_x + velocity);//tyxaia kinisi tou oximatos ston aksona x
	new_y = get_rand(thesi_y - velocity, thesi_y + velocity);//tyxaia kinisi tou oximatos ston aksona y
//elegxos oriwn gia thn kinisi
	if (new_x < 0)
		new_x = 0;
	if (new_y < 0)
		new_y = 0;
	if (new_x >= SPACE_X)
		new_x = SPACE_X - 1;
	if (new_y >= SPACE_Y)
		new_y = SPACE_Y - 1;
	if(kosmos[new_x][new_y].danger_flag==false){
		thesi_x = new_x;
		thesi_y = new_y;	
	}
	//yparxei pithanothta na pathei blabei to robot kata thn kinhsh tou	
	corr_prop=kosmos[new_x][new_y].access_dangers *(1-access_cap);
	if(corr_prop>5){
		corruption();
	}	
		
	
}

//h action twn diaswstikwn oximatwn
void rescuing::action(){
	move();			
	has_robot(thesi_x,thesi_y);
	if(kosmos[thesi_x][thesi_y].robot_here){
		int i;
		int x,y;
		for(i=0;i<MAX_ROBOTS;i++){
			robots[i]->get_thesi(&x,&y);
			if(x==thesi_x && y==thesi_y){
				robots[i]->repair();
				rescues++;
			}
		}
	}	
	move();
}
//ylopoihsh ths virtual synarthshs print gia ta rescuing robots
void rescuing::print_robot(){
cout<<"\n";	
cout<< type<<" "<<status<<"X|Y:"<<thesi_x<<"|"<<thesi_y <<"\tVel"<< velocity;
cout<<"Oi mexri twra diaswseis einai:"<<rescues;
}


//synarthsh poy thetei tis syntetagmenes bashs
void planet:: set_base(int x, int y){
	base_x = x;
	base_y = y;
}
//eksetazoume thn uparksi robot sto shmeio
void planet::has_robot(int x,int y){
	int tx,ty;
	for(int i=0;i<MAX_ROBOTS;i++){
		robots[i]->get_thesi(&tx,&ty);
		if (tx==x && ty==y){
			kosmos[x][y].robot_here=true;
		}
	}		
}
//synarthsh pou mas deixnei an exoume kapou shmaia
bool planet::has_flag(int x,int y){
	if(kosmos[x][y].danger_flag==true){
		return true;
	}
	else
		return false;				
}
//syanrthsh play
void planet::play(){
     int i;
     for(i = 0; i < MAX_ROBOTS; i++)
         if (robots[i]) {
            robots[i]->action();//kaleitai h synarthsh action() gia kathe oxima
        }
		 else{
		 	cout<<"\nproblem";
		 }                                                             
}
//pithani dhmiourgia robot 
void planet::new_rescuing_robot(void){
	for(int i=0;i<3;i++){
		robots[i]=new rescuing;
	}
}
//rhth dimiourgia robot
void planet::add_robot(void){
	int new_thesi = -1,i,temp;
	for(i = 0; i < MAX_ROBOTS; i++) {
         if (robots[i] == 0) {
            new_thesi = i;
            break;
        }
	}
 if (new_thesi == -1)
      return;
  
      
 temp=get_rand(0,100);
 //cout<<temp;     
 if (temp<PROB_NEW_EXP ){
     robots[i] = new exploring();
     
   }
   else if(PROB_NEW_EXP<temp<=PROB_NEW_ANAL){
   		robots[i]= new analyzing();
		
   }
   
   else {
      robots[i] = new rescuing();
   }      
}


/*	briskoume to robot kai mporoume na allaksoume thn katastash tou */
void planet::get_robot_and_change_status(){
    int a;

    cout << "Dwse kwdiko oximatos metaksi [0, " << MAX_ROBOTS << "):";
    cin >> a;
    if (a < 0 || a >= MAX_ROBOTS || !robots[a]) {
        cout << "Den iparxei oxima me ton sigkekrimeno kwdiko!\n";
        return;
    } 
    else {
    	char choice;
    	cin>>choice;
    	if (choice=='f'){
    		robots[a]->corruption();
    	}
        else{
        	robots[a]->repair();	
        }
    }
}

//printaroume ena shmeio
void planet::point_info(){
	int x,y,tx,ty;
	cout<<"Dwse syntetagmenes me X < "<< SPACE_X <<" kai Y < "<< SPACE_Y <<"\n";
	cin>>x>>y;
	if(x >= SPACE_X || y >= SPACE_Y || x < 0 || y < 0) {
           cout << "Oi sintetagmenes pou dwstae einai ektos oriwn\n";
           return;
    }
    
    if(kosmos[x][y].total_amount>0){
    	cout<<"To shmeio periexei"<<kosmos[x][y].amount_of_Pd<<"Paladio\t"<<kosmos[x][y].amount_of_Ir<<"iridio\t "<<kosmos[x][y].amount_of_Pt<<"leykoxrysos\t"<<"\n";
    }
    
	if (x == base_x && y == base_y){
         cout << "To shmeio pou epileksate einai h bash\n";
    }
    
    if(kosmos[x][y].danger_flag==true){
    	cout << "To shmeio exei shmaia kyndinou\n";
    }
    for(int i = 0; i < MAX_ROBOTS; i++){
    	 if (robots[i]) {
                robots[i]->get_thesi(&tx, &ty);
                if (tx == x && ty == y) {
                  cout << i << "\t";
                  robots[i]->print_robot();
                }
           }	
    }
}


//mas epistrefei an einai gemath h bash
bool planet::base_full(){
	if(base_Ir==MAX_Ir && base_Pt==MAX_Pt && base_Pd==MAX_Pd)
		return true;
}


//ektypwnei sthn kosnola ton kosmo
void planet::print_planet(void) {
	int x,y, nx, ny;
    int i;
    char robot;

    system ("cls");
 	for( x=0;x<SPACE_X;x++){
  		for(y=0;y<SPACE_Y;y++){
			robot = 0;
	  		for(i = 0; i < MAX_ROBOTS; i++) {
	       		if (robots[i]) {
	          		robots[i]->get_thesi(&nx, &ny);
	          		if (nx == x && ny == y) {
						robot = robots[i]->get_type();
						break;
					}

	      	 	}
			}

   			if (robot) {
   				cout <<robot;
    		}
    		
   			else if (kosmos[x][y].total_amount>0){
   				cout <<" ";
   			}
   			else if(kosmos[x][y].total_amount=0){
   				cout << "-";
   			}
   				
   			
			if (x == base_x && y == base_y) {
    			cout <<"B";	 	
   			}
   			
   			
  			 
  		}

  	cout << "| ";
  	switch(x) {
    case 0:
        cout <<"5343: Xaris Mallios";
        break;
    case 1:
        cout <<"5228: Lefteris Mantas";
        break;
    case 2:
        cout <<"5431: Dimitris Stafidas";
        break;
    case 4:
        cout << "MENU:";
        break;
    case 5:
        cout << "E: Exit";
        break;
    case 6:
        cout << "C: Create robot";
        break;
    case 7:
        cout << "F: Destroy robot";
        break;
    case 8:
        cout << "S: Print statistics";
        break;
    case 9:
        cout << "P: Pause";
        break;
        break;
    case 10:
        cout << "Base:" << base_x << "-" << base_y;
        break;
    case 11:
        cout << "Press choice";
        break;
    }
  cout << "\n";
 }

  for(i = 0; i < MAX_ROBOTS; i++)
       if (robots[i]) {
          cout << i << "\t";
          robots[i]->print_robot();
       }
}
planet *mars;


/*******************************************************************/
int main() {
	srand(time(NULL));
	int kb;
    mars = new planet();
	if (!mars) {
       cout << "world is broken!\n";
       system("pause");
       return 0;
    }	
    cout<<"Mars is up!";
    //mars->new_rescuing_robot();
    while(mars->base_full()==false){
    	for(int j=0;j<MAX_ROBOTS;j++){
    	mars->add_robot();
    }
    mars->play();
    mars->print_planet();
    kb = get_keyboard_character();
    if (kb == 'E' || kb == 'e') {
    	break;//to programma termatizetai
    }
    else if (kb == 'C' || kb == 'c') {
    	mars->add_robot();
		cout << "A new robot just created just for you!\n";
    }
    else if (kb == 'F' || kb == 'f') {
    	mars->get_robot_and_change_status();
        cout << "A robot destroyed!\n";
    }
    else if (kb == 'P' || kb == 'p') {
        cout << "System paused!\n";
        system("pause");
    }
	else if (kb == 'S' || kb == 's') {
        mars->point_info();
        system("pause");
    }
    sleep_for(TIME_DELAY);
    }
    
    cout<<"\ncheckpoint";
    delete mars;
    system("pause");
    }
    


/*To do list
* Mia play ston planet h opoia tha kalei thn action twn robot kai thn add_robot(). 
Ta oximata bgenoun ola mazi opote lew na ta arxikopoioume
se ena perithwrio xE[10,35] kai meta na kanoume mia loopa dhmiourgias gia x robot.
* Mia print_planet() ston planet gia na blepoume ti kanoume 
* Ena menu epikoinwnias
* beyond_repair();

*/


