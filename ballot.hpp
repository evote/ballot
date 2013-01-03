#pragma once

#include <string>
#include <map>
#include <utility>
#include <functional>
#include <yaml-cpp/yaml.h>

class Ballot
{
	public:
		//Ballot() = delete;
		Ballot ( const Ballot & ) = delete;
		Ballot ( Ballot && ) = delete;
		Ballot & operator= ( const Ballot & ) = delete;
		Ballot & operator= ( Ballot && ) = delete;
	public:
		Ballot ();
		std::string process ( std::string );
		~Ballot();
	private:
		const std::map<std::string, std::function<YAML::Node ( const YAML::Node & ) >> on;
		YAML::Node on_prepare_voting ( const YAML::Node & );
		YAML::Node on_start_voting ( const YAML::Node & );
		YAML::Node on_take_my_vote ( const YAML::Node & );
		YAML::Node on_stop_voting ( const YAML::Node & );
};
