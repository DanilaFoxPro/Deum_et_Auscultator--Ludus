#include "string.h"

#include <cstdlib> // rand

#include "file.h"

namespace stringh {
	
	//Splits string into words
	std::vector<std::string> splitString(std::string input, char spliton){
		
		if(!input.size())
			return {input};
		
		std::string current_word;
		std::vector<std::string> output;
		
		for(unsigned int i = 0; input[i] != 0; i++){
			
			if(input[i]==spliton){
				
				if(current_word.size()){
					output.push_back(current_word);
					current_word.clear();
				}
				
			} else {
				
				current_word.push_back(input[i]);
				
			}
			
		}
		
		if(current_word.size())
			output.push_back(current_word);
		
		return output;
		
	}
	
	enum {
		sem_normal,
		sem_space,
		sem_quotes,
	};
	
	std::vector<std::string> parseString ( std::string text )
	{
		//Seeking_Mode remebers what the loop is searching for,
		//so it kind'a cuts down on code.
		//
		//Basically Seeking_Mode is a state, and it can change
		//depending on the code in the state itself. Allowing
		//for cool stuff to happen and everything to work just
		//fine.
		unsigned int seeking_mode = sem_normal;
		bool advance = true;
		
		std::vector<std::string> output;
		std::string current;
		
		unsigned int text_size = text.size();
		for(unsigned int i = 0; i < text_size;){
			
			switch(seeking_mode){
				
				case sem_normal:
				{
					if(text[i]!=' '){
						if(text[i]=='"'){
							seeking_mode = sem_quotes;
						} else {
							seeking_mode = sem_space;
						}
					}
					break;
				}
				
				case sem_space:
				{
					if(text[i]==' ')
					{
						seeking_mode = sem_normal;
						advance = false;
					}
					break;
				}
				
				case sem_quotes:
				{
					if(text[i]=='"')
					{
						current.push_back(text[i]);
						seeking_mode = sem_normal;
					}
					break;
				}
				
				default:
				{
					break;
				}
				
			}
			
			if(seeking_mode != sem_normal){
				current.push_back(text[i]);
			} else {
				if(current.size()){
					output.push_back(current);
					current.clear();
				}
			}
			
			if(advance)
				i++;
			
			advance = true;
		}//for each char in text
		
		if(current.size())
			output.push_back(current);
		
		return output;
		
	}
	
	
	std::string unquote(std::string input){
		
		if(input.size()<2)
			return input;
		
		if( (input[0] != '"') and (input[input.size()-1] != '"'))
			return input;
		
		input.erase( input.begin() );
		input.pop_back();
		
		return input;
		
	}
	
	std::string padTo(std::string input, unsigned int length){
		
		if(input.length()>=length){
			input.resize(length);
			return input;
		}
		
		return (input + std::string(length-input.length(), ' '));
		
	}
	
	bool isInteger(std::string input){
		
		for(char cur : input){
			
			if(!isdigit(cur)){
				return false;
			}
			
		}
		
		return input.size();
		
	}
	bool isFloat(std::string input){
		
		unsigned int point_count = 0;
		
		for(char cur : input){
			
			if(!isdigit(cur)){
				if(cur != '.')
					return false;
				else
					point_count++;
			}
			
		}
		
		return (point_count<2&&input.size());
		
	}
	
	std::string randomName(){
		
		std::vector<std::string> names = file::getLines("names.txt");
		
		if(!names.size())
			return "";
		
		return names[rand()%names.size()];
		
	}
		
}

