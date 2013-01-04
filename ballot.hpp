#pragma once

#include <string>
#include <map>
#include <set>
#include <fstream>
#include <utility>
#include <functional>
#include <yaml-cpp/yaml.h>
#include <integer.hpp>
#include <integer.yaml.hpp>

class Ballot
{
	private:
		const std::string data_dir;
	public:
		Ballot() = delete;
		Ballot ( const Ballot & ) = delete;
		Ballot ( Ballot && ) = delete;
		Ballot & operator= ( const Ballot & ) = delete;
		Ballot & operator= ( Ballot && ) = delete;
	public:
		Ballot ( const std::string& );
		const std::string operator() ( const std::string & );
		~Ballot();
	private:
		const std::map<std::string, std::function<YAML::Node ( const YAML::Node & ) >> on;
		YAML::Node on_prepare_voting ( const YAML::Node & );
		YAML::Node on_start_voting ( const YAML::Node & );
		YAML::Node on_take_my_vote ( const YAML::Node & );
		YAML::Node on_stop_voting ( const YAML::Node & );
	private:
		struct Voting
		{
			const Ballot& parrent;
			std::string vuid;
			YAML::Node data;
			Voting() = delete;
			Voting ( const Voting & ) = delete;
			Voting ( Voting && ) = delete;
			Voting & operator= ( const Voting & ) = delete;
			Voting & operator= ( Voting && ) = delete;
			Voting ( const Ballot& );
			Voting ( const Ballot&, const std::string& );
			~Voting();
		};
};
