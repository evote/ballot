#include "ballot.hpp"

Ballot::Ballot () :
	on (
		{
			{"on_prepare_voting", std::bind ( &Ballot::on_prepare_voting, std::ref ( *this ), std::placeholders::_1 ) },
			{"on_start_voting", std::bind ( &Ballot::on_start_voting, std::ref ( *this ), std::placeholders::_1 ) },
			{"on_take_my_vote", std::bind ( &Ballot::on_take_my_vote, std::ref ( *this ), std::placeholders::_1 ) },
			{"on_stop_voting", std::bind ( &Ballot::on_stop_voting, std::ref ( *this ), std::placeholders::_1 ) },
		} )
{
}

Ballot::~Ballot()
{

}

std::string Ballot::process ( std::string message )
{

	YAML::Node msg ( message );
	return YAML::Dump ( on.at ( msg["type"].as<std::string>() ) ( msg ) );
}

YAML::Node Ballot::on_prepare_voting ( const YAML::Node & msg )
{
	YAML::Node ret;
	ret["type"] = "voting_prepared";
	return ret;
}

YAML::Node Ballot::on_start_voting ( const YAML::Node & msg )
{
	YAML::Node ret;
	ret["type"] = "voting_started";
	return ret;
}

YAML::Node Ballot::on_take_my_vote ( const YAML::Node & msg )
{
	YAML::Node ret;
	ret["type"] = "vote_taken";
	return ret;
}

YAML::Node Ballot::on_stop_voting ( const YAML::Node & msg )
{
	YAML::Node ret;
	ret["type"] = "voting_stoped";
	return ret;
}

