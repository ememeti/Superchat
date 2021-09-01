#include "interface.hpp"
#include <cstdio>
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cstring>

Interface::Interface() {
    // Seed so we get a random sequence of values
    srand ( time(NULL) );

    // Stores the strings to be outputted by the file    
    title.push_back("       _         __     __      ________        _________    ________         ___          __     __              _           _______________");
    title.push_back("     _____       __     __      __      __      ________     __     __      __   __        __     __             _ _           _____________ ");
    title.push_back("   ___   ___     __     __      __       __     __           __     __    ___     ___      __     __            _ _ _               ___      ");
    title.push_back("   ___           __     __      __       __     __           __    __     ___              __     __           __   __              ___      ");
    title.push_back("     ___         __     __      __      __      _____        __  _        ___              _________          __ ___ __             ___      ");
    title.push_back("       ___       __     __      __   __         _____        __  __       ___              _________         __ _____ __            ___      ");
    title.push_back("         ___     __     __      __   __         __           __   __      ___              __     __        __         __           ___      ");
    title.push_back("   ___   ___     __     __      __   __         __           __    __     ___     ___      __     __       __           __          ___      ");
    title.push_back("     _____        __   __       __   __         _______      __     __      __   __        __     __      __             __         ___      ");
    title.push_back("       _            ___         _______         _________    __      __       ___          __     __     ___             ___        ___      ");
    title.push_back("                                                                                                                                             ");
    title.push_back("																		                                                                      ");
    title.push_back("                               By: Phillip Gamblin, Gaurav Ganthapodi, Misbah Gilani, Endrit Memeti, Jorgi Rajan                             ");

    // Displays the first three choices the user can pick
    start_choice.push_back("[ Login ]");
    start_choice.push_back("[ Create New Account ]");
    start_choice.push_back("[ Delete Account ]");

    // Displays the next three choices to be used
    login.push_back("[   Enter   ]");
    login.push_back("[   Refresh   ]");
    login.push_back("[   Exit   ]");
	


    // Stores all the available chatroom names
    // At the initial start up, the user only has access to the Lobby
    choices.push_back("Lobby");

    // Represents the ASCII value for 0
    int frst_input = 48;
    // Holds the values for the ASCII values from 0 to 9
    for(;frst_input < 57;frst_input++)
        inputs.push_back(frst_input);

    // Represents the chatroom options that the user can undergo
    options.push_back("Join");      // Join a chatroom
    options.push_back("Create");    // Create a chatroom
    options.push_back("Leave");     // Leave a chatroom
    options.push_back("Delete");    // Delete a chatroom
    options.push_back("Log Out");

    n_title = title.size();
    n_login = login.size();
    n_start = start_choice.size();
	n_options = options.size();

    cur_room = 0;

    chatrooms.push_back(all_messages); 
}

char* Interface::get_ip() {
    return ip_addr;
}

char* Interface::get_username() {
    return username;
}

char* Interface::get_password() {
    return password;
}


void Interface::add_message(char * string, int length)
{
	std::string str(string, length);
	all_messages.push_back(str);
  n_messages++;
}



void Interface::login_highlight(WINDOW *inp_win, int highlight, int cur_y, int cur_x) {
    int i = 0;

    for(; i < n_login; i++)
	{	
		char cstr[login.at(i).size() + 1];
		strcpy(cstr, login.at(i).c_str());
		if(highlight == i + 1) { // Highlight the present choice
		    wattron(inp_win, A_REVERSE);
		    mvwprintw(inp_win,cur_y, cur_x, "%s", cstr);
	        wattroff(inp_win, A_REVERSE);
		} else
		    mvwprintw(inp_win, cur_y, cur_x, "%s", cstr);
		cur_x+=22;
	}
	wrefresh(inp_win);
}

void Interface::start_highlight(WINDOW *inp_win, int highlight, int cur_y) {
    int i = 0;

    int cur_x = 44;
    for(; i < n_start; i++)
	{	
		char cstr[start_choice.at(i).size() + 1];
		strcpy(cstr, start_choice.at(i).c_str());
		if(highlight == i + 1) { // Highlight the present choice
		    wattron(inp_win, A_REVERSE);
		    mvwprintw(inp_win,cur_y, cur_x, "%s", cstr);
	        wattroff(inp_win, A_REVERSE);
		} else
		    mvwprintw(inp_win, cur_y, cur_x, "%s", cstr);
		cur_x+=start_choice.at(i).size()+5;
	}
	wrefresh(inp_win);
}


void Interface::print_title(WINDOW*start, int index, char* part) {
    int num_color = ( rand() % 7 ) + 1;
    int j = 0;

    for(; j < strlen(part); j++ ) {
        wattron(start, COLOR_PAIR(num_color));
        mvwprintw( start, index, 1, part );
        wattroff(start, COLOR_PAIR(num_color));
        doupdate();
        usleep( 400 );
        wrefresh( start );
    }
}

int Interface::login_choice( WINDOW* inwin, int y, int hl) {
    int c;
    int next_step;
    int old_highlight = hl;
    
    // The user can see the cursor again
    curs_set(1);

    if( old_highlight == 1 ) {
        mvwprintw( inwin, y-52, 58, "  Username:" );
        mvwgetnstr( inwin, y-52, 71, username, 8 );
        mvwprintw( inwin, y-49, 58, "  Password:" );
        mvwgetnstr( inwin, y-49, 71, password, 8 );
        mvwprintw( inwin, y-46, 58, "IP Address:" );
        mvwgetnstr( inwin, y-46, 71, ip_addr, 15 );
	    char cstr[login.at(0).size() + 1];
	    strcpy(cstr, login.at(0).c_str());
        mvwprintw( inwin, y-40, 41, cstr );
	    char cstr1[login.at(1).size() + 1];
	    strcpy(cstr1, login.at(1).c_str());
        mvwprintw( inwin, y-40, 63, cstr1 );
	    char cstr2[login.at(2).size() + 1];
	    strcpy(cstr2, login.at(2).c_str());
        mvwprintw( inwin, y-40, 85, cstr2 );
        
        // Cursor is now invisible
        curs_set(0);

        while(1) {
            c = wgetch(inwin);
	        switch(c){
                case KEY_LEFT:
		            if(hl == 1)
		                hl = n_login;
		            else
		                --hl;
		            break;
		        case KEY_RIGHT:
		            if(hl == n_login)
			            hl = 1;
		            else
			            ++hl;
		            break;
                case 10:
                    break;
                default:
		            mvwprintw(inwin, y-30, 1 , "Character pressed is = %3d Hopefully it can be printed as '%c'", c, c);
		            wrefresh(inwin);
		            break;
            }
            if(c == 10)
            {
                if( hl == 1 )
                    next_step = 1;
                else if ( hl == 2 )
                    next_step = 2;
                else if( hl ==3 )
                    next_step = 0;
                break;
            } else
                login_highlight(inwin, hl, y-40, 41);
        }
    } else if( old_highlight == 2 ) {
        mvwprintw( inwin, y-52, 54, "  New Username:" );
        mvwgetnstr( inwin, y-52, 71, username, 8 );
        mvwprintw( inwin, y-49, 54, "  New Password:" );
        mvwgetnstr( inwin, y-49, 71, password, 8 );
        mvwprintw( inwin, y-46, 58, "IP Address:" );
        mvwgetnstr( inwin, y-46, 71, ip_addr, 15 );
	    char cstr[login.at(0).size() + 1];
	    strcpy(cstr, login.at(0).c_str());
        mvwprintw( inwin, y-40, 41, cstr );
	    char cstr1[login.at(1).size() + 1];
	    strcpy(cstr1, login.at(1).c_str());
        mvwprintw( inwin, y-40, 63, cstr1 );
	    char cstr2[login.at(2).size() + 1];
	    strcpy(cstr2, login.at(2).c_str());
        mvwprintw( inwin, y-40, 85, cstr2 );
        
        // Cursor is now invisible
        curs_set(0);

        while(1) {
            c = wgetch(inwin);
	        switch(c){
                case KEY_LEFT:
		            if(hl == 1)
		                hl = n_login;
		            else
		                --hl;
		            break;
		        case KEY_RIGHT:
		            if(hl == n_login)
			            hl = 1;
		            else
			            ++hl;
		            break;
                case 10:
                    break;
                default:
		            mvwprintw(inwin, y-30, 1 , "Character pressed is = %3d Hopefully it can be printed as '%c'", c, c);
		            wrefresh(inwin);
		            break;
            }
            if(c == 10)
            {
                if( hl == 1 )
                    next_step = 1;
                else if ( hl == 2 )
                    next_step = 2;
                else if( hl ==3 )
                    next_step = 0;
                break;
            } else
                login_highlight(inwin, hl, y-40, 41);
        }
    } else {
        mvwprintw( inwin, y-52, 58, "  Username:" );
        mvwgetnstr( inwin, y-52, 71, username, 8 );
        mvwprintw( inwin, y-49, 58, "  Password:" );
        mvwgetnstr( inwin, y-49, 71, password, 8 );
	    char cstr[login.at(0).size() + 1];
	    strcpy(cstr, login.at(0).c_str());
        mvwprintw( inwin, y-40, 41, cstr );
	    char cstr1[login.at(1).size() + 1];
	    strcpy(cstr1, login.at(1).c_str());
        mvwprintw( inwin, y-40, 63, cstr1 );
	    char cstr2[login.at(2).size() + 1];
	    strcpy(cstr2, login.at(2).c_str());
        mvwprintw( inwin, y-40, 85, cstr2 );
        
        // Cursor is now invisible
        curs_set(0);

        while(1) {
            c = wgetch(inwin);
	        switch(c){
                case KEY_LEFT:
		            if(hl == 1)
		                hl = n_login;
		            else
		                --hl;
		            break;
		        case KEY_RIGHT:
		            if(hl == n_login)
			            hl = 1;
		            else
			            ++hl;
		            break;
                case 10:
                    break;
                default:
		            mvwprintw(inwin, y-30, 1 , "Character pressed is = %3d Hopefully it can be printed as '%c'", c, c);
		            wrefresh(inwin);
		            break;
            }
            if(c == 10)
            {
                if( hl == 1 )
                    next_step = 1;
                else if ( hl == 2 )
                    next_step = 2;
                else if( hl ==3 )
                    next_step = 0;
                break;
            } else
                login_highlight(inwin, hl, y-40, 41);
        }
    } 

    return next_step;
}

int Interface::login_Page(WINDOW*start, WINDOW*input) {
    int org_x, org_y, new_x, new_y;
    int highlight = 1;
    int choice = 0;
    int chosen_fst, chosen;
    int c;

    initscr();
    clear();
    start_color();
    //noecho();
    cbreak();	// Line buffering disabled. pass on everything
    curs_set(0);

    // Defines all the colors used for the text
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);

    getmaxyx(stdscr,org_y,org_x);

    start = newwin( org_y, org_x, 5, 30 );
    input = newwin( org_y, org_x, 20, 30 );
    keypad(input, TRUE);

    while(1) {
        getmaxyx(stdscr, new_y, new_x);

        if (new_y != org_y || new_x != org_x) {
            org_x = new_x;
            org_y = new_y;

            wresize( start, new_y-57, new_x );
            wresize( input, new_y, new_x );
            mvwin( input, new_y-57, 30 );

            wclear(start);
            wclear(input);
            wclear(stdscr);
        }

	    int num_color;
        int i = 0;
        for(;i<n_title;i++) {
	        char cstr[title.at(i).size() + 1];
	        strcpy(cstr, title.at(i).c_str());
            print_title( start, i, cstr );
	    }

        while(1) {
            i=0;
            int x=44;
            for(;i<n_start;i++)
            {
                char cstr[start_choice.at(i).size() + 1];
                strcpy(cstr, start_choice.at(i).c_str());
	            mvwprintw( input, new_y-57, x, cstr );
                x+=start_choice.at(i).size()+5;
            }
            
            while(1) {
                c = wgetch(input);
	            switch(c){
                    case KEY_LEFT:
	                    if(highlight == 1)
		                    highlight = n_start;
		                else
                        --highlight;
		            break;
		        case KEY_RIGHT:
                    if(highlight == n_start)
                        highlight  = 1;
                    else
                        ++highlight;
                    break;
                case 10:
                    break;
                default:
                    wattron(input, COLOR_PAIR(1));
                    mvwprintw(input,new_y-30, 1 , "You can only press the LEFT, RIGHT, and ENTER keys");
                    wattroff(input, COLOR_PAIR(1));
		            wrefresh(input);
                    break;
                }
                if(c == 10)
                {
                    if (highlight == 1)
                        chosen_fst = 1;
                    else if( highlight == 2 )
                        chosen_fst = 2;
                    else if( highlight == 3 )
                        chosen_fst = 3;
                    break;
                } else
                    start_highlight(input, highlight, new_y-57);
            }

            // The user can see the cursor again
            curs_set(1);

            chosen = login_choice(input,new_y,highlight);
            switch(chosen) {
                case 0:
                    delwin(start);
                    delwin(input);

                    usleep(750);

                    clear();
                    refresh();
                    endwin();
                    break;
                case 1:
                    if(chosen_fst != 3) {
                        delwin(start);
                        delwin(input);

                        usleep(750);

                        clear();
                        refresh();
                        endwin();
                    }    
                    else {
                        wclear(input);
                        wrefresh(input);
                    }
                    break;
                case 2:
                    wclear(input);
                    wrefresh(input);
                    break;
                default:
                    wattron(input, COLOR_PAIR(1));
                    mvwprintw(input,new_y-30, 15 , "You can only press the LEFT, RIGHT, and ENTER keys");
                    wattroff(input, COLOR_PAIR(1));
		            wrefresh(input);
		            break;
            }

            if( chosen == 1) {
                if( chosen_fst != 3 )
                    break;
            }
            else if( chosen != 2 )
                break;
        }
        break;
    }
    return chosen;
}

void Interface::print_option_menu(WINDOW *new_win, int cur_x, int cur_y ) {
    int i = 0;

    wclear(new_win);
    box(new_win, 0, 0);
    for(;i<options.size();i++) { 
        char cstr1[options.at(i).size() + 1];
        strcpy(cstr1, options.at(i).c_str());
        mvwprintw(new_win, cur_y, cur_x, "%s", cstr1 );
        cur_y+=2;
    }
    wrefresh(new_win);
}

void Interface::print_rooms_menu(WINDOW *new_win, int cur_x, int cur_y) {
    int i = 0;

    wclear(new_win);
    box(new_win, 0, 0);
    for(;i<choices.size();i++) { 
        char cstr1[choices.at(i).size() + 1];
        strcpy(cstr1, choices.at(i).c_str());
        mvwprintw(new_win, cur_y, cur_x, "%d: %s", i, cstr1 );
        cur_y+=2;
    }
    wrefresh(new_win);
}

void Interface::print_chat_log(WINDOW*side_win, WINDOW *new_win, int cur_x, int cur_y, int room) {
    int index=0;
    int start_index=0; 
    int end_index=chatrooms[room].size();
    int start_y = 0;

    if(end_index > 37)
        start_index = end_index - 37;

    wclear(new_win);
    box(new_win, 0, 0);
    for(start_index;start_index<end_index;start_index++) {
        char cstr1[chatrooms[room].at(start_index).size() + 1];
        strcpy(cstr1, chatrooms[room].at(start_index).c_str());
        mvwprintw(new_win, ++index, 2, "%s", cstr1 );
        mvwprintw(side_win, start_y, 0, "%d", start_index);
        start_y++;
    }

    wrefresh(new_win);
    wrefresh(side_win);
}

void Interface::option_highlight(WINDOW *inp_win, int highlight, int cur_y) {
    int i = 0;
 
    int cur_x = 9;
    for(; i < options.size(); i++)
	{	
		char cstr[options.at(i).size() + 1];
		strcpy(cstr, options.at(i).c_str());
		if(highlight == i + 1) { // Highlight the present choice
		    wattron(inp_win, A_REVERSE);
		    mvwprintw(inp_win, cur_y, cur_x, "%s", cstr);
	        wattroff(inp_win, A_REVERSE);
		} else
		    mvwprintw(inp_win, cur_y, cur_x, "%s", cstr);
		cur_y+=2;
	}
	wrefresh(inp_win);
}

// Still needs to be fixed
void Interface::chatroom_features(int operation, WINDOW *chatrooms_avail, WINDOW*curr_chat, WINDOW*chatlog, WINDOW*chatlines) {
    int x,y;
    getmaxyx(stdscr, y, x);
    char chatroom_name[11];
    char chat_pass[9];
    int move_rooms = options.size();

    WINDOW* join_room = newwin( 20, 45, 18, 90 );
    box(join_room, 0, 0);

    echo();
    // The user can see the cursor again
    curs_set(1);

    if(operation == 1) {
        mvwprintw(join_room, 1, 15, "Join a Chatroom");
        mvwprintw(join_room, 7, (40-strlen("Enter a chat name:"))/2, "Enter a chat name: ");
        mvwgetnstr(join_room, 7, 30, chatroom_name, 10 );
        mvwprintw(join_room, 12, (40-strlen("Enter the password:"))/2, "Enter the password: ");
        mvwgetnstr(join_room, 12, 30, chat_pass, 8 );
        std::string new_str = std::string(chatroom_name);
        choices.push_back(new_str);
        std::vector<std::string> new_chatroom;
        chatrooms.push_back(new_chatroom);
        cur_room=move_rooms;

        wclear(chatrooms_avail);
        wclear(curr_chat);
        wclear(chatlog);
        wclear(chatlines);
		box(curr_chat,0,0);
        print_rooms_menu(chatrooms_avail, 9, 3);
        print_chat_log(chatlines, chatlog, 1, 1, 0);
        mvwprintw(curr_chat, 1, 70, "%s", chatroom_name);
        wrefresh(curr_chat);
    } else if(operation == 2) {
        mvwprintw(join_room, 1, 13, "Create a Chatroom");
        mvwprintw(join_room, 7, (40-strlen("Enter a chat name:"))/2, "Enter a chat name: ");
        mvwgetnstr(join_room, 7, 30, chatroom_name, 10 );
        mvwprintw(join_room, 12, (40-strlen("Enter the password:"))/2, "Enter the password: ");
        mvwgetnstr(join_room, 12, 30, chat_pass, 8 );
    } else if(operation == 3) {
        mvwprintw(join_room, 1, 13, "Leave a Chatroom");
        mvwprintw(join_room, 7, (40-strlen("Enter a chat name:"))/2, "Enter a chat name: ");
        mvwgetnstr(join_room, 7, 30, chatroom_name, 10 );
        mvwprintw(join_room, 12, (40-strlen("Enter the password:"))/2, "Enter the password: ");
        mvwgetnstr(join_room, 12, 30, chat_pass, 8 );
    } else {
        mvwprintw(join_room, 1, 13, "Delete a Chatroom");
        mvwprintw(join_room, 7, (40-strlen("Enter a chat name:"))/2, "Enter a chat name: ");
        mvwgetnstr(join_room, 7, 30, chatroom_name, 10 );
        mvwprintw(join_room, 12, (40-strlen("Enter the password:"))/2, "Enter the password: ");
        mvwgetnstr(join_room, 12, 30, chat_pass, 8 );
    }

    curs_set(0);
    noecho();

    wrefresh(join_room);
    wclear(join_room);
    wrefresh(join_room);
    delwin(join_room);
}
