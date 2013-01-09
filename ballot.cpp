#include "ballot.hpp"

Ballot::Ballot ( const std::string& data_dir ) :
	data_dir ( data_dir ),
	on (
	{
		{"prepare_voting", std::bind ( &Ballot::on_prepare_voting, std::ref ( *this ), std::placeholders::_1, std::placeholders::_2 ) },
		{"start_voting", std::bind ( &Ballot::on_start_voting, std::ref ( *this ), std::placeholders::_1, std::placeholders::_2 ) },
		{"take_my_vote", std::bind ( &Ballot::on_take_my_vote, std::ref ( *this ), std::placeholders::_1, std::placeholders::_2 ) },
		{"stop_voting", std::bind ( &Ballot::on_stop_voting, std::ref ( *this ), std::placeholders::_1, std::placeholders::_2 ) },
	} )
{
}

Ballot::~Ballot()
{

}

const std::string Ballot::operator() ( const std::string & message )
{
	YAML::Node msg = YAML::Load ( message );
	YAML::Node ret;
	ret["error"] = "OK";
	try
	{
		on.at ( msg["type"].as<std::string>() ) ( msg, ret );
	}
	catch ( ... )
	{
		ret["error"] = "unknown";
	}
	return YAML::Dump ( ret );
}

YAML::Node& Ballot::on_prepare_voting ( const YAML::Node & msg, YAML::Node & ret )
{
	ret["type"] = "voting_prepared";
	Voting ( *this ).on_prepare_voting ( msg, ret );
	return ret;
}

YAML::Node& Ballot::on_start_voting ( const YAML::Node & msg, YAML::Node & ret )
{
	ret["type"] = "voting_started";
	Voting ( *this, msg["vuid"].as<std::string>() ). on_start_voting ( msg, ret );
	return ret;
}

YAML::Node& Ballot::on_take_my_vote ( const YAML::Node & msg, YAML::Node & ret )
{
	ret["type"] = "vote_taken";
	Voting ( *this, msg["vuid"].as<std::string>() ).on_take_my_vote ( msg, ret );
	return ret;
}

YAML::Node& Ballot::on_stop_voting ( const YAML::Node & msg, YAML::Node & ret )
{
	ret["type"] = "voting_stopped";
	Voting ( *this, msg["vuid"].as<std::string>() ).on_stop_voting ( msg, ret );
	return ret;
}

Ballot::Voting::Voting ( const Ballot& parrent ) :
	parent ( parrent )
{
	do
	{
		this->vuid = Integer::Random ( std::string ( "Z0000000000000000" ), std::string ( "ZZZZZZZZZZZZZZZZZ" ) );
	}
	while ( std::fstream ( parrent.data_dir + this->vuid, std::ios_base::in ).good() );
}

Ballot::Voting::Voting ( const Ballot& parrent, const std::string& vuid ) :
	parent ( parrent ),
	vuid ( vuid )
{
	YAML::Node data = YAML::LoadFile ( parrent.data_dir + "/" + this->vuid );
	this->V = data["V"].as<uint>();
	this->O = data["O"].as<uint>();
	this->p = data["p"].as<Integer>();
	this->g = data["g"].as<Integer>();
	for ( auto a: data["A"] )
	{
		this->A.insert ( a.as<Integer>() );
	}
	for ( auto b: data["B"] )
	{
		this->B.insert ( b.as<Integer>() );
	}
	this->G = data["G"].as<Integer>();
	this->P = data["P"].as<Integer>();
}

Ballot::Voting::~Voting()
{
	YAML::Node data;
	data["V"] = this->V;
	data["O"] = this->O;
	data["p"] = this->p;
	data["g"] = this->g;
	for ( auto a: this->A )
	{
		data["A"].push_back ( a );
	}
	for ( auto b: this->B )
	{
		data["B"].push_back ( b );
	}
	data["G"] = this->G;
	data["P"] = this->P;
	std::fstream fstream ( parent.data_dir + "/" + this->vuid, std::ios_base::out );
	fstream << ( data ) << std::endl;
}

YAML::Node& Ballot::Voting::on_prepare_voting ( const YAML::Node & msg, YAML::Node & ret )
{
	ret["vuid"] = this->vuid;
	this->V = msg["data"][0].as<uint>();
	this->O = msg["data"][1].as<uint>();
	this->p = 1;
	this->g = 3;
	this->A = std::set<Integer>();
	this->B = std::set<Integer>();
	this->G = 1;
	this->P = 1;
	for ( uint o = 0; o < this->O; ++o )
	{
		this->p.call ( mpz_nextprime, this->p );
	}
	this->p.call ( mpz_pow_ui, this->p, this->V );
	this->p.call ( mpz_nextprime, this->p );
	ret["data"][0] = this->p;
	ret["data"][1] = this->g;
	return ret;
}

YAML::Node& Ballot::Voting::on_start_voting ( const YAML::Node & msg, YAML::Node & ret )
{
	ret["vuid"] = this->vuid;
	for ( auto a : msg["data"] )
	{
		this->A.insert ( a.as<Integer>() );
	}
	return ret;
}

YAML::Node& Ballot::Voting::on_take_my_vote ( const YAML::Node & msg, YAML::Node & ret )
{
	ret["vuid"] = this->vuid;
	if ( std::find ( this->A.begin(), this->A.end(), msg["data"][0].as<Integer>() ) != this->A.end() )
	{
		if ( std::find ( this->B.begin(), this->B.end(), msg["data"][0].as<Integer>() ) == this->B.end() )
		{
			if ( Integer::Call ( mpz_powm, this->g, msg["data"][1].as<Integer>(), this->p ) == ( Integer::Call ( mpz_powm, msg["data"][0].as<Integer>(), msg["data"][2].as<Integer>(), this->p ) * Integer::Call ( mpz_powm, msg["data"][2].as<Integer>(), msg["data"][3].as<Integer>(), this->p ) ) % this->p )
			{
				this->B.insert ( msg["data"][0].as<Integer>() );
				this->G = ( this->G * msg["data"][0].as<Integer>() ) % this->p;
				this->P = ( this->P * msg["data"][1].as<Integer>() ) % this->p;
			}
			else throw "wrong_signature";
		}
		else throw "aleardy_voted";
	}
	else "not_authorized";
	return ret;
}

YAML::Node& Ballot::Voting::on_stop_voting ( const YAML::Node & msg, YAML::Node & ret )
{
	ret["vuid"] = this->vuid;
	ret["data"][0] = G;
	ret["data"][1] = P;
	return ret;
}
