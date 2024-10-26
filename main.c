#include <sys/sys.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define inf 32766

short int map[15][13];
short int closed[15][13];
short int fclickx,fclicky,cursorx,cursory;
int MAP_WIDTH;
int MAP_HEIGHT;
int gen=0;
int bombs_cnt;
int initbombs_cnt=1;

void writelowb(){               //function to paint the row with NO numbers   
    printf("+---+");
    for(int j=0;j<MAP_WIDTH;j++) 
        printf("+----");
    printf("+\n");
    return;
}

void startscreen(){
    FILE* kbd=fopen("/dev/kbd", "");
    char key;
    printf("\033[93m");
    printf("\033[10;5H __  __ _____ _   _ ______  _______          ________ ______ _____  ______ _____\n");
    printf("\033[10;6H|  \\/  |_   _| \\ | |  ____|/ ____\\ \\        / /  ____|  ____|  __ \\|  ____|  __ \\\n");
    printf("\033[10;7H| \\  / | | | |  \\| | |__  | (___  \\ \\  /\\  / /| |__  | |__  | |__) | |__  | |__) |\n");
    printf("\033[10;8H| |\\/| | | | | . ` |  __|  \\___ \\  \\ \\/  \\/ / |  __| |  __| |  ___/|  __| |  _  /\n");
    printf("\033[10;9H| |  | |_| |_| |\\  | |____ ____) |  \\  /\\  /  | |____| |____| |    | |____| | \\ \\ \n");
    printf("\033[10;10H|_|  |_|_____|_| \\_|______|_____/    \\/  \\/   |______|______|_|    |______|_|  \\_\\\n");
    
    printf("\033[97m");
    printf("\033[9;15H        _ _ _  ");
    printf("\033[9;16H       | | | | ");
    printf("\033[9;17H       | | | | ");
    printf("\033[9;18H       |__/_/    ");
    printf("\033[9;19H    ___  ___  ___ ");
    printf("\033[9;20H   | . |/ __>| . \\");
    printf("\033[9;21H   |   |\\__ \\| | |");
    printf("\033[9;22H   |_|_|<___/|___/");
    printf("\033[9;24H\033[97mTO MOVE CURSOR USE \033[93mWASD");


    printf("\033[38;15H          \033[97m..");
    printf("\033[38;16H          \033[97m||\033[31m|###\\");
    printf("\033[38;17H          \033[97m||\033[31m|####|");
    printf("\033[38;18H          \033[97m||\033[31m|###/");
    printf("\033[38;19H          \033[97m||");
    printf("\033[38;20H          ||");
    printf("\033[38;21H          ||");
    printf("\033[38;22H         //\\\\");
    printf("\033[38;24HTO MARK FIELD PRESS \033[93mSPACE");


    printf("\033[67;16H            \033[97m|");
    printf("\033[67;17H         \033[97m\\ \033[37m/#\\ \033[97m/");
    printf("\033[67;18H         \033[37m/#####\\");
    printf("\033[67;19H      \033[97m-<\033[37m|#######|\033[97m>-");
    printf("\033[67;20H         \033[37m\\#####/   ");
    printf("\033[67;21H         \033[97m/ \033[37m\\#/ \033[97m\\");
    printf("\033[67;22H            \033[97m|");
    printf("\033[67;24HTO OPEN A CELL PRESS \033[93mENTER ");

    printf("\033[42;30H\033[97mCHOOSE MAP SIZE:");

    printf("\033[32m");
    printf("\033[11;33H.--------------.");
    printf("\033[11;34H|    _______   |");
    printf("\033[11;35H|   /  ___  |  |");
    printf("\033[11;36H|  |  (__ \\_|  |");
    printf("\033[11;37H|   '.___`-.   |");
    printf("\033[11;38H|  |`\\____) |  |");
    printf("\033[11;39H|  |_______.'  |");
    printf("\033[11;40H|              |");
    printf("\033[11;41H'--------------'");
    printf("\033[11;42H   SMALL: 7x9");
    printf("\033[11;43H   (PRESS S)");

    printf("\033[35m");
    printf("\033[42;33H.--------------.");
    printf("\033[42;34H| ____    ____ |");
    printf("\033[42;35H||_   \\  /   _||");
    printf("\033[42;36H|  |   \\/   |  |");
    printf("\033[42;37H|  | |\\  /| |  |");
    printf("\033[42;38H| _| |_\\/_| |_ |");
    printf("\033[42;39H||_____||_____||");
    printf("\033[42;40H|              |");
    printf("\033[42;41H'--------------'");
    printf("\033[42;42H  MEDIUM:10x12");
    printf("\033[42;43H   (PRESS M)");

    printf("\033[31m");
    printf("\033[71;33H.--------------.");
    printf("\033[71;34H|   _____      |");
    printf("\033[71;35H|  |_   _|     |");
    printf("\033[71;36H|    | |       |");
    printf("\033[71;37H|    | |   _   |");
    printf("\033[71;38H|   _| |__/ |  |");
    printf("\033[71;39H|  |________|  |");
    printf("\033[71;40H|              |");
    printf("\033[71;41H'--------------'");
    printf("\033[71;42H  LARGE: 13x15");
    printf("\033[71;43H   (PRESS L)");

    fflush(stdout);
    while(1){
        fread(&key, 1, 1, kbd);
        switch(key){
            case 'S':
            case 's':
                MAP_HEIGHT=9;
                MAP_WIDTH=7;
                initbombs_cnt=10;
                return;
            case 'M':
            case 'm':
                MAP_HEIGHT=13;
                MAP_WIDTH=11;
                initbombs_cnt=25;
                return;
            case 'L':
            case 'l':
                MAP_HEIGHT=15;
                MAP_WIDTH=13;
                initbombs_cnt=40;
                return;
        }
    }

}

void screenclear(){
    printf("\033[0;0H");
    for(int i=0;i<44;i++){
            printf("\033[d                                                                                                             ");
    }
    printf("\033[77;53H                                  ");
}

void writeline(int line){      //function to paint the row with numbers
    for(int j=0;j<MAP_WIDTH;j++)
        printf("|    ");
    printf("|\n"); 
}

void uprow(){                //function to paint the zero row
    printf("\033[93m");
    writelowb();
    printf("|-_-|");
    for(int j=0;j<MAP_WIDTH;j++){
        printf("| \033[97m");
        if(j<9) printf("0");
        printf("%d\033[93m ",j+1);
    }
    printf("|\n"); 

    printf("+===+");
    for(int j=0;j<MAP_WIDTH;j++) 
        printf("+====");
    printf("+\n");
}

void scr_init(){             //init score board
    bombs_cnt=initbombs_cnt;
    printf("\033[95m\033[%d;2H",MAP_WIDTH*5+1);
    printf("\033[%d;2H+-------------------------------+",MAP_WIDTH*5+7);
    printf("\033[%d;3H|                               |",MAP_WIDTH*5+7); 
    printf("\033[%d;4H|                               |",MAP_WIDTH*5+7);           
    if(bombs_cnt>=10 )printf("\033[95m\033[%d;5H|        BOMBS LEFT: %d         |",MAP_WIDTH*5+7,bombs_cnt);
    else printf("\033[95m\033[%d;5H|        BOMBS LEFT: 0%d         |",MAP_WIDTH*5+7,bombs_cnt);
    printf("\033[%d;6H|                               |",MAP_WIDTH*5+7);
    printf("\033[%d;7H|                               |",MAP_WIDTH*5+7);
    printf("\033[%d;8H|                               |",MAP_WIDTH*5+7);
    printf("\033[%d;9H+-------------------------------+",MAP_WIDTH*5+7);
    fflush(stdout);
}

void initmapbase(){           //whole first initialistion of graphics base(it won't be changed) 

    printf("\033[0;0H");
    uprow();
    for(int i=0;i<MAP_HEIGHT;i++){
        printf("| \033[97m%c\033[93m |",'A'+i);
        writeline(i);
        writelowb();
    }

    for(int i=0;i<MAP_HEIGHT;i++)
        for(int j=0;j<MAP_WIDTH;j++)
            printf("\033[32m\033[%d;%dH[]",7+j*5,3+i*2);
    scr_init();
    printf("\033[77;53H \033[97m*PRESS \033[93mR \033[97mTO RETURN TO MENU");
    fflush(stdout);
}

int mod(unsigned int val, int base){   //modulo
    unsigned int foo=val/base;
    return val-(foo*base);
}

int check(int posx,int posy){       //checks if momb can be placed here
    if(map[posy][posx]==inf) return 0;
    for(int i=-1;i<2;i++)
        for(int j=-1;j<2;j++) 
            if(posx+i==fclickx && posy+j==fclicky) return 0;
    return 1;
}

void mapgen(){             //initial map generation
    srand(mod(rand()*MAP_WIDTH*MAP_HEIGHT*initbombs_cnt,RAND_MAX)); //set depending on time 
    for(int i=0;i<MAP_HEIGHT;i++)
        for(int j=0;j<MAP_WIDTH;j++){
            map[i][j]=0;
            closed[i][j]=1;
        }
            
    int posx=0,posy=0;  //0-x 1-y
    for(int bombs=0;bombs<initbombs_cnt;){
        posx=mod(rand(),MAP_WIDTH);
        posy=mod(rand(),MAP_HEIGHT);
        if(check(posx,posy)){
            map[posy][posx]=inf;
            bombs++;
        }
        //bombs++;
    }
}

void mapcalc(){                       //calculates numbers regarding how much bombs are there near a cell
    for(int i=0;i<MAP_HEIGHT;i++)
        for(int j=0;j<MAP_WIDTH;j++)
            if(map[i][j]!=inf)
                for(int k=-1;k<2;k++)
                    for(int l=-1;l<2;l++) 
                        if(i+k>=0 && i+k<MAP_HEIGHT && j+l>=0 && j+l<MAP_WIDTH && map[i+k][j+l]==inf) map[i][j]++;       
}   

void cursor_update(char c){       //updates visual and true cursor position
    int oldx=cursorx;int oldy=cursory;
    switch(c){
        case 'W':
        case 'w' : 
            if(cursory!=0) cursory--;
            break;
        case 'S':
        case 's' : 
            if(cursory+1!=MAP_HEIGHT) cursory++;
            break;
        case 'A':
        case 'a' : 
            if(cursorx!=0) cursorx--;
            break;
        case 'D':
        case 'd' : 
            if(cursorx+1!=MAP_WIDTH) cursorx++;
            break;
    }
    if(oldy) printf("\033[93m\033[%d;%dH+----+\033[%d;%dH|\033[4C|\033[%d;%dH+----+",5+oldx*5,2+oldy*2,5+oldx*5,3+oldy*2,5+oldx*5,4+oldy*2);
    else printf("\033[93m\033[%d;%dH+====+\033[%d;%dH|\033[4C|\033[%d;%dH+----+",5+oldx*5,2+oldy*2,5+oldx*5,3+oldy*2,5+oldx*5,4+oldy*2);
    fflush(stdout);
    if(cursory) printf("\033[35m\033[%d;%dH+----+\033[%d;%dH|\033[4C|\033[%d;%dH+----+",5+cursorx*5,2+cursory*2,5+cursorx*5,3+cursory*2,5+cursorx*5,4+cursory*2);
    else printf("\033[35m\033[%d;%dH+====+\033[%d;%dH|\033[4C|\033[%d;%dH+----+",5+cursorx*5,2+cursory*2,5+cursorx*5,3+cursory*2,5+cursorx*5,4+cursory*2);
    fflush(stdout);
}

void open_cells(short int clickx,short int clicky,int d){   //opens cells graphically and in code
    closed[clicky][clickx]=0;
    if(map[clicky][clickx])printf("\033[96m\033[%d;%dH0%d",7+clickx*5,3+clicky*2,map[clicky][clickx]);
    else printf("\033[96m\033[%d;%dH  ",7+clickx*5,3+clicky*2);
    if(!map[clicky][clickx]) d=0;
    if(d) return;
    for(int k=-1;k<2;k++)
        for(int l=-1;l<2;l++)
            if(clicky+k>=0 && clicky+k<MAP_HEIGHT && clickx+l>=0 && clickx+l<MAP_WIDTH 
                && map[clicky+k][clickx+l]!=inf && closed[clicky+k][clickx+l]==1){
                    if(map[clicky+k][clickx+l]==0) open_cells(clickx+l,clicky+k,d);
                    else open_cells(clickx+l,clicky+k,d+1); 
                }
    fflush(stdout);               
}

void endgame(){   //nanomachines son
    printf("\033[91m");
    printf("\033[%d;3H ____                        _ ",MAP_WIDTH*5+8); 
    printf("\033[%d;4H|  _ \\                      | |",MAP_WIDTH*5+8);           
    printf("\033[%d;5H| |_) | ___   ___  _ __ ___ | |",MAP_WIDTH*5+8);
    printf("\033[%d;6H|  _ < / _ \\ / _ \\| '_ ` _ \\| |",MAP_WIDTH*5+8);
    printf("\033[%d;7H| |_) | (_) | (_) | | | | | |_|",MAP_WIDTH*5+8);
    printf("\033[%d;8H|____/ \\___/ \\___/|_| |_| |_(_)",MAP_WIDTH*5+8);
    fflush(stdout);
    for(int i=0;i<MAP_HEIGHT;i++)
        for(int j=0;j<MAP_WIDTH;j++)
            if(map[i][j]==inf) printf("\033[91m\033[%d;%dH**",7+j*5,3+i*2);
    fflush(stdout);
    gen=0;
    initmapbase();
    cursor_update(' ');
}

void wincheck(){   //Are you winning son?
    int cnt=0;
    for(int i=0;i<MAP_HEIGHT;i++)
        for(int j=0;j<MAP_WIDTH;j++)
            if(map[i][j]==inf && closed[i][j]==2) cnt++;
    if(cnt==initbombs_cnt){      //Yesd Dad I do.
        printf("\033[93m");
        for(int i=0;i<3;i++){
            for(int j=3;j<9;j++)
                printf("\033[%d;%dH             YOU WON!          ",MAP_WIDTH*5+8,j);
            fflush(stdout);
            for(int j=3;j<9;j++)
                printf("\033[%d;%dH                               ",MAP_WIDTH*5+8,j);
            fflush(stdout);
        }
        gen=0;
        initmapbase();
        cursor_update(' ');
    }
    
    return;
}
short int start=0; 

int main() {

    FILE* kbd = fopen("/dev/kbd", "");
    char key;
    startscreen();
    screenclear();
    
    initmapbase();
    cursor_update(' ');

    while(1){
        if(bombs_cnt==0) wincheck();
        fread(&key, 1, 1, kbd);
        cursor_update(key);
        if(key=='r' || key=='R'){
            gen=0;
            screenclear();
            startscreen();
            screenclear();
            initmapbase();
            cursor_update(' ');
        }
        if(key=='\n'){                      //press ENTER(open cell)
            if(!gen){
                fclickx=cursorx;fclicky=cursory;
                bombs_cnt=initbombs_cnt;  
                mapgen(); 
                mapcalc();
                gen=1;
            }

            if(map[cursory][cursorx]>0 && map[cursory][cursorx]!=inf) open_cells(cursorx,cursory,1);
            else if(map[cursory][cursorx]!=inf) open_cells(cursorx,cursory,0);

            if(map[cursory][cursorx]==inf) endgame();
            fflush(stdout);
             
        }
        else if(key==' '){                  //pressed SPACE(place flag)
            if(!gen){
                fclickx=cursorx;fclicky=cursory;
                mapgen();   
                mapcalc();
                gen=1;
            }
            if(closed[cursory][cursorx]==1 && bombs_cnt){
                closed[cursory][cursorx]=2;
                printf("\033[%d;%dH\033[97m|\033[91m>",7+cursorx*5,3+cursory*2);
                bombs_cnt--;
                if(bombs_cnt>=10 )printf("\033[95m\033[%d;5H|        BOMBS LEFT: %d         |",MAP_WIDTH*5+7,bombs_cnt);
                else printf("\033[95m\033[%d;5H|        BOMBS LEFT: 0%d         |",MAP_WIDTH*5+7,bombs_cnt);
            }
            else if(closed[cursory][cursorx]==2){
                closed[cursory][cursorx]=1;
                printf("\033[%d;%dH\033[32m[]",7+cursorx*5,3+cursory*2);
                bombs_cnt++;
                if(bombs_cnt>=10 )printf("\033[95m\033[%d;5H|        BOMBS LEFT: %d         |",MAP_WIDTH*5+7,bombs_cnt);
                else printf("\033[95m\033[%d;5H|        BOMBS LEFT: 0%d         |",MAP_WIDTH*5+7,bombs_cnt);
            }
            fflush(stdout);
        }
    } 
    return 0;
} 
