#include "interface.hpp"
#include <ncurses.h>

int main() {
    Interface usr_interface;
    WINDOW *startup = NULL;
    WINDOW *inputscreen = NULL;

    int next_step = usr_interface.login_Page( startup, inputscreen );

    if( next_step == 0 ) {
        endwin();
        return 0;
    }

    else {
        WINDOW *chat_log = NULL;
	WINDOW *message_block = NULL;
        WINDOW *word_recc = NULL;
	WINDOW *chats = NULL;
        WINDOW *chat_options = NULL;
	WINDOW *curr_chat = NULL;

        usr_interface.chatroom_Page( chat_log, message_block, word_recc, chats, chat_options, curr_chat );

        return 0;
    }
}
