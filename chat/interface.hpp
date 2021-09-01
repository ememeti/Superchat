#ifndef INTERFACE_H
#define INTERFACE_H
#include <ncurses.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <vector>

class Interface {
    private:
        char username[9];
        char password[9];
        char ip_addr[16];


    public:
	    char msg[513];
        std::vector<std::string> title;
        std::vector<std::string> login;
        std::vector<std::string> choices;
        std::vector<std::string> all_messages;
        std::vector<std::string> all_messages1;
        std::vector<std::string> all_messages2;
        std::vector<std::string> all_messages3;
        std::vector<std::vector<std::string> > chatrooms;
        std::vector<int> inputs;

        std::vector<std::string> options;
        std::vector<std::string> start_choice;


		WINDOW * curr = NULL;
		WINDOW * log = NULL;
		WINDOW * chat_ops = NULL;
		WINDOW * rooms = NULL;
		WINDOW * message = NULL;
		WINDOW* lines = NULL;

	    int n_title;
        int n_choices;
        int n_messages;
        int n_options;
        int n_login;
        int n_start;

        int cur_room;

	    Interface();
        char* get_ip();
        char* get_username();
        char* get_password();
        void add_message(char * string, int length);
        void login_highlight(WINDOW *inp_win, int highlight, int cur_y, int cur_x);
        void start_highlight(WINDOW *inp_win, int highlight, int cur_y);
        void print_title(WINDOW*start, int index, char* part);
        int login_choice( WINDOW* inwin, int y, int hl);
        int login_Page(WINDOW*start, WINDOW*input);
        void print_option_menu(WINDOW *new_win, int cur_x, int cur_y );
        void print_rooms_menu(WINDOW *new_win, int cur_x, int cur_y );
        void print_chat_log(WINDOW*side_win, WINDOW *new_win, int cur_x, int cur_y, int room);
        void option_highlight(WINDOW *inp_win, int highlight, int cur_y);
        int option_choice(WINDOW *inp_win, int hl);
        void chatroom_features(int operation, WINDOW *chatrooms_avail, WINDOW*curr_chat, WINDOW*chatlog, WINDOW*chatlines);
        void chatroom_Page( WINDOW*log, WINDOW*message, WINDOW*rooms, WINDOW*chat_ops, WINDOW*curr);
};

#endif
