//
// chat_server.cpp
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
#include <list>
#include <memory>
#include <set>
#include <utility>
#include "asio.hpp"
#include "chat_message.hpp"

#include <vector>
#include <fstream>
#include <string.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

using asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_queue;
std::vector<std::string> users;
std::vector<std::string> passwords;
std::vector<std::string> logged_in_users;
int current_number ;

std::string filename = "sp.txt";

//----------------------------------------------------------------------

class chat_participant
{
public:
  virtual ~chat_participant() {}
  virtual void deliver(const chat_message& msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

//----------------------------------------------------------------------

class chat_room
{
public:

  void join(chat_participant_ptr participant)
  {

    for (auto msg: recent_msgs_){

      participant->deliver(msg);


	}


  }

  void leave(chat_participant_ptr participant)

  {

    participants_.erase(participant);
  }

  void deliver(const chat_message& msg)
  {
    recent_msgs_.push_back(msg);
  //  save_file_msg(this);
    while (recent_msgs_.size() > max_recent_msgs)
      recent_msgs_.pop_front();

    for (auto participant: participants_)
      participant->deliver(msg);
  }

  chat_message_queue get_messages()
  {
    return recent_msgs_;
  }
  void add_message(const chat_message& msg)
  {
    recent_msgs_.push_back(msg);
  }

private:
  std::set<chat_participant_ptr> participants_;
  enum { max_recent_msgs = 500 };
  chat_message_queue recent_msgs_;
};

void load_msg(chat_room& , std::string);

//----------------------------------------------------------------------

class chat_session
  : public chat_participant,
    public std::enable_shared_from_this<chat_session>
{
public:
  chat_session(tcp::socket socket, chat_room& room)
    : socket_(std::move(socket)),
      room_(room)
  {
  }

  void start()
  {
    if(logged_in_users.size() >= 50)
      {

        return;
      }
    room_.join(shared_from_this());
    do_read_header();
  }

  void deliver(const chat_message& msg)
  {
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress)
    {
      do_write();
    }
  }

private:
  void do_read_header()
  {
    auto self(shared_from_this());
    asio::async_read(socket_,
        asio::buffer(read_msg_.data(), chat_message::header_length),
        [this, self](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec && read_msg_.decode_header())
          {
            printf("\n%s",read_msg_.body());
            do_read_body();
          }
          else
          {
            room_.leave(shared_from_this());
          }
        });
  }

  void do_read_body()
  {
    auto self(shared_from_this());
    asio::async_read(socket_,
        asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this, self](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {

            room_.deliver(read_msg_);
            do_read_header();
          }
          else
          {
            room_.leave(shared_from_this());
          }
        });
  }

  void do_write()
  {
    auto self(shared_from_this());
    asio::async_write(socket_,
        asio::buffer(write_msgs_.front().data(),
          write_msgs_.front().length()),
        [this, self](std::error_code ec, std::size_t /*length*/)
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
            room_.leave(shared_from_this());
          }
        });
  }

  tcp::socket socket_;
  chat_room& room_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};
//std::vector<chat_room> chat_rooms;

//----------------------------------------------------------------------

class chat_server
{
public:
  chat_server(asio::io_context& io_context,
      const tcp::endpoint& endpoint)
    : acceptor_(io_context, endpoint)
  {
    load_msg(room_,"room1.txt");
    /*load_msg(room2,"room2.txt");
    load_msg(room3,"room3.txt");
    load_msg(room4,"room4.txt");
    load_msg(room5,"room5.txt");
    load_msg(room6,"room6.txt");
    load_msg(room7,"room7.txt");
    load_msg(room8,"room8.txt");
    load_msg(room9,"room9.txt");
    load_msg(room10,"room10.txt");*/
    do_accept();
  }

private:
  void do_accept()
  {
    acceptor_.async_accept(
        [this](std::error_code ec, tcp::socket socket)
        {
          if (!ec)
          {
            std::make_shared<chat_session>(std::move(socket), room_)->start();
          }

          do_accept();
        });
  }

  tcp::acceptor acceptor_;
  chat_room room_;
 // chat_rooms.push_back(room_);
  //chat_room room2, room3, room4, room5, room6,room7,room8,room9,room10;
};

//----------------------------------------------------------------------

void load_file()
{
  std::string usernames;
  std::string passes;
  std::string message;
  std::string temp;
  std::fstream file;
  std::string line;
  //char ln[chat_message::max_body_length + 1];

  file.open(filename); //change to proper name of load_file
  if(file.is_open())
  {
    std::getline(file,line);
    std::stringstream ss(line);

    while (std::getline(ss,temp,'>'))
      users.push_back(temp);

    //passwords
    std::getline(file,line);
    std::stringstream sa(line);

    while (std::getline(sa,temp,'>'))
      passwords.push_back(temp);
    file.close();
  }
    else std::cerr << "Cannot load file";
}


void load_msg(chat_room& roomit, std::string filenamer)
{
  std::string usernames;
  std::string passes;
  std::string message;
  std::string temp;
  std::fstream file;
  std::string line;
  //char ln[chat_message::max_body_length + 1];
    file.open("room1.txt");
//    if(file.is_open()){
    while(std::getline(file,line))
    {
      char b[line.length()];
      strcpy(b,line.c_str());
      chat_message msg;
      msg.body_length(std::strlen(line.c_str()));
      std::memcpy(msg.body(), line.c_str(), msg.body_length());
      msg.encode_header();
      roomit.add_message(msg); //need to push messages to recent_msgs_
    }
//  }
//  else{
  //  std::cerr << "can't ojpen file" ;
//  }
    file.close();

  return;
}

void save_file(std::string line)
{
  std::vector<std::string> msg;
  std::string temp;
  std::stringstream ss(line);

  while (std::getline(ss,temp,'>'))
    msg.push_back(temp);

  std::fstream file;
  file.open(filename);
  char d = msg[0].at(0);
  if(d == '&')
  {
    file.seekg(-1,std::ios_base::end);

    bool kl = true;
    while(kl)
    {
      char c;
      file.get(c);
      if((int)file.tellg() <= 1)
      {
        file.seekg(0);
        kl = false;
      }
      else if(c == '\n')
      {
        kl = false;
      }
      else
      {
        file.seekg(-2,std::ios_base::cur);
      }
    }
    file << line <<std::endl;
    file.close();
  }
  else if(file.is_open() && d == '!')
  {
    for(size_t i = 0; i < users.size(); i++)
    {
      file << users[i] << ";";
    }
    file << std::endl;
    for(size_t i = 0; i < users.size(); i++)
    {
      file << passwords[i] << ";";
    }
    file <<std::endl;
      file.close();
    }
  else
    return;

}


int main(int argc, char* argv[])
{
  try
  {
    /*
    if (argc < 2)
    {
      std::cerr << "Usage: chat_server <port> [<port> ...]\n";
      return 1;
    }
*/
	current_number = 0;
    asio::io_context io_context;

    char * port;
	port = (char*)malloc(5);
	std::string strprt = "9000";
	strcpy(port, strprt.c_str());
	argv[1] = port;
    std::list<chat_server> servers;
    //for (int i = 1; i < argc; ++i)
    //{
      tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[1]));
      servers.emplace_back(io_context, endpoint);
  //  }
    load_file();


    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
