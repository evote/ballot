#include "ballot.hpp"

Ballot::Ballot () :
	on (
{
	{"prepare_voting", std::bind ( &Ballot::on_prepare_voting, std::ref ( *this ), std::placeholders::_1 ) },
	{"start_voting", std::bind ( &Ballot::on_start_voting, std::ref ( *this ), std::placeholders::_1 ) },
	{"take_my_vote", std::bind ( &Ballot::on_take_my_vote, std::ref ( *this ), std::placeholders::_1 ) },
	{"stop_voting", std::bind ( &Ballot::on_stop_voting, std::ref ( *this ), std::placeholders::_1 ) },
} )
{
}

Ballot::~Ballot()
{

}

std::string Ballot::process ( std::string message )
{
	YAML::Node msg = YAML::Load ( message );
	try
	{
		return YAML::Dump ( on.at ( msg["type"].as<std::string>() ) ( msg ) );
	}
	catch ( ... )
	{
		YAML::Node ret;
		ret["error"] = "unknown";
		return YAML::Dump ( ret );
	}
}

YAML::Node Ballot::on_prepare_voting ( const YAML::Node & msg )
{
	YAML::Node ret;
	ret["type"] = "voting_prepared";
	ret["error"] = "OK";

	try
	{
		uint V = msg["data"][0].as<uint>();
		uint O = msg["data"][1].as<uint>();
		Integer p ( 1 ), q ( 3 );
		for ( uint o = 0; o < O; ++o )
		{
			p.call ( mpz_nextprime, p );
		}
		p.call ( mpz_pow_ui, p, V );
		p.call ( mpz_nextprime, p);
		ret["data"][0] = p;
		ret["data"][1] = q;
	}
	catch ( ... )
	{
		ret["error"] = "unknown";
	}

	return ret;
}

YAML::Node Ballot::on_start_voting ( const YAML::Node & msg )
{
	YAML::Node ret;
	ret["type"] = "voting_started";
	ret["error"] = "OK";

	try
	{

	}
	catch ( ... )
	{
		ret["error"] = "unknown";
	}

	return ret;
}

YAML::Node Ballot::on_take_my_vote ( const YAML::Node & msg )
{
	YAML::Node ret;
	ret["type"] = "vote_taken";
	ret["error"] = "OK";

	try
	{

	}
	catch ( ... )
	{
		ret["error"] = "unknown";
	}

	return ret;
}

YAML::Node Ballot::on_stop_voting ( const YAML::Node & msg )
{
	YAML::Node ret;
	ret["type"] = "voting_stoped";
	ret["error"] = "OK";

	try
	{

	}
	catch ( ... )
	{
		ret["error"] = "unknown";
	}

	return ret;
}

