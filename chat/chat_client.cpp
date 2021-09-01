//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "asio.hpp"
#include "chat_message.hpp"
#include "interface.hpp"
#include <ncurses.h>
#include <type_traits>


using asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

Interface usr_interface;

class chat_client
{
public:
  chat_client(asio::io_context& io_context,
      const tcp::resolver::results_type& endpoints)
    : io_context_(io_context),
      socket_(io_context)
  {
    do_connect(endpoints);
  }

  void write(const chat_message& msg)
  {
    asio::post(io_context_,
        [this, msg]()
        {
          bool write_in_progress = !write_msgs_.empty();
          write_msgs_.push_back(msg);
          if (!write_in_progress)
          {
            do_write();
          }
        });
  }

  void close()
  {
    asio::post(io_context_, [this]() { socket_.close(); });
  }

private:
  void do_connect(const tcp::resolver::results_type& endpoints)
  {
    asio::async_connect(socket_, endpoints,
        [this](std::error_code ec, tcp::endpoint)
        {
          if (!ec)
          {
            do_read_header();
          }
        });
  }

  void do_read_header()
  {
    asio::async_read(socket_,
        asio::buffer(read_msg_.data(), chat_message::header_length),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec && read_msg_.decode_header())
          {
            do_read_body();
          }
          else
          {
            socket_.close();
          }
        });
  }

  void do_read_body()
  {
    asio::async_read(socket_,
        asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
						//std::cerr.write(read_msg_.body(), read_msg_.body_length());
            //std::cerr << "\n";
            //------------------------------################################################
            //###############################################################################################
					std::string complete_string(read_msg_.body(),read_msg_.body_length());
						//std::cerr << complete_string << "s";
						usr_interface.chatrooms[usr_interface.cur_room].push_back(complete_string);
						wclear(usr_interface.lines);
						wclear(usr_interface.log);
						//usr_interface.add_to_chatroom();
						usr_interface.print_chat_log(usr_interface.lines,usr_interface.log,1,1,usr_interface.cur_room);
						wrefresh(usr_interface.lines);
						wrefresh(usr_interface.log);
						//wrefresh(usr_interface.message);


            do_read_header();
          }
          else
          {
            socket_.close();
          }
        });
  }

  void do_write()
  {
    asio::async_write(socket_,
        asio::buffer(write_msgs_.front().data(),
          write_msgs_.front().length()),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            write_msgs_.pop_front();
            if (!write_msgs_.empty())
            {
              do_write();
            }
          }
          else
          {
            socket_.close();
          }
        });
  }

private:
  asio::io_context& io_context_;
  tcp::socket socket_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};

int main(int argc, char* argv[])
{
  try{
    while(1){

    WINDOW *startup = NULL;
      WINDOW *inputscreen = NULL;

      int next_step = usr_interface.login_Page( startup, inputscreen );

      if( next_step == 0 )
      {
          endwin();
          return 0;
      }

      else
      {
  	  char * ip_address = usr_interface.get_ip();
      asio::io_context io_context;
      char * port ;//= "9000";
	    port = (char*)malloc(5);
	    std::string strprt = "9000";
      strcpy(port, strprt.c_str());
      argv[2] = port;
      argv[1] = ip_address;
      tcp::resolver resolver(io_context);
      auto endpoints = resolver.resolve(argv[1], argv[2]);
      chat_client cl(io_context, endpoints);

  	//chatroom start
  	//------------------------------------------------------------------------------------------
  	//------------------------------------------------------------------------------------------

	   char * username = usr_interface.get_username();
	    //char msg[];
  	   int x,y;
      int highlight = 1;
      int c;

      // Stores what the user types
      std::string add_msg;
      // Stores how the string should look like in the window
      std::string complete_string;

      // Prints the numbers on the side of the chatlog window
      usr_interface.lines = newwin(38,5,4,188);

      cbreak();	// Line buffering disabled. pass on everything
      curs_set(0);

      // Stores current terminal length in y and widith in x
      getmaxyx(stdscr,y,x);
      y++;
      // Changes the color of the text for the instructions in the bottom left of the screen
      attron(COLOR_PAIR(3));
      mvprintw(45, 11, "Type UP or DOWN for the Chat Ops.");
      // Turns the color changer off
      attroff(COLOR_PAIR(3));
      attron(COLOR_PAIR(6));
      mvprintw(47, 11, "Type a number to change rooms.");
      attroff(COLOR_PAIR(6));
      attron(COLOR_PAIR(5));
      mvprintw(49, 11, "Press 't' to type a message.");
      attroff(COLOR_PAIR(5));

      // Initializes window that shows you what chatroom you're currently in at the top
      usr_interface.curr = newwin( 3, 140, 0, 46 );
      // Initializes window that shows whats been written in the chatroom
      usr_interface.log = newwin( 40, 140, 3, 46 );
      // Initializes window that prints out the chat operations at the top left
      usr_interface.chat_ops = newwin( 14, 30, 3, 15 );
      // Initializes window that prints all the rooms the user is apart of
      usr_interface.rooms = newwin( 26, 30, 17, 15 );
      // Initializes window that prints the box that the user will type a message in
      usr_interface.message = newwin( 10, 140, 43, 46 );

      // Draws boxes around
      box(usr_interface.curr, 0, 0);
      box(usr_interface.log, 0, 0);
      box(usr_interface.message, 0, 0);

      // Converts the name of the chatroom that the user is currently in from a string to char*
      char cstr[usr_interface.choices.at(usr_interface.cur_room).size() + 1];
      strcpy(cstr, usr_interface.choices.at(usr_interface.cur_room).c_str());
      mvwprintw(usr_interface.curr, 1, 70, "%s", cstr);

      // Functions that update and fill whats inside the operations, rooms, and chatlog window
      usr_interface.print_option_menu(usr_interface.chat_ops, 9, 2 );
      usr_interface.print_rooms_menu(usr_interface.rooms, 9, 3);
      usr_interface.print_chat_log(usr_interface.lines, usr_interface.log, 1, 1, 0);

      // Prints the name of the user in the message box
      mvwprintw(usr_interface.message, 1, 1, "%s: ", usr_interface.get_username() );

      // Updates how each window looks like
      wrefresh(usr_interface.curr);
      wrefresh(usr_interface.rooms);
      wrefresh(usr_interface.log);
      wrefresh(usr_interface.chat_ops);
      wrefresh(usr_interface.message);
      wrefresh(usr_interface.lines);

      // Updates the screen behind the windows
      refresh();

      // Stores whether or not the user wants to join, create, or delete a chatroom
      int op_choice;
      // Doesn't show whatever the user types
      noecho();

  	std::thread t([&io_context](){ io_context.run(); });
						  chat_message m;
     while(1) {
        // Allows the user to navigate through the join, create, or delete options at the top right window
        c=wgetch(usr_interface.chat_ops);
        switch(c) {
            // Catches if the user type the up arrow key
            case 'A':
	            if(highlight == 1) // Highlight starts at the furthest most option
                    highlight = usr_interface.n_options;
    	        else
                    --highlight;
		        break;
            // Catches if the user type the down arrow key
	        case 'B':
                if(highlight == usr_interface.n_options) // Highlight starts at the earliest option
                    highlight  = 1;
                else
                    ++highlight;
                break;
            // Catches when the user wants to type a message to the chat
            case 't':
                // Allows the user to see what they're typing in
                echo();
                // Allows the user to see their cursor
                curs_set(1);
                // Input from the message box window that lets the user write a message
                mvwgetnstr( usr_interface.message, 1, sizeof(username) + 2, usr_interface.msg, 512 );

                // Converts char* to string in order to be stored into filelog of all the messages sent
                add_msg = std::string(usr_interface.msg);
                // Stores the message plus the person who sent it
                complete_string = std::string(username) + ": " + usr_interface.msg;
                // chatrooms is a vector of a vector strings, so it stores the message string into the vector of
                // vectors of all the messages inside the chat room the user is currently in.
                //chatrooms[usr_interface.cur_room].push_back(complete_string);
                 	  char line[chat_message::max_body_length + 1];
					  strcpy(line, complete_string.c_str());

					  m.body_length(std::strlen(line));
					  std::memcpy(m.body(), line, m.body_length());
						m.encode_header();
						cl.write(m);
                // Erases the original log, message, and lines windows from before in order to be redrawn with the new info
                //wclear(usr_interface.log);
                wclear(usr_interface.message);
                //wclear(usr_interface.lines);

                // User can't see cursor anymore
                curs_set(0);
                // User can't see what they're typing anymore
                noecho();
                // Redraws the message box to have the user's name in the box again
                mvwprintw(usr_interface.message, 1, 1, "%s: ", username );
                // Draws a box around the message box
                box(usr_interface.message, 0, 0);

                // Changes will now print to screen
                wrefresh(usr_interface.message);
                //wrefresh(usr_interface.lines);
                //usr_interface.print_chat_log(usr_interface.lines, usr_interface.log, 1, 1, usr_interface.cur_room);
                break;
            // User presses the enter key
            case 10:
                break;
            // Will just refresh the page behind all the windows
            default:
	            refresh();

                break;
        }
        // Want to catch if they hit the enter key in order to catch if the user wants to join,
        // create, leave, or logout from a chatroom
        if( c == 10 ) {
            // Stores whatever choice the user wants to pick: either join, create, leave, logout
            op_choice = highlight;

            // 5 denotres that they want to logout
            if(op_choice != 5) {
                // Function that deals with creating, deleting and joining chatrooms
                usr_interface.chatroom_features(op_choice, usr_interface.rooms, usr_interface.curr, usr_interface.log, usr_interface.lines);
                refresh();
            } else {
	            refresh();
                // Want to logout from the program
                break;

            }
            // If the user wants to get one of the op_choice choices, they need to break away from the highlight
            // Breaking means that the while loop that continues highlighting the choices will finally break away
        } else if(c>=48 && c <=57) { // Catches the ASCII values for 0-9, needed to switch chatrooms
            int index=0;
            for(;(unsigned long)index < usr_interface.inputs.size();index++) { // Searches through a vector of ints to find what chatroom they want to switch to
                if(usr_interface.inputs[index] == c) {
                    usr_interface.cur_room=index; // Updates their current room
                }
            }
            // Makes sure they can only choose chatrooms within the number chatrooms not just press random numbers to go somewhere
            if((unsigned long)usr_interface.cur_room < usr_interface.chatrooms.size()) {
                // Removes these windows to allow for the windows to be rewritten with the new info
                wclear(usr_interface.log);
                wclear(usr_interface.curr);
                wclear(usr_interface.lines);
                box(usr_interface.log, 0, 0);
                box(usr_interface.curr, 0, 0);

                // Changes what room they're in by displaying the name at the top of the out
                char new_cstr[usr_interface.choices.at(usr_interface.cur_room).size() + 1];
                strcpy(new_cstr, usr_interface.choices.at(usr_interface.cur_room).c_str());
                mvwprintw(usr_interface.curr, 1, 70, "%s", new_cstr);
                // Updates the new chatlog window to display what's stored within the chatroom they want to switch to
                usr_interface.print_chat_log(usr_interface.lines, usr_interface.log, 1, 1, usr_interface.cur_room);

                // The windows will now be updated with the new information
                wrefresh(usr_interface.curr);
                wrefresh(usr_interface.log);
                wrefresh(usr_interface.lines);
            }
        }

        // The user will keep on highlighting the top left options in order until theby hit enter to continue the process
        else {
            usr_interface.option_highlight(usr_interface.chat_ops, highlight, 2);
        }
    }

      // Clears out each window from showing anything in the terminal
      wclear(usr_interface.curr);
      wclear(usr_interface.rooms);
      wclear(usr_interface.log);
      wclear(usr_interface.chat_ops);
      wclear(usr_interface.message);
      wclear(usr_interface.lines);
      clear();
      clrtoeol();

      // Updates the new screen to look like a blank screen
      wrefresh(usr_interface.curr);
      wrefresh(usr_interface.rooms);
      wrefresh(usr_interface.log);
      wrefresh(usr_interface.chat_ops);
      wrefresh(usr_interface.message);
      refresh();
      // Ends the ncurses feature
      endwin();
         //t.join();
          cl.close();
          t.join();
        //  return 0;
  }
    }
	}

  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
/*

    char line[chat_message::max_body_length + 1];
    while (std::cin.getline(line, chat_message::max_body_length + 1))
    {
      chat_message msg;
      msg.body_length(std::strlen(line));
      std::memcpy(msg.body(), line, msg.body_length());
      msg.encode_header();
      c.write(msg);
    }

    c.close();
		f.join();
    t.join();
*/
